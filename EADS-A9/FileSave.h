/**********************************************************************
*File name 		:   FileSave.h
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/1/29
*Description    :   Include file creat,assign value to FRAME struct,transfer char to ASCII and write file functions.
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
#ifndef _FILESAVE_H
#define _FILESAVE_H

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
#include <sys/vfs.h>  
#include <dirent.h>
#include "ModifConfig.h"
#include "Crc.h"
#include "ADCDataProc.h"
#include "xml.h"
/**********************************************************************
*
*Global Macro Define Section
*
*********************************************************************/
/*Get the free size of the hard disk path*/
#define HARD_DISK_PATH "/mnt/"


//20200328
#define FILE_LRCH_NUM 17 //for LR save

#define RESER_SPACE 1024 // reserve space for FIFO

#define  EVENT_SAVE_NUM 50000 //* 2ms = 200s

#define EVENT_MAX_NUM 60000

#define POWOFF_SAVE_NUM 10000

#define DIGITAL_NUM_WORD 8  //word //0 digital signal tube = 0 digital signals  
//#define ANALOG_NUM_WORD 80   //word  //80 analog signal tube = 80 analog signals=160 uint8
//#define OPERAT_NUM_WORD 40
/**********************************************************************
*
*Global Struct Define Section
*
*********************************************************************/
typedef struct
{
	uint16_t RecEnDianType_U16;
	uint16_t RecHeadSize_U16;
	uint16_t RecHeadVer_U16;
	uint16_t RecordType_U16;
	uint16_t ProjectNo_U16;;
	uint16_t DCUType_U16;
	uint16_t VehicleNo_U16;
	uint16_t DCUNO_U16;
	uint16_t RecordTotalChnNum_U16;
	uint16_t RecordChnNum_U16;
	uint16_t RecordInterval_U16;
	uint16_t ChnMask1_U16;
	uint16_t ChnMask2_U16;
	uint16_t ChnMask3_U16;
	uint16_t ChnMask4_U16;
	uint16_t ChnMask5_U16;
	uint16_t ChnMask6_U16;
	uint16_t RecordBefore_U16;
	uint16_t RecordAfter_U16;
	uint16_t RecordTotal_U16;
	uint8_t RecordTime_MT; //uint8_t RecordTime_MT; uint8_t RecordTime_YY
	uint8_t RecordTime_YY;
	uint8_t RecordTime_HH;//uint8_t RecordTime_HH uint8_t RecordTime_DD
	uint8_t RecordTime_DD;
	uint8_t RecordTime_SS;//uint8_t RecordTime_SS uint8_t RecordTime_MN
	uint8_t RecordTime_MN;
	uint16_t EnvData_U16[16];
	uint16_t FaultCode_U16[16];
	uint16_t HavePulse_U16;
	uint16_t RecordTotalChnNumPulse_U16;
	uint16_t RecordChnNumPulse_U16;
	uint16_t RecordIntervalPulse_U16;
	uint16_t PulseChnMask1_U16;
	uint16_t PulseChnMask2_U16;
	uint16_t PulseChnMask3_U16;
	uint16_t PulseChnMask4_U16;
	uint16_t PulseChnMask5_U16;
	uint16_t PulseChnMask6_U16;

}DRIVE_FILE_TOP;

typedef struct
{
  	uint16_t DriveDigital_U16[DIGITAL_NUM_WORD]; //max 16 word  now 8 Word
  	uint16_t DriveAnalog_U16[ALL_CHAN_NUM]; //max 80 word   ANALOG_NUM_WORD 
}DRIVE_FILE_DATA;

typedef struct
{
  	uint16_t DriveDigital_U16[DIGITAL_NUM_WORD]; //max 16 word  now 8 Word
  	uint32_t DriveAnalog_U16[VOL_CHAN_NUM]; //max 100channe
}DRIVE_FILE_OPRTNUM_DATA;
typedef struct
{

   uint8_t RecordTime_MT;
   uint8_t RecordTime_YY;
   uint8_t RecordTime_HH;
   uint8_t RecordTime_DD;
   uint8_t RecordTime_SS;
   uint8_t RecordTime_MN;
   uint8_t EnvDigital[8];
   uint8_t EnvAnalog[24];
   uint8_t FaultCode[32];
}DRIVE_LOG_DATA;

typedef enum _FILE_TYPE
{
    
	EVENT_FILE = 1,
	LOG_FILE = 2,	
	FAST_FILE = 3,
	REAL_FILE = 4,

}FILE_TYPE_ENUM;
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/

//extern uint8_t g_file_Flag_U8; /*when a new file is ture,else  is false*/
//extern uint8_t g_file_EventName_U8[40];
//extern uint32_t g_frame_EventNum_U32; /*the event data number*/

/**********************************************************************
*
*Global Prototype Declare Section
*
*********************************************************************/
int8_t FileCreatePowOn(FILE_FD * file_P,RECORD_XML * RrdXml_p,TRAIN_INFO * TranInfo_p);
int8_t WriteFile(FILE *FilePath,uint16_t Length,uint8_t *BufferIn);
unsigned long GetFileSize(char *Path) ;
int8_t EventFileCreateByNum(FILE_FD * file_P,RECORD_XML * RrdXml_p,TRAIN_INFO * TranInfo_p);
int8_t EventDataSave(FILE_FD * file_p, uint8_t ChanNumTmp,uint8_t EADSType);
/**********************************************************************
*
*Multi-Include-Prevent End Section
*
*********************************************************************/
#endif 
