/**
 * Copyright (c) Andreas Fitzek <andreas[dot]fitzek[at]gmail[dot]com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce 
 * the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or 
 * other materials provided with the distribution. 
 * Neither the name of the nor the names of its contributors
 * may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
/*
 * atags.c
 *
 *  Created on: Aug 31, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common.h>
#include <kprintf.h>
#include <mm/mm.h>

/**
 * \defgroup atags ATAGS - Bootparameters
 *
 * Bootparameter functions
 */

/**
 * The current ATAGS
 *
 * These are set by copyAtags
 */
struct atag* cur_atags;

struct atag* atag_copy(struct atag* startTag) {
	struct atag* atag = startTag;
	uint8_t* ptr = (uint8_t*) startTag;

	uint32_t size = 0;
	do {
		size += (atag->hdr.size * 4);
		ptr = (uint8_t*) atag;
		atag = (struct atag*) (ptr + (atag->hdr.size * 4));
	} while (atag->hdr.tag != ATAG_NONE);
	size += (2 * 4);
	struct atag* newTag = (struct atag*) kmalloc(size);
	memcpy(newTag, startTag, size);
	cur_atags = newTag;
	return (newTag);
}

struct atag* atag_get_current() {
	return (cur_atags);
}

void atag_generate_nonsecure(uintptr_t start,
		uint32_t rdstart, uint32_t rdsize, const char* cmdl) {
	atag_setup_core(start);
	atag_setup_revision();
	const char* cmdline = atag_get_guest_cmdline(atag_get_current());
	if(cmdline == NULL) {
		cmdline = cmdl;
	}
	list* pos = NULL;
	list* next = NULL;
	list* phys_mem = pmm_get_mem_list();
	list_for_each_safe(pos, next, phys_mem)
	{
		phys_mem_area* area = (phys_mem_area*) pos->data;
		if (area) {
			if (area->type == MEM_TYPE_UNSECURE) {
				atag_setup_mem(area->pstart, area->pend - area->pstart);
			}
		}
	}

	if (cmdline != NULL ) {
		atag_setup_cmdline(cmdline);
		kfree((uintptr_t)cmdline);
	}
	if (rdstart != 0) {
		atag_setup_initrd(rdstart, rdsize);
	}
	//atag_setup_mem(0x70000000, 0x20000000); // TODO: We should generate this
	//atag_setup_mem(0xC0000000, 0x10000000); // TODO: We should generate this
	atag_setup_end();
}

const char* atag_get_cmdline(struct atag* startTag) {
	struct atag* atag = startTag;
	uint8_t* ptr = (uint8_t*) startTag;
	do {
		if (atag->hdr.tag == ATAG_CMDLINE) {
			return ((const char*)atag->u.cmdline.cmdline);
		}
		ptr = (uint8_t*) atag;
		atag = (struct atag*) (ptr + (atag->hdr.size * 4));
	} while (atag->hdr.tag != ATAG_NONE);
	return (NULL);
}

const char* atag_get_guest_cmdline(struct atag* startTag) {
	const char* cmdline = atag_get_cmdline(startTag);
	char* ncmdline = NULL;
	if(cmdline != NULL) {
		ncmdline = (char*) kmalloc(strlen(cmdline) + 1);
		if(ncmdline == NULL) {
			atag_error("Out of memory!");
			kpanic();
		}
		strncpy(ncmdline, cmdline, strlen(cmdline) + 1);
		// TODO: could be used for andix parameters ...
	}
	return (ncmdline);
}

void atag_dump(struct atag* startTag) {
	struct atag* atag = startTag;
	uint8_t* ptr = (uint8_t*) startTag;
	//dump_stack_trace();
	atag_debug("Reading ATAGS:");
	atag_debug("=================================");

	do {
		atag_debug("ATAG: @ 0x%x", atag);
		atag_debug("   TYPE :  %s", atag_get_type_name(atag->hdr.tag));
		atag_debug("   NUM  :  0x%x", atag->hdr.tag);
		atag_debug("   SIZE :  0x%x", atag->hdr.size);

		if (atag->hdr.tag == ATAG_CORE) {
			atag_debug("   FLAGS:  0x%x", atag->u.core.flags);
			atag_debug("   PSIZE:  0x%x", atag->u.core.pagesize);
			atag_debug("   ROOT :  0x%x", atag->u.core.rootdev);
		} else if (atag->hdr.tag == ATAG_REVISION) {
			atag_debug("   REVS :  0x%x", atag->u.revision.rev);
		} else if (atag->hdr.tag == ATAG_MEM) {
			atag_debug("   START:  0x%x", atag->u.mem.start);
			atag_debug("   SIZE :  0x%x (%d MB)", atag->u.mem.size,
					atag->u.mem.size / (1024 * 1024));
			atag_debug("   END  :  0x%x", atag->u.mem.start + atag->u.mem.size);
		} else if (atag->hdr.tag == ATAG_CMDLINE) {
			atag_debug("   CMD:  %s", atag->u.cmdline.cmdline);
		} else if (atag->hdr.tag == ATAG_INITRD2) {
			atag_debug("   START:  0x%x", atag->u.initrd2.start);
			atag_debug("   SIZE :  0x%x", atag->u.initrd2.size);
		}

		ptr = (uint8_t*) atag;

		atag = (struct atag*) (ptr + (atag->hdr.size * 4));
	} while (atag->hdr.tag != ATAG_NONE);

	atag_debug("ATAG: @ 0x%x", atag);
	atag_debug("   NAME:  %s", atag_get_type_name(atag->hdr.tag));
	atag_debug("   NUM :  0x%x", atag->hdr.tag);
	atag_debug("   SIZE:  0x%x", atag->hdr.size);
	atag = atag + atag->hdr.size;

	atag_debug("=================================");
}

