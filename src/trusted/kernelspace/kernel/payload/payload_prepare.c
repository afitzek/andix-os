/*
 * payload_prepare.c
 *
 *  Created on: Jun 12, 2013
 *      Author: andy
 */

#include <mm/mm.h>

int32_t payload_load_data(void* vdata, void* pdest, uint32_t size) {
	uintptr_t vpayload_ptr = map_phys_mem((uintptr_t) pdest, size,
			AP_SVC_RW_USR_NO, 1, 1, 0);

	if (vpayload_ptr == NULL) {
		return (-1);
	}

	uint8_t* src = (uint8_t*) vdata;
	uint8_t* dst = (uint8_t*) vpayload_ptr;

	main_debug("Copying Payload form 0x%x to 0x%x (p 0x%x) %d bytes", src, dst,
			v_to_p(dst), size);

	for (uint32_t i = 0; i < size; i++) {
		dst[i] = src[i];
	}

	kdumpMem(dst, 128);

	unmap_memory(vpayload_ptr, size);

	return (0);
}
