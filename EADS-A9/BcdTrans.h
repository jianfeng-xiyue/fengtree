/**********************************************************************
*File name 		:   BCDTrans.h
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/1/29
*Description    :   crc32 calculate
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
*Multi-Include-Prevent Sart Section
*
*********************************************************************/
#ifndef _BCDTRANS_H
#define _BCDTRANS_H


/**********************************************************************
*
*Include File Section
*
*********************************************************************/
#include <stdint.h>
#include <string.h>
/**********************************************************************
*
*Global Prototype Declare Section
*
*********************************************************************/



uint16_t Litte2BigEnd(uint16_t Indata);

int8_t DectoBCD(uint16_t Dec,uint8_t * Bcd,uint8_t length);
/**********************************************************************
*
*Multi-Include-Prevent End Section
*
*********************************************************************/
#endif 