struct atag* atag_get_next_mem_tag(struct atag* startTag) {
	struct atag* atag = startTag;
	uint8_t* ptr = (uint8_t*) startTag;
	if (atag->hdr.tag == ATAG_MEM) {
		ptr = (uint8_t*) atag;
		atag = (struct atag*) (ptr + (atag->hdr.size * 4));
	}

	do {
		if (atag->hdr.tag == ATAG_MEM) {
			return (atag);
		}

		ptr = (uint8_t*) atag;

		atag = (struct atag*) (ptr + (atag->hdr.size * 4));
	} while (atag->hdr.tag != ATAG_NONE);

	return (NULL );
}

struct atag* atag_get_revision_tag(struct atag* startTag) {
	struct atag* atag = startTag;
	uint8_t* ptr = (uint8_t*) startTag;
	do {
		if (atag->hdr.tag == ATAG_REVISION) {
			return (atag);
		}

		ptr = (uint8_t*) atag;

		atag = (struct atag*) (ptr + (atag->hdr.size * 4));
	} while (atag->hdr.tag != ATAG_NONE);

	return (NULL );
}

char* atag_get_type_name(uint32_t tag) {
	switch (tag) {
	case ATAG_NONE:
		return ("NONE");
	case ATAG_CORE:
		return ("CORE");
	case ATAG_MEM:
		return ("MEM");
	case ATAG_VIDEOTEXT:
		return ("VIDEOTEXT");
	case ATAG_RAMDISK:
		return ("RAMDISK");
	case ATAG_INITRD2:
		return ("INITRD2");
	case ATAG_SERIAL:
		return ("SERIAL");
	case ATAG_REVISION:
		return ("REVISION");
	case ATAG_VIDEOLFB:
		return ("VIDEOLFB");
	case ATAG_CMDLINE:
		return ("CMDLINE");
	default:
		return ("UNKOWN");
	}
}

// ===================================================================
// Internal atag functions
// ===================================================================

/**
 * ATAG params for setup functions
 */
static atag_t* params;

void atag_setup_core(uintptr_t address) {
	params = (atag_t*) address; /* Initialise parameters to start at given address */

	params->hdr.tag = ATAG_CORE; /* start with the core tag */
	params->hdr.size = tag_size(atag_core); /* size the tag */

	params->u.core.flags = 1; /* ensure read-only */
	params->u.core.pagesize = 0; /* systems pagesize (4k) */
	params->u.core.rootdev = 3; /* zero root device (typicaly overidden from commandline )*/

	params = tag_next(params); /* move pointer to next tag */
}

void atag_setup_mem(uint32_t start, uint32_t len) {
	params->hdr.tag = ATAG_MEM; /* Memory tag */
	params->hdr.size = tag_size(atag_mem); /* size tag */

	params->u.mem.start = start; /* Start of memory area (physical address) */
	params->u.mem.size = len; /* Length of area */

	params = tag_next(params); /* move pointer to next tag */
}

void atag_setup_revision() {
	atag_t* revision_tag = atag_get_revision_tag(atag_get_current());
	if (revision_tag != NULL ) {
		params->hdr.tag = revision_tag->hdr.tag;
		params->hdr.size = revision_tag->hdr.size;
		params->u.revision.rev = revision_tag->u.revision.rev;
		params = tag_next(params);
	}
}

void atag_setup_initrd(uint32_t start, uint32_t size) {
	params->hdr.tag = ATAG_INITRD2; /* Initrd2 tag */
	params->hdr.size = tag_size(atag_initrd2); /* size tag */

	params->u.initrd2.start = start; /* physical start */
	params->u.initrd2.size = size; /* compressed ramdisk size */

	params = tag_next(params); /* move pointer to next tag */
}

void atag_setup_cmdline(const char * line) {
	int linelen = strlen(line);

	if (!linelen)
		return; /* do not insert a tag for an empty commandline */

	params->hdr.tag = ATAG_CMDLINE; /* Commandline tag */
	params->hdr.size = (sizeof(struct atag_header) + linelen + 1 + 4) >> 2;

	memset(params->u.cmdline.cmdline, 0, linelen + 1 + 4);
	strncpy(params->u.cmdline.cmdline, line, linelen + 1 + 4); /* place commandline into tag */

	params = tag_next(params); /* move pointer to next tag */
}

void atag_setup_end(void) {
	params->hdr.tag = ATAG_NONE; /* Empty tag ends list */
	params->hdr.size = 0; /* zero length */
}
