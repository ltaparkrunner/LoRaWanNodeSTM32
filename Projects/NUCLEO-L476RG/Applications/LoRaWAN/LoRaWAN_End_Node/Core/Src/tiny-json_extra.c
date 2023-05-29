#include "tiny-json.h"
#include "tiny-json_extra.h"
#include <string.h>
#include "settings_json.h"

struct node{
	//char prop[70];
	const json_t* prop;
	int32_t level;
};

struct field{
	char name[20];
	uint64_t min;
	uint64_t max;
	uint32_t bytes;
	int level;
	jsonType_t ty;
};

//extern struct field json_descr[Json_Descript_Length];
extern char sets_JSON[];
struct node props[Num_Field];
int32_t props_pointer = 0;
const json_t* stack[Num_Field];			///????
int32_t stack_pointer = 0;
uint8_t buff[Buff_Len];
uint32_t stack2[Json_Descript_Length];
uint32_t stack2_pointer = 0;

struct field json_descr[Json_Descript_Length] = {
															{"LoRa_settings",0,0,0,0,JSON_OBJ},
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
																	{"period",0,0,0,2,JSON_OBJ},
																		{"sec",0,60,1,3,JSON_INTEGER},
																		{"min",0,60,1,3,JSON_INTEGER},
																		{"hour",0,24,1,3,JSON_INTEGER},
																		{"days",0,1000,2,3,JSON_INTEGER},
																		{"work",0,1000,1,3,JSON_BOOLEAN},
																{"LoRa",0,0,0,1,JSON_OBJ},
																	{"blinks",0,0,0,2,JSON_OBJ},
																		{"msec",0,100,1,3,JSON_INTEGER},
																		{"times",0,100,1,3,JSON_INTEGER},		
																	{"period",0,0,0,2,JSON_OBJ},
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

															{"LoRa_text",0,0,34,0,JSON_TEXT},
															{"LoRa_Data2",0,0,5,0,JSON_ARRAY},
															{"LoRa_Data",0,0,5,0,JSON_ARRAY},
															{"Command",0,0,1,0,JSON_BOOLEAN}
															//	\"LoRa_Data\": [\"AD1\", \"INP1\", \"INP3\", \"AD4\", \"text\"],
};

void initStack2(void)
{
	stack2_pointer = 0;
}
int32_t stack2IsEmpty()
{
	if( stack2_pointer == 0 ) return 1;
	else return 0;
}
int32_t push2(uint32_t indx)
{
	if(stack2_pointer <= Num_Field)
	{
		stack2[stack2_pointer] = indx;
		stack2_pointer++;
		return 0;
	}
	else return -1;
}
int32_t pop2(void)
{
	int32_t retvalue;
	if(stack2_pointer > 0)
	{
		retvalue = stack2[stack2_pointer];
		stack2_pointer--;
		return retvalue;
	}
	else return NULL; // or JSON_NULL
}


void initStack()
{
	stack_pointer = 0;
}

int32_t stackIsEmpty()
{
	if( stack_pointer == 0 ) return 1;
	else return 0;
}

int32_t to_list(const json_t* node, int32_t level)
{
	if(props_pointer <= Num_Field)
	{
		props[props_pointer].prop = node;
		props[props_pointer].level = level;
		props_pointer++;
		return 0;
	}
	else return -1;
	
}

int32_t push(const json_t* node)
{
	if(stack_pointer <= Num_Field)
	{
		stack[stack_pointer] = node;
		stack_pointer++;
		return 0;
	}
	else return -1;
}

const json_t* pop(void)
{
	const json_t* retvalue;
	if(stack_pointer > 0)
	{
		retvalue = (const json_t*) stack[stack_pointer - 1];
		stack_pointer--;
		return retvalue;
	}
	else return NULL; // or JSON_NULL
}

