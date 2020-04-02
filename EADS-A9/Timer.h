/**********************************************************************
*File name 		:   Timer.h
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/1/29
*Description    :   System timer set.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/5/16  Create
*
*
*
*********************************************************************/


/**********************************************************************
*
*Multi-Include-Prevent Sart Section
*
*********************************************************************/
#ifndef _TIMER_H
#define _TIMER_H


/**********************************************************************
*
*Include File Section
*
*********************************************************************/
#include <time.h>  
#include <sys/time.h> // 包含setitimer()函数

#include "BaseDefine.h"


/**********************************************************************
*
*Global Macro Define Section
*
*********************************************************************/
#define SYSTIME_LENGTH 16

/**********************************************************************
*
*Global Struct Define Section
*
*********************************************************************/

typedef struct 
{

    uint8_t Year_U8[2];
	uint8_t Month_U8;
	uint8_t Day_U8;
	uint8_t Hour_U8; 
	uint8_t Minute_U8;
	uint8_t Second_U8; /*8 byte*/
	uint8_t Resevr_U8;

}SYSTEM_TIME;


/**********************************************************************
*
*Global Prototype Declare Section
*
*********************************************************************/
int8_t set_timer(uint32_t second,uint32_t usecond);
int8_t SetSystemTime(SYSTEM_TIME *DatStr);
int threadDelay(const long lTimeSec, const long lTimeUSec);
/**********************************************************************
*
*Multi-Include-Prevent End Section
*
*********************************************************************/
#endif 