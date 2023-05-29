//enum arrLR_t{
//	ADC1,
//	INP1,
//	INP3,
//	ADC4,
//	TEXT
//};

#define	AD_1 0x11
#define	INP1 0x22
#define	INP3 0x33
#define	ADC4 0x44
#define	TEXT 0x55

#define arrElLen		4
int32_t json_to_buffer(char* str, json_t mem[], unsigned int qty);
