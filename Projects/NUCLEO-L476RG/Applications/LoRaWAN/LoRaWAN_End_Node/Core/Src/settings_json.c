#include <string.h>
#include <ctype.h>
#include "tiny-json.h"
#include "settings_json.h"

char sets_JSON[] = "{\
	\"LoRa_settings\": {\
		\"DEVEUI\": 111101011010,\
		\"APPKEY\": 1698162386,\
		\"FREQ\": 86800000,\
		\"FR\": 129374907\
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
	\"LoRa_Data\": [{\"AD_1\":true}, {\"INP1\":false}, {\"INP3\":false}, {\"AD_4\":true}, {\"TEXT\":false}],\
	\"Command\": \"read_settings\"\
}";

//	\"LoRa_Data\": [{\"AD_1\":\"true\"}, {\"INP1\":\"false\"}],\
//, {\"INP1\":true}, {\"INP3\":f}, {\"AD_4\":f}, {\"TEXT\":f}
//struct node props[Num_Field];typedef enum {
//    JSON_OBJ, JSON_ARRAY, JSON_TEXT, JSON_BOOLEAN,
//    JSON_INTEGER, JSON_REAL, JSON_NULL
//} jsonType_t;



json_t pool[ Num_Field ];
json_t const *settings;// = json_create(set_JSON, pool, Num_Field);
	
	///////////////
json_t const* namefield;// = json_getProperty(settings, "LoRa_text");

	
	//////////////////
char const* namevalue;// = json_getValue(namefield);
