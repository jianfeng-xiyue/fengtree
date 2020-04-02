/**********************************************************************
*File name      :   ModifConfig.h
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/02/25
*Description    :   Read and modify content value of config.ini file
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/25  Create
*
*
*********************************************************************/

/**********************************************************************
*
*Multi-Include-Prevent Sart Section
*
*********************************************************************/
#ifndef _XML_H
#define _XML_H

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
/**********************************************************************
*
*Global Macro Define Section
*
*********************************************************************/
#define ZONE 50

#define SYS_CFG 0
#define RECORD_CFG 1

#define SYS_CNT 224

#define RECORD_CNT 111

#define RCD_CFG_ZOGPEI_COACH  "/tffs0/Device_Record_CFG_ZongPei.xml" //"/home/feng/Desktop/xilinx_code"

#define RCD_CFG_COACH "/tffs0/Device_Record_CFG.xml"

//the xml line Number
#define RrdCfgFileLineNum 1
#define EventRecTypeId 20
#define EventRecTypeName 21
#define EventRecPath 22
#define EventRecFileHead 23
#define EventRecMaxFileNum  24
#define EventRecTotalChn 25
#define EventRecChnNum 26
#define EventRecChnBitMask1 27
#define EventRecChnBitMask2 28
#define EventRecChnBitMask3 29
#define EventRecChnBitMask4 30
#define EventRecChnBitMask5 31
#define EventRecChnBitMask6 32
#define EventRecInterval 33
#define EventRecBeforeNum 34
#define EventRecAfterNum 35
#define EventRecToTalNum 36

#define FaultRecTypeIdx 37
#define FaultRecTypeName 38
#define FaultRecPath 39
#define FaultRecFileHead 40
#define FaultRecMaxFileNum 41
#define FaultRecTotalChn 42
#define FaultRecChnNum 43
#define FaultRecChnBitMask1 44
#define FaultRecChnBitMask2 45
#define FaultRecChnBitMask3 46
#define FaultRecChnBitMask4 47
#define FaultRecChnBitMask5 48
#define FaultRecChnBitMask6 49
#define FaultRecInterval 50
#define FaultRecBeforeNum 51
#define FaultRecAfterNum 52
#define FaultRecToTalNum 53

#define FastRecTypeId 54
#define FastRecTypeName 55
#define FastRecPath 56
#define FastRecFileHead 57
#define FastRecMaxFileNum 58
#define FastRecTotalChn  59
#define FastRecChnNum 60
#define FastRecChnBitMask1 61
#define FastRecChnBitMask2 62
#define FastRecChnBitMask3 63
#define FastRecChnBitMask4 64
#define FastRecChnBitMask5 65
#define FastRecChnBitMask6 66
#define FastRecInterval  67
#define FastRecBeforeNum 68
#define FastRecAfterNum 69
#define FastRecToTalNum 70

#define RealRecTypeId 71
#define RealRecTypeName 72
#define RealRecPath 73
#define RealRecFileHead 74
#define RealRecMaxFileNum 75
#define RealRecTotalChn 76
#define RealRecChnNum 77
#define RealRecChnBitMask1 78
#define RealRecChnBitMask2 79
#define RealRecChnBitMask3 80
#define RealRecChnBitMask4 81
#define RealRecChnBitMask5 82
#define RealRecChnBitMask6 83
#define RealRecInterval 84
#define RealRecBeforeNum 85
#define RealRecAfterNum 86
#define RealRecToTalNum 87
/**********************************************************************
*
*Global Struct Define Section
*
*********************************************************************/

typedef struct 
{

	uint8_t RecTypeIdx;
	uint8_t RecPath[30];
	uint8_t RecFileHead[30];
	uint8_t RecTotalChn_U8;
	uint8_t RecChnNum_U8;
	uint16_t RecMaxFileNum_U16;
	uint16_t RecChnBitMask1;
	uint16_t RecChnBitMask2;
	uint16_t RecChnBitMask3;
	uint16_t RecChnBitMask4;
	uint16_t RecChnBitMask5;
	uint16_t RecChnBitMask6;
    uint16_t RecInterval;
    uint16_t RecBeforeNum;
    uint16_t RecAfterNum;
    uint16_t RecToTalNum;

}RECORD_TYPE_CFG;

typedef struct 
{
    
	RECORD_TYPE_CFG  Rec_Event_ST;
	RECORD_TYPE_CFG  Rec_Fault_ST;
	RECORD_TYPE_CFG  Rec_Fast_ST;
	RECORD_TYPE_CFG  Rec_Real_ST;
	
}RECORD_XML;

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
char * xml_to_mem(const char *xml_file, int type);
/**********************************************************************
*
*Multi-Include-Prevent End Section
*
*********************************************************************/
#endif 





