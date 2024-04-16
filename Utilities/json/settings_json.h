#ifndef SETTINGS_JSON_H
#define SETTINGS_JSON_H

#include <stdint.h>
#define Num_Field 70
#define Buff_Len	512
#define Json_Descript_Length 58

#define truefl	0x99
#define falsefl 0xf0
#define SETTINGS_MAX_LEN 0x600
struct json_sets_t{
	char* p;
	char array[SETTINGS_MAX_LEN];
	char array2[SETTINGS_MAX_LEN];
	int32_t length;
	int32_t outplen;
};
uint32_t fillBuff(uint8_t* buff);

#endif /* SETTINGS_JSON_H */
