//#include <stdint.h>
#include <string.h>
#include "parseMessage.h"
#include "settings_json.h"
#include "tiny-json.h"
#include "tiny-json_extra.h"
#include "flash_mem.h"

extern struct json_sets_t json_sets;
struct jsonmsg_t jsonmsg;


int32_t outpStr(struct bufc_t* bufc, struct strc_t* str, struct parentharray_t* pararr)
{
//	__disable_irq();
//	__enable_irq();
	int32_t i1 = bufc->head_outp, i2 = 0;
	if ((bufc->tail_outp - bufc->head_outp) > 0){
		while( i1 < bufc->tail_outp && i1 < bufc->length && i2 < str->length)
		{
			if(pararr->parth[pararr->length].exists == 0){
//				while(i1 < bufc->tail_json)	{
					if(bufc->array[i1] == '{') {
						pararr->parth[pararr->length].exists = 1;
						pararr->parth[pararr->length].first_opening = i1-1;
						pararr->parth[pararr->length].opening++;
					}
//				}
			}
			else {
				if(bufc->array[i1] == '{') {
					pararr->parth[pararr->length].first_opening = i1 - 1;
					pararr->parth[pararr->length].opening++;
				}
				if(bufc->array[i1] == '}') {
					pararr->parth[pararr->length].last_closing = i1 - 1;
					pararr->parth[pararr->length].closing++;
				}
				if(pararr->parth[pararr->length].opening <= pararr->parth[pararr->length].closing && pararr->parth[pararr->length].exists == 1) // что здесь делать??
				{
//					jsonmsg->length = pararr->parth[pararr->length].closing - pararr->parth[pararr->length].opening;
//					strncpy((char*)jsonmsg->array, (const char*)(&bufc->array[pararr->parth[pararr->length].first_opening]), jsonmsg->length);
//					jsonmsg->ready = 1;
					pararr->length++;
					if(pararr->length > JSON_MESS_NUM-1) pararr->length = 0;
					pararr->parth[pararr->length].opening = 0;
					pararr->parth[pararr->length].closing = 0;
					pararr->parth[pararr->length].exists = 0;
					pararr->parth[pararr->length].first_opening = 0;
					pararr->parth[pararr->length].last_closing = 0;
				}
			}

			if(bufc->array[i1] != 10 && bufc->array[i1] != 13) {
				str->array[i2++] = bufc->array[i1++];
			}
			else {
				str->array[i2++] = 10;
				str->array[i2++] = 13;
				if(pararr->parth[pararr->length].exists == 1 && pararr->parth[pararr->length].opening > pararr->parth[pararr->length].closing)
					str->array[i2++] = '>';
				while(bufc->array[i1] == 10 || bufc->array[i1] == 13)
					i1++;
			}
		}
	}
	else if ((bufc->tail_outp - bufc->head_outp) < 0){
		while(i1<bufc->length && i2 < str ->length)
		{
			if(bufc->array[i1] != 10 && bufc->array[i1] != 13) {
				str->array[i2++] = bufc->array[i1++];
			}
			else {
				str->array[i2++] = 10;
				str->array[i2++] = 13;
				while(bufc->array[i1] == 10 || bufc->array[i1] == 13)
					i1++;
			}
		}
		i1 = 0;
		while( i1 < bufc->tail_outp && i1<bufc->length && i2 < str ->length)
		{
			if(bufc->array[i1] != 10 && bufc->array[i1] != 13) {
				str->array[i2++] = bufc->array[i1++];
			}
			else {
				str->array[i2++] = 10;
				str->array[i2++] = 13;
				while(bufc->array[i1] == 10 || bufc->array[i1] == 13)
					i1++;
			}
		}
	}
//	str[i2++] = 10;
//	str[i2++] = 13;
//	str[i2] = 0;

//////////////	
//	bufc->head_outp = bufc->tail_outp;
	return i2;
}


extern struct bufc_t bufc;

uint8_t ExtractJson(void)
{
	if(FetchJson(&bufc, &jsonmsg) > 0) return PARSED_JSON_OK;
	else return PARSED_JSON_FAIL;
}

