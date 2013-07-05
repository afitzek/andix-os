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
 * utils.c
 *
 *  Created on: Jan 8, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tz_service_mod/andixtz.h>

void* v_to_p(void* ptr) {
	uint32_t addr;
	//vmm_debug("Translating v (0x%x)", ptr);
	__asm__ __volatile__(
			"MCR p15, 0, %1, c7, c8, 1\n"
			"MRC p15, 0, %0, c7, c4, 0":
			"=r" (addr): "r" (ptr):"memory", "cc");

	if (addr < 0xFF) {
		return (NULL);
	}

	addr = (addr & 0xFFFFF000) + ((uint32_t) ptr & 0xFFF);

	return ((void*) addr);
}



int ipow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return (result);
}

// helper function, mmap's the kmalloc'd area which is physically contiguous
int mmap_comm_mem(struct file *filp, struct vm_area_struct *vma, TZ_CTLR_SPACE* com)
{
        int ret;
        long length = vma->vm_end - vma->vm_start;

        long max_len = sizeof(TZ_CTLR_SPACE) + 0x1000;
        long pgsize = 0x1000;

        int pgcount = max_len / pgsize;

        while(pgcount * pgsize < max_len) {
        	pgcount++;
        }


        /* check length - do not allow larger mappings than the number of
           pages allocated */
        if (length > (pgcount * pgsize)) {
        	printk(KERN_WARNING "Failed to map comm memory len is %d should be max %d",
        			(int)length, (int)(pgcount * pgsize));
        	return -EIO;
        }

        printk(KERN_INFO "Mapping: size %lx\n", (pgcount * pgsize));

        //vma->vm_flags |= VM_RESERVED;

        vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

        /* map the whole physically contiguous area in one piece */
        if ((ret = remap_pfn_range(vma,
                                   vma->vm_start,
                                   virt_to_phys((void *)com) >> PAGE_SHIFT,
                                   (pgcount * pgsize),
                                   vma->vm_page_prot)) < 0) {
        	printk(KERN_ERR "remap_pfn_range failed with: %d", ret);
            return (ret);
        }

        printk(KERN_INFO "Simple VMA open, virt %lx, phys %lx\n", vma->vm_start, vma->vm_pgoff << PAGE_SHIFT);
        return (0);
}
