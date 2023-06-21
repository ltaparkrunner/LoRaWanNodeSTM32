#ifndef TINY_JSON_EXTRA_H
#define TINY_JSON_EXTRA_H

#include <stdint.h>
#include "flash_mem.h"
#include "tiny-json.h"
#define	AD_1 0x11
#define	INP1 0x22
#define	INP3 0x33
#define	AD_4 0x44
#define	TEXT 0x55

#define datLoraNum 5
#define arrElLen		4
int32_t json_to_buffer(char* str, json_t mem[], unsigned int qty, uint8_t buff[], uint32_t len);
int32_t parse_array(const json_t* json_ptr, uint32_t* buff_ptr, int32_t* descrCnt, uint8_t buff[], uint32_t len);
int32_t parse_array2(const json_t* json_ptr, uint32_t* buff_ptr, int32_t* descrCnt, struct buffer_t* buff, uint32_t len);
int check( void );


struct field_json{
	char name[20];
	uint64_t min;
	uint64_t max;
	uint32_t bytes;
	int level;
	jsonType_t ty;
	uint32_t offset;
	uint32_t changed;
};

struct json_desc_t{
	struct field_json* descr[Json_Descript_Length];
	int32_t len;
};

//void initStack(void);
//#define truefl	0x99
//#define falsefl 0xf0

#endif /* TINY_JSON_EXTRA_H */
