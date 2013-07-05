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
 * atags.h
 *
 *  Created on: Aug 31, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef ATAGS_H_
#define ATAGS_H_

/**
 * \addtogroup atags
 *	\{
 */

/**
 * ATAG Type Value for none
 */
#define ATAG_NONE		0x00000000

/**
 * ATAG Type Value for core
 */
#define ATAG_CORE		0x54410001

/**
 * ATAG Type Value for mem
 */
#define ATAG_MEM		0x54410002

/**
 * ATAG Type Value for videotext
 */
#define ATAG_VIDEOTEXT	0x54410003

/**
 * ATAG Type Value for ramdisk
 */
#define ATAG_RAMDISK	0x54410004

/**
 * ATAG Type Value for initrd2
 */
#define ATAG_INITRD2	0x54420005

/**
 * ATAG Type Value for serial
 */
#define ATAG_SERIAL		0x54410006

/**
 * ATAG Type Value for revision
 */
#define ATAG_REVISION	0x54410007

/**
 * ATAG Type Value for videolfb
 */
#define ATAG_VIDEOLFB	0x54410008

/**
 * ATAG Type Value for cmdline
 */
#define ATAG_CMDLINE	0x54410009

/**
 * ATAG Header
 */
struct atag_header {
        uint32_t size; /**< length of tag in words including this header */
        uint32_t tag;  /**< ATAG Type Value */
};

/**
 * ATAG for core type
 */
struct atag_core {
		uint32_t flags;   	/**< core flags: bit 0 = read-only */
		uint32_t pagesize;  /**< systems page size (usually 4k) */
		uint32_t rootdev;   /**< root device number */
};

/**
 * ATAG for mem type
 */
struct atag_mem {
		uint32_t     size;   /**< size of the memory area */
		uint32_t     start;  /**< physical start address */
};

/**
 * ATAG for videotext type
 */
struct atag_videotext {
        uint8_t              x;           /**< width of display */
        uint8_t              y;           /**< height of display */
        uint16_t             video_page;
        uint8_t              video_mode;
        uint8_t              video_cols;
        uint16_t             video_ega_bx;
        uint8_t              video_lines;
        uint8_t              video_isvga;
        uint16_t             video_points;
};

/**
 * ATAG for command line
 */
struct atag_cmdline {
        char    cmdline[1];     /**< this is the minimum size */
};

/**
 * ATAG for ramdisk
 */
struct atag_ramdisk {
		uint32_t flags;      /**< bit 0 = load, bit 1 = prompt */
		uint32_t size;       /**< decompressed ramdisk size in _kilo_ bytes */
		uint32_t start;      /**< starting block of floppy-based RAM disk image */
};

/**
 * ATAG for initrd2
 */
struct atag_initrd2 {
		uint32_t start;      /**< physical start address */
		uint32_t size;       /**< size of compressed ramdisk image in bytes */
};

/**
 * ATAG for serialnr
 */
struct atag_serialnr {
		uint32_t low;		/**< low serial number part */
		uint32_t high;		/**< high serial number part */
};

/**
 * ATAG for revision
 */
struct atag_revision {
		uint32_t rev;		/**< revision */
};

/**
 * ATAG for videolfb
 */
struct atag_videolfb {
		uint16_t             lfb_width;
        uint16_t             lfb_height;
        uint16_t             lfb_depth;
        uint16_t             lfb_linelength;
        uint32_t             lfb_base;
        uint32_t             lfb_size;
        uint8_t              red_size;
        uint8_t              red_pos;
        uint8_t              green_size;
        uint8_t              green_pos;
        uint8_t              blue_size;
        uint8_t              blue_pos;
        uint8_t              rsvd_size;
        uint8_t              rsvd_pos;
};

/**
 * ATAG
 */
struct atag {
        struct atag_header hdr;
        union {
                struct atag_core         core;
                struct atag_mem          mem;
                struct atag_videotext    videotext;
                struct atag_ramdisk      ramdisk;
                struct atag_initrd2      initrd2;
                struct atag_serialnr     serialnr;
                struct atag_revision     revision;
                struct atag_videolfb     videolfb;
                struct atag_cmdline      cmdline;
        } u;
};

/**
 * ATAG
 */
typedef struct atag atag_t;

/**
 * Get next ATAG
 */
#define tag_next(t)     ((atag_t *)((uint32_t *)(t) + (t)->hdr.size))

/**
 * Get ATAG size
 */
#define tag_size(type)  ((sizeof(struct atag_header) + sizeof(struct type)) >> 2)

/**
 * Setup ATAGS for rich OS
 * @param start memory start address
 */
void atag_generate_nonsecure(uintptr_t start,
		uint32_t rdstart, uint32_t size);

/**
 * Print ATAGS starting at startTag
 * @param startTag memory start address
 */
void atag_dump(struct atag* startTag);

/**
 * Get ATAG Type String for given type
 * @param tag The ATAG type from atag_header
 * @return Human readable String of ATAG type
 */
char* atag_get_type_name(uint32_t tag);

/**
 * Gets the next ATAG of type mem
 * @param startTag The start tag
 * @return the next mem ATAG or NULL
 */
struct atag* atag_get_next_mem_tag(struct atag* startTag);

/**
 * Get current ATAGS
 * @return the current atags
 */
struct atag* atag_get_current();

/**
 * Copy ATAGS to kernel memory and set current atags
 * @param startTag The initial start TAG
 * @return the current atags
 */
struct atag* atag_copy(struct atag* startTag);

/**
 * Extracts cmd line from atags
 */
const char* atag_get_cmdline(struct atag* startTag);

/**
 * Generates guest cmd line for guest atags
 */
const char* atag_get_guest_cmdline(struct atag* startTag);

// ===================================================================
// Internal atag functions
// ===================================================================

/**
 * Setup core atags for current ATAG params (internal structure)
 *
 * Don't call this out of atag_generate_nonsecure!
 *
 * @param address Address for ATAG params
 */
void atag_setup_core(uintptr_t address);

/**
 * Setup a memory atag for current ATAG params (internal structure)
 *
 * Don't call this out of atag_generate_nonsecure!
 *
 * @param start Memory start address
 * @param len Memory block length
 */
void atag_setup_mem(uint32_t start, uint32_t len);

/**
 * Setup the revision atag
 */
void atag_setup_revision();

/**
 * Setup init ramdisk atag
 */
void atag_setup_initrd(uint32_t start, uint32_t size);

/**
 * Setup cmdline atag
 */
void atag_setup_cmdline(const char * line);

/**
 * Setup a none atag for finish the current ATAG params (internal structure)
 *
 * Don't call this out of atag_generate_nonsecure!
 */
void atag_setup_end(void);

/* \} group */

#endif /* ATAGS_H_ */
