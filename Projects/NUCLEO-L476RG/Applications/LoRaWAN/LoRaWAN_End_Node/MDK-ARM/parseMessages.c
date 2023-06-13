//#include <stdint.h>
//#include <string.h>
#include "parseMessage.h"

int32_t outpStr(struct bufc_t* bufc, uint8_t* str, uint32_t lenbuf, uint32_t lenstr)
{
	char CR[] = {10, 13};
	char *result;
	int32_t i1 = bufc->head, i2 = 0;
	while( i1 < lenbuf && i2<lenstr && bufc->array[i1] != 0)
	{
		if(bufc->array[i1] != 10 && rxBuf[i1] != 13) {
			str[i2++] = rxBuf[i1++];
		}
		else {
			str[i2++] = 10;
			str[i2++] = 13;
			while(rxBuf[i1] == 10 || rxBuf[i1] == 13)
				i1++;
		}
	}
	str[i2++] = 10;
	str[i2++] = 13;
	str[i2] = 0;
	return i2;
}
