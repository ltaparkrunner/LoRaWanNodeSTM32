#include "tiny-json.h"
#include "tiny-json_extra.h"
#include <string.h>
#include "settings_json.h"
void Error_Handler(void);
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
	uint32_t pos;
	uint32_t changed;
};

//extern struct field json_descr[Json_Descript_Length];
extern char sets_JSON[];
struct node props[Num_Field];
int32_t props_pointer = 0;
const json_t* stackf[Num_Field];			///????
int32_t stack_pointer = 0;
//uint8_t buff[Buff_Len];
uint32_t stack2[Json_Descript_Length];
uint32_t stack2_pointer = 0;

struct field json_descr[Json_Descript_Length] = {
															{"LoRa_settings",0,0,0,0,JSON_OBJ,0,0},
																{"DEVEUI",0,0xffffffffffff,8,1,JSON_INTEGER,0,0},
																{"APPKEY",0,0xffffffffff,8,1,JSON_INTEGER,8,0},
																{"FREQ",86400000,87000000,4,1,JSON_INTEGER,16,0},
																{"FR",4,12,1,1,JSON_INTEGER,20,0},
															{"UART3",0,0,1,0,JSON_BOOLEAN,21,0},

															{"LED2",0,0,0,0,JSON_OBJ,22,0},
																{"USB",0,0,0,1,JSON_OBJ,22, 0},
																	{"sec",0,60,1,2,JSON_INTEGER,22,0},
																	{"min",0,60,1,2,JSON_INTEGER,23,0},
																	{"hour",0,24,1,2,JSON_INTEGER,24,0},
																	{"days",0,1000,2,2,JSON_INTEGER,25,0},
																	{"work",0,1000,1,2,JSON_BOOLEAN,27,0},
																{"battery",0,0,0,1,JSON_OBJ,28,0},
																	{"sec",0,60,1,2,JSON_INTEGER,28,0},
																	{"min",0,60,1,2,JSON_INTEGER,29,0},
																	{"hour",0,24,1,2,JSON_INTEGER,30,0},
																	{"days",0,1000,2,2,JSON_INTEGER,31,0},
																	{"work",0,1000,1,2,JSON_BOOLEAN,33,0},
																	
															{"LED1",0,0,0,0,JSON_OBJ,34,0},
																{"USB",0,0,0,1,JSON_OBJ,34,0},
																	{"blinks",0,0,0,2,JSON_OBJ,34,0},
																		{"msec",0,100,1,3,JSON_INTEGER,34,0},
																		{"times",0,100,1,3,JSON_INTEGER,35,0},		
																	{"period",0,0,0,2,JSON_OBJ,36,0},
																		{"sec",0,60,1,3,JSON_INTEGER,36,0},
																		{"min",0,60,1,3,JSON_INTEGER,37,0},
																		{"hour",0,24,1,3,JSON_INTEGER,38,0},
																		{"days",0,1000,2,3,JSON_INTEGER,39,0},
																		{"work",0,1000,1,3,JSON_BOOLEAN,41,0},
																{"LoRa",0,0,0,1,JSON_OBJ,42,0},
																	{"blinks",0,0,0,2,JSON_OBJ,42,0},
																		{"msec",0,100,1,3,JSON_INTEGER,42,0},
																		{"times",0,100,1,3,JSON_INTEGER,43,0},		
																	{"period",0,0,0,2,JSON_OBJ,44,0},
																		{"sec",0,60,1,3,JSON_INTEGER,44,0},
																		{"min",0,60,1,3,JSON_INTEGER,45,0},
																		{"hour",0,24,1,3,JSON_INTEGER,46,0},
																		{"days",0,1000,2,3,JSON_INTEGER,47,0},
																		{"work",0,1000,1,3,JSON_BOOLEAN,49,0},
																	
																{"period_LoRa",0,0,0,0,JSON_OBJ,50,0},
																	{"sec",0,60,1,1,JSON_INTEGER,50,0},
																	{"min",0,60,1,1,JSON_INTEGER,51,0},
																	{"hour",0,24,1,1,JSON_INTEGER,52,0},
																	{"days",0,1000,2,1,JSON_INTEGER,53,0},
																	{"work",0,1000,1,3,JSON_BOOLEAN,55,0},