int32_t FetchJson(struct bufc_t* bufc, struct jsonmsg_t* jsonmsg) 
{
	static uint8_t result = PARSED_JSON_OK;
	if(result == PARSED_JSON_OK) bufc->head_json = bufc->tail_json;
	bufc->tail_json = bufc->tail;
	int32_t i1 = bufc->head_json;
	static struct parenthesis_t parenthesis = {0,0,0,0,0};
	// at the beginning we are looking for the first parenthesis
	if(parenthesis.exists == 0){
		while(i1 < bufc->tail_json)	{
			if(bufc->array[i1++] == '{') {
				parenthesis.exists = 1;
				parenthesis.first_opening = i1 -1;
				parenthesis.opening++;
			}
		}
	}
	if(parenthesis.exists == 1){
		while(i1 < bufc->tail_json)	{
			if(bufc->array[i1++] == '{') {
				parenthesis.first_opening = i1 - 1;
				parenthesis.opening++;
			}
			if(bufc->array[i1++] == '}') {
				parenthesis.last_closing = i1 - 1;
				parenthesis.closing++;
			}
			if(parenthesis.opening == parenthesis.closing)
			{
				jsonmsg->length = parenthesis.closing - parenthesis.opening;
				strncpy((char*)jsonmsg->array, (const char*)(&bufc->array[parenthesis.first_opening]), jsonmsg->length);
				jsonmsg->ready = 1;
				break;
			}
		}
	}		
	return 0;
}

extern struct parentharray_t pararr;
const json_t* stackf2[Num_Field];			///????
int32_t stack_pointer_pm = 0;
//extern uint8_t buff[Buff_Len];
extern struct buffer_t buff;
uint8_t JsonSettingsToBuffer_wrap(void)
{
	int32_t res = JsonSettingsToBuffer(&bufc, &pararr, &jsonmsg, &buff);
	if(res > 0) return PARSED_JSON_OK;
	else if (res > 0) return NO_PARSED_JSON;
	else return PARSED_JSON_FAIL;
}
void initStack_pm()
{
	stack_pointer_pm = 0;
}

int32_t stackIsEmpty_pm()
{
	if( stack_pointer_pm == 0 ) return 1;
	else return 0;
}

int32_t push_pm(const json_t* node)
{
	if(stack_pointer_pm <= Num_Field)
	{
		stackf2[stack_pointer_pm] = node;
		stack_pointer_pm++;
		return 0;
	}
	else return -1;
}

const json_t* pop_pm(void)
{
	const json_t* retvalue;
	if(stack_pointer_pm > 0)
	{
		retvalue = (const json_t*) stackf2[stack_pointer_pm - 1];
		stack_pointer_pm--;
		return retvalue;
	}
	else return NULL; // or JSON_NULL
}

int32_t find_coincid(const json_t* json_ptr, int32_t level, struct field_json json_descr[])
{
	static int32_t nm = 0;
	int32_t ret = -1;
	int32_t ln = strlen(json_ptr->name);
	for(; nm< Json_Descript_Length; nm++)
		if(	strncmp(json_ptr->name, json_descr[nm].name, ln)) 
		{
			ret = nm;
			break;
		}
	return ret;
}

extern struct field_json json_descr[Json_Descript_Length];

