/*
 * tee_storage_types.h
 *
 *  Created on: Mar 2, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_STORAGE_TYPES_H_
#define TEE_STORAGE_TYPES_H_

#include <common/tee_list.h>

/**
 * \addtogroup storage
 *	\{
 */

typedef struct {
	uint32_t attributeID;
	union {
		struct {
			void* buffer;
			size_t length;
		} ref;
		struct {
			uint32_t a, b;
		} value;
	} content;
} TEE_Attribute;

typedef struct {
	uint32_t objectType;
	uint32_t objectSize;
	uint32_t maxObjectSize;
	uint32_t objectUsage;
	uint32_t dataSize;
	uint32_t dataPosition;
	uint32_t handleFlags;
} TEE_ObjectInfo;

typedef enum {
	TEE_DATA_SEEK_SET = 0,
	TEE_DATA_SEEK_CUR,
	TEE_DATA_SEEK_END
} TEE_Whence;

struct __TEE_ObjectHandle {
	TEE_ObjectInfo info;
	union {
		struct {
			int fd;
		} persistent;
		struct {
			list* attributes;
		} transient;
	} content;
	bool initialized;
};

typedef struct __TEE_ObjectHandle* TEE_ObjectHandle;

typedef struct __TEE_ObjectEnumHandle* TEE_ObjectEnumHandle;

/* \} group */

#endif /* TEE_STORAGE_TYPES_H_ */
