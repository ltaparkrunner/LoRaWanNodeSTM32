#include <stdint.h>
#define APP_RX_DATA_SIZE  2048
#define APP_STR_SIZE APP_RX_DATA_SIZE + APP_RX_DATA_SIZE/3

struct bufc_t{
	uint8_t array[APP_STR_SIZE];
	uint32_t length;
	uint32_t head;
	uint32_t tail;
};
int32_t outpStr(struct bufc_t bufc, uint8_t* str, uint32_t lenbuf, uint32_t lenstr);
