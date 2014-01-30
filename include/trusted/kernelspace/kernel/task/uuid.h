/**
 * @file uuid.h
 * @brief
 *  Created on: Jan 28, 2014
 * @author Florian Achleitner <florian.achleitner@student.tugraz.at>
 */

#ifndef UUID_H_
#define UUID_H_

#include <common/typedefs.h>

typedef struct
{
	uint32_t timeLow;
	uint16_t timeMid;
	uint16_t timeHiAndVersion;
	uint8_t clockSeqAndNode[8];
}
TASK_UUID;


/**
 * Parse UUID from in to uuid
 */
int uuid_parse(const char *in, TASK_UUID* uuid);

/**
 * Sets uuid empty
 */
void set_uuid_emtpy(TASK_UUID *uuid);
/**
 * Copies UUID
 */
void cpy_uuid(TASK_UUID *dst, TASK_UUID *src);

/**
 * Checks if uuid is empty
 */
uint8_t is_uuid_empty(TASK_UUID *uuid);

/**
 * Match uuids
 */
uint8_t match_uuids(TASK_UUID *a, TASK_UUID *b);


#endif /* UUID_H_ */