int32_t parse_array(const json_t* json_ptr, uint32_t* buff_ptr, int32_t j2)
{
// "ADC1\", \"INP1\", \"INP3\", \"ADC4\", \"TEXT\"	
	//static char curEl[100];
	const char* val = json_getValue(json_ptr);
	const char* nm = json_getPropertyValue(json_ptr, json_descr[j2].name);
	const char* nm2 = json_getPropertyValue(json_ptr, "LoRa_Data2");
	json_t const* curEl = json_getChild( json_ptr );
	const char* val2 = json_getValue(curEl);
	char pool[10];
	strncpy(pool, nm, 5);
	strncpy(pool, nm2, 5);
	strncpy(pool, val, 5);
	strncpy(pool, val2, 5);
	
	//curEl = json_getText(json_ptr);
	//curEl = objValue(json_ptr->u, pool);
	for(int i2 = 0; i2<json_descr[j2].bytes; i2++)
	{
		if(strlen(curEl->name) == arrElLen)
		{
			if(strncmp(curEl->name, "AD_1", arrElLen)) buff[(*buff_ptr)++] = AD_1;
			if(strncmp(curEl->name, "ADC4", arrElLen)) buff[(*buff_ptr)++] = ADC4;
			if(strncmp(curEl->name, "INP1", arrElLen)) buff[(*buff_ptr)++] = INP1;
			if(strncmp(curEl->name, "INP3", arrElLen)) buff[(*buff_ptr)++] = INP3;
			if(strncmp(curEl->name, "TEXT", arrElLen)) buff[(*buff_ptr)++] = TEXT;
		}
		curEl = curEl -> sibling;
	}
	return json_descr[j2].bytes;
}
static void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
///* Search a property by its name in a JSON object. */
//json_t const* json_getProperty( json_t const* obj, char const* property ) {
//    json_t const* sibling;
//    for( sibling = obj->u.c.child; sibling; sibling = sibling->sibling )
//        if ( sibling->name && !strcmp( sibling->name, property ) )
//            return sibling;
//    return 0;
//}

/* Search a property by its name in a JSON object. */
struct node* json_allProperties( json_t const* obj, char const* property ) {
    json_t const* sibling = obj->u.c.child;
	 int32_t level = 0;
	initStack();
	while(sibling && stackIsEmpty())
	{
		to_list(sibling, level);
		if(sibling->u.c.child) {
			push(sibling);
			sibling = sibling -> u.c.child;
			level++;
		}
		else if(sibling->sibling) {
			sibling = sibling->sibling;
		}
		else if(!stackIsEmpty()) {
			while(sibling -> sibling && !stackIsEmpty())
			sibling = pop();
			if( level>0 ) level--;
		}
			if(sibling == NULL) break;
	}
	return props;
}

int32_t json_to_buffer(char* str, json_t mem[], unsigned int qty)
{
//	json_t pool[ qty ];
	json_t const *json_sets = json_create(str, mem, qty);
	uint32_t buff_len = 0;
	uint32_t buff_ptr = 0;
	static uint64_t tempI;
	static uint8_t tempB;
	static const char* tempT;
	static int32_t j2 = 0;
	int32_t len = 0;
	
	for(int i = 0; i < Json_Descript_Length; i++)
	{
		buff_len += json_descr[i].bytes;
	}
	
	initStack();
	const json_t* json_ptr = json_getChild(json_sets);
	
	//for(int i = 0; i < Json_Descript_Length; i++)
	while(json_ptr != NULL  ||  !stackIsEmpty())
	{				
				len = strlen(json_descr[j2].name);
				//int strncmp(const char * /*s1*/, const char * /*s2*/, size_t /*n*/)
				int8_t rez = strncmp(json_ptr->name, json_descr[j2].name, len);
				if(rez == 0){
					switch(json_ptr->type){
					case JSON_OBJ:
						push(json_ptr); // or push(json_ptr->sibling)
						json_ptr = json_getChild(json_ptr);	
					break;
					case JSON_INTEGER:
						tempI = json_getInteger(json_ptr);
						for(int i2=0; i2<json_descr[j2].bytes; i2++)
						{
							buff[buff_ptr++] = (uint8_t)tempI & 0xff;
							tempI >>= 1;
						}
						json_ptr = json_ptr->sibling;
						break;
					case JSON_BOOLEAN:
						tempB = json_getBoolean(json_ptr);
						buff[buff_ptr++] = tempB;
						json_ptr = json_ptr->sibling;
						break;
					case JSON_TEXT:
						tempT = json_getValue(json_ptr);
						//len = len
						for(int i2=0; i2<json_descr[j2].bytes; i2++)
							buff[buff_ptr++] = tempT[i2];
						json_ptr = json_ptr->sibling;
						break;
					case JSON_ARRAY: 
						//for(int i2=0; i2<json_descr[j2].bytes; i2++)
							parse_array(json_ptr, &buff_ptr, j2);
						json_ptr = json_ptr->sibling;
						
					default: Error_Handler();
					}
				}
				else   Error_Handler();
				
				while(json_ptr == NULL && !stackIsEmpty())
				{
					json_ptr = pop();
					json_ptr = json_ptr->sibling;
				}

				if(json_ptr == NULL && stackIsEmpty()) break;
				j2++;
	}
	return buff_ptr;
}
