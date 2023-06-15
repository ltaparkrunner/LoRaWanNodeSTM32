//#include <stdint.h>
#include <string.h>
#include "parseMessage.h"
#include "settings_json.h"
#include "tiny-json.h"

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
	if(FetchJson(&bufc, &jsonmsg) > 0) return PARSE_JSON_OK;
	else return PARSE_JSON_FAIL;
}

int32_t FetchJson(struct bufc_t* bufc, struct jsonmsg_t* jsonmsg) 
{
	static uint8_t result = PARSE_JSON_OK;
	if(result == PARSE_JSON_OK) bufc->head_json = bufc->tail_json;
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

uint8_t ParseJsonMessage(void)
{
	if(FetchJsonSettings(&bufc, &pararr, &jsonmsg) > 0) return PARSE_JSON_OK;
	else return PARSE_JSON_FAIL;
}

int32_t FetchJsonSettings(struct bufc_t* bufc, struct parentharray_t* pararr, struct jsonmsg_t* jsonmsg)
{
	const char str[APP_RX_DATA_SIZE] = {0};
	static json_t pool2[Num_Field ];
	static uint8_t buff2[Buff_Len];
	int32_t i1;
	int32_t i2;
	if( (pararr->parth[pararr->wrkng].exists == 1) &&
		(pararr->parth[pararr->length].opening <= pararr->parth[pararr->length].closing))
	{
		i1 = pararr->parth[pararr->length].first_opening;
		i2 = pararr->parth[pararr->length].last_closing - i1;
		strncpy((char*)jsonmsg->array, (const char*)&bufc->array[i1], i2);
		strncpy((char*)&jsonmsg->array[i2], str, APP_RX_DATA_SIZE - i2);
		
		json_t const *settings = json_create((char*)jsonmsg->array, pool2, Num_Field);
		pararr->wrkng++;
	}
	return 0;
}
