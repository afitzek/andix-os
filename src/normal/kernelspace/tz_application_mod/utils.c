/*
 * utils.c
 *
 *  Created on: Jan 8, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tz_application_mod/andixtee.h>

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
/*int mmap_comm_mem(struct file *filp, struct vm_area_struct *vma, TZ_TEE_SPACE* com)
{
        int ret;
        long length = vma->vm_end - vma->vm_start;

        long max_len = sizeof(TZ_TEE_SPACE) + 0x1000;
        long pgsize = 0x1000;

        int pgcount = max_len / pgsize;

        while(pgcount * pgsize < max_len) {
        	pgcount++;
        }


        // check length - do not allow larger mappings than the number of
        //   pages allocated
        if (length > (pgcount * pgsize)) {
        	printk(KERN_WARNING "Failed to map comm memory len is %d should be max %d",
        			(int)length, (int)(pgcount * pgsize));
        	return -EIO;
        }

        printk(KERN_INFO "Mapping: size %lx\n", (pgcount * pgsize));

        //vma->vm_flags |= VM_RESERVED;

        vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

        // map the whole physically contiguous area in one piece
        if ((ret = remap_pfn_range(vma,
                                   vma->vm_start,
                                   virt_to_phys((void *)com) >> PAGE_SHIFT,
                                   (pgcount * pgsize),
                                   vma->vm_page_prot)) < 0) {
        	printk(KERN_ERR "remap_pfn_range failed with: %d", ret);
            return (ret);
        }

        printk(KERN_INFO "Simple VMA open, virt %lx, phys %lx\n",
        		vma->vm_start, vma->vm_pgoff << PAGE_SHIFT);
        return (0);
}*/