															{"LoRa_text",0,0,34,0,JSON_TEXT,56,0},
															{"LoRa_Data",0,0,5,0,JSON_ARRAY,90,0},
																{"AD_1", 0,0,1,1,JSON_BOOLEAN,90,0},
																{"INP1", 0,0,1,1,JSON_BOOLEAN,91,0},
																{"INP3", 0,0,1,1,JSON_BOOLEAN,92,0},
																{"AD_4", 0,0,1,1,JSON_BOOLEAN,93,0},
																{"TEXT", 0,0,1,1,JSON_BOOLEAN,94,0},
															{"Command",0,0,1,0,JSON_BOOLEAN,95,0},
															{"WRTN",0,0x99,1,0,JSON_INTEGER,96,0},
															{"DevEuiVal",0,0,1,0,JSON_BOOLEAN,97,0},
															{"AppKeyValid",0,0,1,0,JSON_BOOLEAN,98,0}
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
		stackf[stack_pointer] = node;
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
		retvalue = (const json_t*) stackf[stack_pointer - 1];
		stack_pointer--;
		return retvalue;
	}
	else return NULL; // or JSON_NULL
}

const uint8_t datLora[datLoraNum] = {AD_1, INP1, INP3, AD_4, TEXT};
int32_t parse_array(const json_t* json_ptr, uint32_t* buff_ptr, int32_t* descrCnt, uint8_t buff[], uint32_t len)
{
	const char* Elval;
	uint32_t j3 = 0;
	for(json_t const* curEl = json_getChild( json_ptr );
	curEl; curEl = json_getSibling( curEl ))
	{
		Elval = json_getPropertyValue(curEl, json_descr[++(*descrCnt)].name);
		if(Elval[0] == 't') buff[(*buff_ptr)++] = datLora[j3++];
		else {
			buff[(*buff_ptr)++] = 0;
			j3++;
		}
	}
	return j3;
}

//static void Error_Handler(void)
//{
//  /* USER CODE BEGIN Error_Handler_Debug */
//  /* User can add his own implementation to report the HAL error return state */
//  __disable_irq();
//  while (1)
//  {
//  }
//  /* USER CODE END Error_Handler_Debug */
//}
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

int32_t json_to_buffer(char* str, json_t mem[], unsigned int qty, uint8_t buff[], uint32_t len)
{
//	json_t pool[ qty ];
	json_t const *json_sets = json_create(str, mem, qty);
	uint32_t buff_len = 0;
	uint32_t buff_ptr = 0;
	static uint64_t tempI;
	static uint8_t tempB;
	static const char* tempT;
	int32_t j2 = 0;
	int32_t slen = 0;
	
	for(int i = 0; i < Json_Descript_Length; i++)
	{
		buff_len += json_descr[i].bytes;
	}
	
	initStack();
	const json_t* json_ptr = json_getChild(json_sets);
	
	//for(int i = 0; i < Json_Descript_Length; i++)
	while(json_ptr != NULL  ||  !stackIsEmpty())
	{				
				slen = strlen(json_descr[j2].name);
				//int strncmp(const char * /*s1*/, const char * /*s2*/, size_t /*n*/)
				int8_t rez = strncmp(json_ptr->name, json_descr[j2].name, slen);
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
							tempI >>= 8;
						}
						json_ptr = json_ptr->sibling;
						break;
					case JSON_BOOLEAN:
						tempB = json_getBoolean(json_ptr);
						if(tempB)	buff[buff_ptr++] = truefl;	// true
						else buff[buff_ptr++] = falsefl;			// false
						json_ptr = json_ptr->sibling;
						break;
					case JSON_TEXT:
						tempT = json_getValue(json_ptr);
						for(int i2=0; i2<json_descr[j2].bytes; i2++)
							buff[buff_ptr++] = tempT[i2];
						json_ptr = json_ptr->sibling;
						break;
					case JSON_ARRAY: 
						//for(int i2=0; i2<json_descr[j2].bytes; i2++)
							parse_array(json_ptr, &buff_ptr, &j2, buff, len);
						json_ptr = json_ptr->sibling;
						break;
					case JSON_HEX:
						tempI = json_gethexInteger(json_ptr);
						for(int i2=0; i2<json_descr[j2].bytes; i2++)
						{
							buff[buff_ptr++] = (uint8_t)tempI & 0xff;
							tempI >>= 8;
						}
						json_ptr = json_ptr->sibling;
						break;
					default: Error_Handler(); break;
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
	return buff_ptr+1;
}
