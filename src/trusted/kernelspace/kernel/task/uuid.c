/**
 * @file uuid.c
 * @brief
 *  Created on: Jan 28, 2014
 * @author Florian Achleitner <florian.achleitner@student.tugraz.at>
 */

#include <task/uuid.h>
#include <common/string.h>
#include <common/stdlib.h>

void set_uuid_emtpy(TASK_UUID *uuid) {
	uuid->timeHiAndVersion = 0;
	uuid->timeMid = 0;
	uuid->timeLow = 0;
	for (int i = 0; i < sizeof(uuid->clockSeqAndNode); i++) {
		uuid->clockSeqAndNode[i] = 0;
	}
}

int uuid_parse(const char *in, TASK_UUID* uuid)
{
	char		uuid_string[40];
	int 		i;
	const char	*cp;
	char		buf[3];

	if (strlen(in) != 36)
		return (-1);
	for (i=0, cp = in; i <= 36; i++,cp++) {
		if ((i == 8) || (i == 13) || (i == 18) ||
		    (i == 23)) {
			if (*cp == '-')
				continue;
			else
				return (-1);
		}
		if (i== 36)
			if (*cp == 0)
				continue;
		if (!isxdigit(*cp))
			return (-1);
	}

	memcpy(uuid_string, in, strlen(in));

	uuid_string[8] = 0;
	uuid_string[13] = 0;
	uuid_string[18] = 0;
	uuid_string[23] = 0;

	uuid->timeLow = strtoul(in, NULL, 16);
	uuid->timeMid = strtous(in+9, NULL, 16);
	uuid->timeHiAndVersion = strtous(in+14, NULL, 16);
	//uuid->clockSeqAndNode[0] = strtous(in+19, NULL, 16);
	cp = in+19;
	buf[2] = 0;
	for (i=0; i < 8; i++) {
		buf[0] = *cp++;
		buf[1] = *cp++;
		if((*cp) == '-') {
			cp++;
		}
		uuid->clockSeqAndNode[7-i] = strtouc(buf, NULL, 16);
	}

	return (0);
}

uint8_t match_uuids(TASK_UUID *a, TASK_UUID *b) {
	if (a->timeHiAndVersion != b->timeHiAndVersion) {
		return (0);
	}
	if (a->timeMid != b->timeMid) {
		return (0);
	}
	if (a->timeLow != b->timeLow) {
		return (0);
	}
	for (int i = 0; i < sizeof(a->clockSeqAndNode); i++) {
		if (a->clockSeqAndNode[i] != b->clockSeqAndNode[i]) {
			return (0);
		}
	}

	return (1);
}

void cpy_uuid(TASK_UUID *dst, TASK_UUID *src) {
	dst->timeHiAndVersion = src->timeHiAndVersion;
	dst->timeMid = src->timeMid;
	dst->timeLow = src->timeLow;
	for (int i = 0; i < sizeof(dst->clockSeqAndNode); i++) {
		dst->clockSeqAndNode[i] = src->clockSeqAndNode[i];
	}
}

uint8_t is_uuid_empty(TASK_UUID *uuid) {

	if (uuid->timeHiAndVersion != 0) {
		return (0);
	}

	if (uuid->timeMid != 0) {
		return (0);
	}

	if (uuid->timeLow != 0) {
		return (0);
	}

	for (int i = 0; i < sizeof(uuid->clockSeqAndNode); i++) {
		if (uuid->clockSeqAndNode[i] != 0) {
			return (0);
		}
	}

	return (1);
}
