#include <stdint.h>
#define APP_RX_DATA_SIZE  2048
#define APP_STR_SIZE APP_RX_DATA_SIZE + APP_RX_DATA_SIZE/3
#define JSON_MESS_NUM 20

struct bufc_t{
	int32_t length;
	int32_t head_outp;
	int32_t head_json;
	int32_t head;
	int32_t tail_outp;
	int32_t tail_json;
	int32_t tail;
	uint8_t array[APP_RX_DATA_SIZE];
};
struct strc_t{
	int32_t length;
	uint8_t array[APP_STR_SIZE];
};

struct jsonmsg_t{
	int32_t length;
	int32_t ready;
	uint8_t array[APP_RX_DATA_SIZE];
};


struct parenthesis_t{
	int32_t exists;
	int32_t first_opening;
	int32_t opening;
	int32_t closing;
	int32_t last_closing;
};

struct parentharray_t{
	struct parenthesis_t parth[JSON_MESS_NUM];
	int32_t length;
	int32_t wrkng;
};

int32_t outpStr(struct bufc_t* bufc, struct strc_t* str, struct parentharray_t* pararr);
int32_t outpStr2(struct bufc_t* bufc, struct strc_t* str);

typedef enum
{
  PARSE_ZERO_LEN = 4U,

}ParseOut_StatusTypeDef;

typedef enum
{
  PARSE_JSON_OK = 5U,
	PARSE_JSON_FAIL

}ParseJson_StatusTypeDef;

uint8_t ExtractJson(void);
int32_t FetchJson(struct bufc_t* bufc, struct jsonmsg_t* jsonmsg);

uint8_t ParseJsonMessage(void);
int32_t FetchJsonSettings(struct bufc_t* bufc, struct parentharray_t* pararr, struct jsonmsg_t* jsonmsg);
