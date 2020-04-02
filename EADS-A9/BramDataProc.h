/**********************************************************************
*File name 		:   BramDataProc.c
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/5/16
*Description    :   Include the Write data to 570,Read data from 570,
*                   Write data to MAX10,Read data from MAX10
*                   Read 570 RTC information and set system time

*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/05/16  Create
*
*********************************************************************/

/**********************************************************************
*
*Multi-Include-Prevent Sart Section
*
*********************************************************************/
#ifndef _BRAMDATAPROC_H
#define _BRAMDATAPROC_H

/**********************************************************************
*
*Debug switch  section
*
*********************************************************************/

/**********************************************************************
*
*Include File Section
*
*********************************************************************/
#include "BaseDefine.h"
#include "Timer.h"
#include "BramDataInter.h"

/**********************************************************************
*
*Global Macro Define Section
*
*********************************************************************/




/**********************************************************************
*
*Global Struct Define Section
*
*********************************************************************/
typedef struct 
{	
	uint8_t PacktLength_U32;
	uint8_t PacktCMD_U8;
	uint8_t ChanNum_U8;
	uint16_t BlockNum_U16;
	COMMU_MCU_ENUM TargeMCU;
}BRAM_CMD_PACKET;



/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/


/**********************************************************************
*
*Global Prototype Declare Section
*
*********************************************************************/
int8_t BoardDataRead(BRAM_ADDRS *BramAddrs_p,uint32_t *ReadData);
int8_t BLVDSDataReadThreaFunc(uint8_t ReadNum_U8,uint8_t EADSType) ;

int8_t SetTimeFromRTC(BRAM_ADDRS *RDSpcBlk_p,BRAM_ADDRS *WRSpcBlk_p);

int8_t BramOpenMap(void);

int8_t BramCloseMap(void);
int8_t BLVDSReadInit(uint8_t EADSType) ;


/**********************************************************************
*
*Multi-Include-Prevent End Section
*
*********************************************************************/
#endif 
