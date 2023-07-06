#include <stdint.h>
#include "flash_mem.h"
#include "tiny-json_extra.h"
#include "realise_settings.h"

extern struct buffer_t buff;
extern struct field_json json_descr[];
#define probes_sett_head		48
#define probes_sett_tail		53



int32_t ChangeProbesSettings(void)
{
	uint32_t addr_w;
	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	uint32_t offset = json_descr[probes_sett_head].offset;
	uint32_t bf_len = json_descr[probes_sett_tail].offset + json_descr[probes_sett_tail].bytes;
	
	static uint8_t temp_buff[10];
	
	//	if(readflash(addr_r, (uint8_t*)temp_buff, FlashBuffLen) > 0) {
	if(readflash_8b(addr_r+offset, temp_buff, bf_len) > 0) {
		for(int32_t i1 = 0; i1 < bf_len; i1++)
			if(temp_buff[i1] == truefl) {
				switch(i1)
				{
					case 0:		//	"AD_1"
					{
						
						
					}
					break;
					case 1:		//	"AD_2"
					{
						
					}
					break;
					case 2:		//	"INP1"
					{
					}
					break;
					case 3:		//	"INP2"
					{
					}
					break;
					case 4:		//	"TEXT"
					default:
					{
					}
					break;
				}
				break;
			}
	}
	return 0;
}
