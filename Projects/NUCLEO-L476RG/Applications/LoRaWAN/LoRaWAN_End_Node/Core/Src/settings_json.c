#include <string.h>
#include <ctype.h>
#include "tiny-json.h"
#include "settings_json.h"

char sets_JSON[] = "{\
	\"LoRa_settings\": {\
		\"DEVEUI\": 0x111010110100202C,\
		\"APPKEY\": 0x1b6c1623865fffff,\
		\"FREQ\": 86800000,\
		\"FR\": 129374907\
	},\
	\"UART3\": true ,\
	\"LED2\": {\
		\"USB\": {\
			\"sec\": 10,\
			\"min\":  2,\
			\"hour\":  0,\
			\"days\":   0,\
			\"work\": true\
		},\
		\"battery\": {\
			\"sec\":  0,\
			\"min\":  3,\
			\"hour\":  0,\
			\"days\":  0,\
			\"work\": true \
		}\
	},\
	\"LED1\": {\
		\"USB\": {\
			\"blinks\":{\
				\"msec\": 100,\
				\"times\":  3\
			},\
			\"period\":{\
				\"sec\":  1,\
				\"min\":  2,\
				\"hour\":  0,\
				\"days\":  0,\
				\"work\": false,\
			},\
		},\
		\"LoRa\": {\
			\"blinks\":{\
				\"msec\": 200,\
				\"times\":  2\
			},\
			\"period\":{\
				\"sec\":  1,\
				\"min\":  2,\
				\"hour\":  0,\
				\"days\":   0,\
				\"work\": true \
			},\
		}\
	},\
	\"period_LoRa\": {\
		\"sec\":  1,\
		\"min\":  2,\
		\"hour\":  0,\
		\"days\":  0,\
		\"work\": true \
	},\
	\"LoRa_text\": \"Soon as early Dawn appeared, the rosy-fingered, up from his bed arose the dear son of Odysseus and put on his clothing.\",\
	\"LoRa_Data\": [{\"AD_1\":true }, {\"INP1\":false}, {\"INP3\":false}, {\"AD_4\":true }, {\"TEXT\":false}],\
	\"Command\": \"              \",\
	\"WRTN\": 153,\
	\"DevEuiValid\":true ,\
	\"AppKeyValid\":true \
}";

struct json_sets_t json_sets = {
	.p =sets_JSON, .length = 0, .array = {0}
};

//	\"LoRa_Data\": [{\"AD_1\":\"true\"}, {\"INP1\":\"false\"}],\
//, {\"INP1\":true}, {\"INP3\":f}, {\"AD_4\":f}, {\"TEXT\":f}
//struct node props[Num_Field];typedef enum {
//    JSON_OBJ, JSON_ARRAY, JSON_TEXT, JSON_BOOLEAN,
//    JSON_INTEGER, JSON_REAL, JSON_NULL
//} jsonType_t;
// \"read_settings\"


json_t pool[ Num_Field ];
json_t const *settings;// = json_create(set_JSON, pool, Num_Field);
	
	///////////////
json_t const* namefield;// = json_getProperty(settings, "LoRa_text");

	
	//////////////////
char const* namevalue;// = json_getValue(namefield);
