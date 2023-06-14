//#include <stdint.h>
#include <string.h>
#include "parseMessage.h"
int32_t outpStr(struct bufc_t* bufc, struct strc_t* str)
{
	//char CR[] = {10, 13};
	//char *result;
	//static int32_t head;//, tail;
//	HAL_Delay(100);
//	__disable_irq();
//	head = bufc->head;
//	tail = bufc->tail;
//	__enable_irq();
	int32_t i1 = bufc->head_proc, i2 = 0;
	if ((bufc->tail - bufc->head_proc) > 0){
		while( i1 < bufc->tail && i1 < bufc->length && i2 < str->length)
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
	else if ((bufc->tail - bufc->head_proc) < 0){
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
		while( i1 < bufc->tail && i1<bufc->length && i2 < str ->length)
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
	bufc->head_proc = bufc->tail;
	return i2;
}

int32_t outpStr2(struct bufc_t* bufc, struct strc_t* str) 
{
	return 0;
}
