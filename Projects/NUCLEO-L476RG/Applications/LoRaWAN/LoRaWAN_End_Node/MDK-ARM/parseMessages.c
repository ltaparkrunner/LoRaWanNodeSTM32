//#include <stdint.h>
#include <string.h>
#include "parseMessage.h"

extern struct json_sets_t json_sets;

int32_t outpStr(struct bufc_t* bufc, struct strc_t* str)
{
//	__disable_irq();
//	__enable_irq();
	int32_t i1 = bufc->head_proc, i2 = 0;
	if ((bufc->tail_proc - bufc->head_proc) > 0){
		while( i1 < bufc->tail_proc && i1 < bufc->length && i2 < str->length)
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
	else if ((bufc->tail_proc - bufc->head_proc) < 0){
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
		while( i1 < bufc->tail_proc && i1<bufc->length && i2 < str ->length)
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
//	bufc->head_proc = bufc->tail_proc;
	return i2;
}

int32_t outpStr2(struct bufc_t* bufc, struct strc_t* str) 
{
	
	return 0;
}
