#include <string.h>
#include <ctype.h>
#include "tiny-json.h"
#include "settings_json.h"

char set_JSON[] = "{\
	\"LoRa_settings\": {\
		\"DEVEUI\": 111101011010,\
		\"APPKEY\": 1698162386,\
		\"FREQ\": 86800000,\
		\"fr\": 129374907\
	},\
	\"UART3\": \"on\",\
	\"LED2\": {\
		\"USB\": {\
			\"sec\": 10,\
			\"min\": 2,\
			\"hour\": 0,\
			\"days\": 0,\
			\"work\": 1\
		},\
		\"battery\": {\
			\"sec\": 0,\
			\"min\": 3,\
			\"hour\": 0,\
			\"days\": 0,\
			\"work\": 1\
		}\
	},\
	\"LED1\": {\
		\"USB\": {\
			\"blinks\":{\
				\"msec\": 100,\
				\"times\": 3\
			},\
			\"period\":{\
				\"sec\": 1,\
				\"min\": 2,\
				\"hour\": 0,\
				\"days\": 0,\
				\"work\": 1\
			},\
		},\
		\"LoRa\": {\
			\"blinks\":{\
				\"msec\": 200,\
				\"times\": 2\
			},\
			\"period\":{\
				\"sec\": 1,\
				\"min\": 2,\
				\"hour\": 0,\
				\"days\": 0\
			},\
			\"work\": 1\
		}\
	},\
	\"period_LoRa\": {\
		\"sec\": 1,\
		\"min\": 2,\
		\"hour\": 0,\
		\"days\": 0	\
	},\
	\"LoRa_text\": \"Soon as early Dawn appeared, the rosy-fingered, up from his bed arose the dear son of Odysseus and put on his clothing.\",\
	\"LoRa_Data\": [\"AD1\", \"INP1\", \"INP3\", \"AD4\", \"text\"],\
	\"Command\": \"read_settings\"\
}";

struct field{
	char name[20];
	uint64_t min;
	uint64_t max;
	uint32_t bytes;
	int level;
	jsonType_t ty;
};

//struct node props[Num_Field];typedef enum {
//    JSON_OBJ, JSON_ARRAY, JSON_TEXT, JSON_BOOLEAN,
//    JSON_INTEGER, JSON_REAL, JSON_NULL
//} jsonType_t;

struct field JSON_arr[49] = {{"LoRa_settings",0,0,0,0,JSON_OBJ},
																{"DEVEUI",0,0xffffffffffff,8,1,JSON_INTEGER},
																{"APPKEY",0,0xffffffffff,8,1,JSON_INTEGER},
																{"FREQ",86400000,87000000,4,1,JSON_INTEGER},
																{"FR",4,12,1,1,JSON_INTEGER},
															{"UART3",0,0,1,0,JSON_BOOLEAN},
															
															{"LED2",0,0,0,0,JSON_OBJ},
																{"USB",0,0,0,1,JSON_OBJ},
																	{"sec",0,60,1,2,JSON_INTEGER},
																	{"min",0,60,1,2,JSON_INTEGER},
																	{"hour",0,24,1,2,JSON_INTEGER},
																	{"days",0,1000,2,2,JSON_INTEGER},
																	{"work",0,1000,2,2,JSON_BOOLEAN},
																{"battery",0,0,0,1,JSON_OBJ},
																	{"sec",0,60,1,2,JSON_INTEGER},
																	{"min",0,60,1,2,JSON_INTEGER},
																	{"hour",0,24,1,2,JSON_INTEGER},
																	{"days",0,1000,2,2,JSON_INTEGER},
																	{"work",0,1000,2,2,JSON_BOOLEAN},
																	
															{"LED1",0,0,0,0,JSON_OBJ},
																{"USB",0,0,0,1,JSON_OBJ},
																	{"blinks",0,0,0,2,JSON_OBJ},
																		{"msec",0,100,1,3,JSON_INTEGER},
																		{"times",0,100,1,3,JSON_INTEGER},		
																	{"battery",0,0,0,2,JSON_OBJ},
																		{"sec",0,60,1,3,JSON_INTEGER},
																		{"min",0,60,1,3,JSON_INTEGER},
																		{"hour",0,24,1,3,JSON_INTEGER},
																		{"days",0,1000,2,3,JSON_INTEGER},
																		{"work",0,1000,1,3,JSON_BOOLEAN},
																{"LoRa",0,0,0,1,JSON_OBJ},
																	{"blinks",0,0,0,2,JSON_OBJ},
																		{"msec",0,100,1,3,JSON_INTEGER},
																		{"times",0,100,1,3,JSON_INTEGER},		
																	{"battery",0,0,0,2,JSON_OBJ},
																		{"sec",0,60,1,3,JSON_INTEGER},
																		{"min",0,60,1,3,JSON_INTEGER},
																		{"hour",0,24,1,3,JSON_INTEGER},
																		{"days",0,1000,2,3,JSON_INTEGER},
																		{"work",0,1000,1,3,JSON_BOOLEAN},
																{"period_LoRa",0,0,0,1,JSON_OBJ},
																	{"sec",0,60,1,2,JSON_INTEGER},
																	{"min",0,60,1,2,JSON_INTEGER},
																	{"hour",0,24,1,2,JSON_INTEGER},
																	{"days",0,1000,2,2,JSON_INTEGER},
																	{"work",0,1000,2,2,JSON_BOOLEAN},
															{"LoRa_text",0,0,34,0,JSON_TEXT},
															{"LoRa_Data",0,0,5,0,JSON_ARRAY},
															{"Command",0,0,1,0,JSON_BOOLEAN}
															//	\"LoRa_Data\": [\"AD1\", \"INP1\", \"INP3\", \"AD4\", \"text\"],
};



json_t pool[ Num_Field ];
json_t const *settings;// = json_create(set_JSON, pool, Num_Field);
	
	///////////////
json_t const* namefield;// = json_getProperty(settings, "LoRa_text");

	
	//////////////////
char const* namevalue;// = json_getValue(namefield);