int32_t JsonSettingsToBuffer(struct bufc_t* bufc, struct parentharray_t* pararr, struct jsonmsg_t* jsonmsg, struct buffer_t* buff)
{
	const char str[APP_RX_DATA_SIZE] = {0};
	static json_t pool2[Num_Field ];

	//static uint8_t buff[]
	int32_t i1;
	int32_t i2;
	static uint64_t tempI;
	static uint8_t tempB;
	static const char* tempT;
	if( (pararr->parth[pararr->wrkng].exists == 1) &&
		(pararr->parth[pararr->length].opening <= pararr->parth[pararr->length].closing))
	{
		i1 = pararr->parth[pararr->length].first_opening;
		i2 = pararr->parth[pararr->length].last_closing - i1;
		strncpy((char*)jsonmsg->array, (const char*)&bufc->array[i1], i2);
		strncpy((char*)&jsonmsg->array[i2], str, APP_RX_DATA_SIZE - i2);
		
		json_t const *json_sets = json_create((char*)jsonmsg->array, pool2, Num_Field);
		// here compare to struct field json_descr[Json_Descript_Length]
		// looking for coincidence between received json message and fields of structure json_descr
		if(json_sets != NULL) {
			//uint32_t i=0;
			int32_t level = 0;
//			int32_t slen = 0;
			const json_t* json_ptr = json_getChild(json_sets);
			initStack_pm();
			while(json_ptr != NULL  ||  !stackIsEmpty_pm())
			{
				//slen = strlen(json_ptr->name);  // это в функцию find_coincid
				int32_t num = find_coincid(json_ptr, level, json_descr);
				
				if(num > 0 && (json_descr[num].ty == json_ptr->type)) {
					switch(json_descr[num].ty){
					case JSON_OBJ:
						push_pm(json_ptr); // or push(json_ptr->sibling)
						json_ptr = json_getChild(json_ptr);	
					break;
					case JSON_INTEGER:
						tempI = json_getInteger(json_ptr);
						for(int i2=0; i2<json_descr[num].bytes; i2++)
						{
							buff->array[json_descr[num].pos + i2] = (uint8_t)tempI & 0xff;
							buff->changed[json_descr[num].pos + i2] = 1;
							tempI >>= 8;
						}
						json_ptr = json_getSibling(json_ptr);
						break;
					case JSON_BOOLEAN:
						tempB = json_getBoolean(json_ptr);
						if(tempB)	buff->array[json_descr[num].pos] = truefl;	// true
						else buff->array[json_descr[num].pos] = falsefl;
						buff->changed[json_descr[num].pos] = 1;					// false
						json_ptr = json_getSibling(json_ptr);
						break;
					case JSON_TEXT:
						tempT = json_getValue(json_ptr);
						for(int i2=0; i2<json_descr[num].bytes; i2++){
							buff->array[json_descr[num].pos + i2] = tempT[i2];
							buff->changed[json_descr[num].pos + i2] = 1;
						}
						json_ptr = json_getSibling(json_ptr);
						break;
					case JSON_ARRAY:
						{						
						//for(int i2=0; i2<json_descr[j2].bytes; i2++)
							int32_t len = Buff_Len - json_descr[num].pos;
							parse_array2(json_ptr, &json_descr[num].pos, &num, buff, len);
							json_ptr = json_getSibling(json_ptr);
						}
						break;
					case JSON_HEX:
						tempI = json_gethexInteger(json_ptr);
						for(int i2=0; i2<json_descr[num].bytes; i2++)
						{
							buff->array[json_descr[num].pos + i2] = (uint8_t)tempI & 0xff;
							buff->changed[json_descr[num].pos + i2] = 1;
							tempI >>= 8;
						}
						json_ptr = json_getSibling(json_ptr);
						break;
					default: json_ptr = json_getSibling(json_ptr); break;
					}
				}
				else json_ptr = json_getSibling(json_ptr);
				while(json_ptr == NULL && !stackIsEmpty_pm())
				{
					json_ptr = pop_pm();
					json_ptr = json_getSibling(json_ptr);
				}
				if(json_ptr == NULL && stackIsEmpty_pm()) break;
			}
			// record to flash
		}
		pararr->wrkng++;
	}
	return 0;
}

#define bufUSBlen 2048
struct bufUSB_t{
	uint8_t array[bufUSBlen];
	int32_t len;
};

//struct field_json json_descr[Json_Descript_Length]
int32_t AssembleFullJSONStringForUSB(struct json_sets_t* json_sets, struct bufUSB_t* bufUSB, struct field_json* json_descr)
{
	char* str;
	for(int32_t i1=0; i1<Json_Descript_Length; i1++)
	{
			// find the position of feature not 
			if( json_descr[i1].ty == JSON_BOOLEAN || json_descr[i1].ty == JSON_INTEGER ||
					json_descr[i1].ty == JSON_TEXT || json_descr[i1].ty == JSON_HEX){
				str = strstr((const char*)(json_sets->p), json_descr[i1].name);
			// seeking for "\":" it's better to add to upper oper-r, and miss spaces
				str = strstr((const char*)(json_sets->p), "\":");
				while(*str == ' ') str++; // the point to insert
				char* str2 = "           ";
				int32_t ln= 0;
				// if digit transform to string
				
				switch(json_descr[i1].ty){
					case JSON_BOOLEAN: //boolean to string 
						break;
					case JSON_INTEGER: // integer to string
						break;
					case JSON_HEX: // hex to string
						break;
					default: break;	
				}
				strncpy(str, str2, ln);
		}
	}
	return strlen(json_sets->p);
}
