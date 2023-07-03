#ifndef PARSE_MESSAGE_H
#define PARSE_MESSAGE_H

#include <stdint.h>
#include "flash_mem.h"
#include "tiny-json.h"
#include "tiny-json_extra.h"
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
  PARSED_JSON_OK = 10U,
	NO_PARSED_JSON,
	PARSED_JSON_FAIL
}ParsedJson_StatusTypeDef;

typedef enum
{ 
	NO_CMD = 60U,
	CMD_READ_SETTINGS = 61U,
	CMD_READ_SETTINGS_RET = 0xC00
}cmd_TypeDef;
uint8_t ExtractJson(void);
int32_t FetchJson(struct bufc_t* bufc, struct jsonmsg_t* jsonmsg);


//#define bufUSBlen 2048
//struct bufUSB_t{
//	uint8_t array[bufUSBlen];
//	int32_t len;
//};


//uint8_t JsonSettingsToBuffer_wrap(void);
uint8_t JsonSettingsToBuffer_wrap(uint8_t* cmd);
int32_t JsonSettingsToBuffer(struct bufc_t* bufc, struct parentharray_t* pararr, struct jsonmsg_t* jsonmsg, struct buffer_t* buff);
//int32_t AssembleFullJSONStringForUSB(struct json_sets_t* json_sets, struct bufUSB_t* bufUSB, struct field_json* json_descr);
int32_t AssembleFullJSONStringForUSB(struct json_sets_t* json_sets, /*struct bufUSB_t* bufUSB,*/ struct field_json json_descr[]);
//int32_t AssembleFullJSONStringForUSB(struct json_sets_t* json_sets, /*struct bufUSB_t* bufUSB,*/ struct json_desc_t* desc_d);
int32_t  FormOutpJson(struct json_sets_t* json_sets);
uint64_t json_getLow128hexInteger(json_t const* property);
uint64_t json_getHigh128hexInteger(json_t const* property);
#endif /* PARSE_MESSAGE_H */
