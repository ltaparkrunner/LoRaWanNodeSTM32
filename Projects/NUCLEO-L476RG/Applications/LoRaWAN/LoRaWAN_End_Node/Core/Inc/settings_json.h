#include <stdint.h>
#define Num_Field 70
#define Buff_Len	512
#define Json_Descript_Length 57

#define truefl	0x99
#define falsefl 0xf0

struct json_sets_t{
	char* p;
	int32_t length;
};
uint32_t fillBuff(uint8_t* buff);
