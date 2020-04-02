/**********************************************************************
*File name 		:   BcdTrans.c
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2020/3/26
*Description    :   BCD trans to Dec, or Dec tans to BCD
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*
*
*
*********************************************************************/

/**********************************************************************
*
*Include file  section
*
*********************************************************************/
#include "BcdTrans.h"

/**********************************************************************
*
*Global Table Define Section
*
*********************************************************************/


/**********************************************************************
*Name			:   uint32_t Crc32Cal( uint8_t *InBuff, uint32_t Length )
*Function       :   crc32
*Para     		:   uint8_t *InBuff   the address of the crc array
*					uint32_t Length     the crc length of array
*
*Return    		:   Crc32  the calculate result
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*
*
*
*********************************************************************/
	
/*unsigned long  BCDtoDec(const unsigned char *bcd, int length)
{
	   int i, tmp;
	     unsigned long dec = 0;
	     for(i=0; i<length; i++)
	     {
	        tmp = ((bcd[i]>>4)&0x0F)*10 + (bcd[i]&0x0F);   
	        dec += tmp * power(100, length-1-i);          
	     }
	     return dec;
}*/

/**********************************************************************
*Name			:   int8_t DectoBCD(int Dec, unsigned char *Bcd, int length)
*Function       :   DectoBCD trans
*Para     		:   int Dec,
*					unsigned char *Bcd
*                   int length
*Return    		:   
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2020/3/26  Create
*
*********************************************************************/
int8_t DectoBCD(uint16_t Dec,uint8_t * Bcd,uint8_t length)
{
	// uint8_t i;
 //    uint16_t temp;
 //    uint8_t BCDtem = 0;
	// for(i=0;i < length; i++)
	// {
	// 	temp = Dec%100;
	// 	BCDtem = ((temp/10)<<4) + ((temp%10) & 0x0F);
	// 	*Bcd = BCsDtem;
	// 	Bcd++;
	// 	Dec /= 100;
	// }
	// return 0;
}
/**********************************************************************
*Name			:   int8_t Litte2BigEnd(uint Dec, unsigned char *Bcd, int length)
*Function       :   DectoBCD trans
*Para     		:   int Dec,
*					unsigned char *Bcd
*                   int length
*Return    		:   
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2020/3/26  Create
*
*********************************************************************/
uint16_t Litte2BigEnd(uint16_t Indata)
{
	uint16_t result = 0;
	result = (((Indata & 0xFF) << 8 )|((Indata >> 8) & 0xFF));



	return  result;
}



