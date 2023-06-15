#include <stdint.h>
#define APP_RX_DATA_SIZE  2048
#define APP_STR_SIZE APP_RX_DATA_SIZE + APP_RX_DATA_SIZE/3

struct bufc_t{
	int32_t length;
	int32_t head_proc;
	int32_t head;
	int32_t tail_proc;
	int32_t tail;
	uint8_t array[APP_RX_DATA_SIZE];
};
struct strc_t{
	int32_t length;
	uint8_t array[APP_STR_SIZE];
};
int32_t outpStr(struct bufc_t* bufc, struct strc_t* str);
int32_t outpStr2(struct bufc_t* bufc, struct strc_t* str);

typedef enum
{
  PARSE_ZERO_LEN = 4U,

} parse_StatusTypeDef;
