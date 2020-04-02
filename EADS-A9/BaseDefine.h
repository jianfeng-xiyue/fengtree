/**********************************************************************
*File name 		:   BaseDefine.h
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/3/29
*Description    :   Include base define and library
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/03/29  Create
*
*
*********************************************************************/

/**********************************************************************
*
*Multi-Include-Prevent Sart Section
*
*********************************************************************/
#ifndef _BASEDEFINE_H
#define _BASEDEFINE_H

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
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <pthread.h>
#include <signal.h>  //包含signal()函数
#include <semaphore.h>
#include <errno.h>

/**********************************************************************
*
*Global Macro Define Section
*
*********************************************************************/

/*EADS Version History*/
/*1.0.0   20180903*/
#define EADS_VERSION 1000

#define BRAM_BLOCK_LENGTH 256
#define BRAM_BLOCK_NUM 128


// #define REAL_BUFFER_NUM 800
/*the channel number of once judgea */

//570 channel
#define OUTIO570_CHAN 1
#define COMMAND570_CHAN 3

#define RD_570FEDERR_NUM 10 


#define UPSURE_BLOCKNUM 0xDDDD
#define UPSTAR_BLOCKNUM 0xEEEE
#define UPSUCC_BLOCKNUM 0xFFFF
//20200326
 #define CHAN_BUFFER_NUM 5  //maybe is CHAN_BUFFER_NUM = 5;casue threshold change
 #define REAL_BUFFER_NUM 200 //the   
/*Board Date LengthID*/
#define ADU_BRD_RD_LENGTH_U32  29 ///
#define CTU_BRD_RD_LENGTH_U32  29 // include top


#define ADU_BRD_DAT_RD_LNGTH_U8  116
#define CTU_BRD_DAT_RD_LNGTH_U8  116

#define COACH1 1
#define COACH2 2
#define COACH3 3
#define COACH4 4
#define COACH5 5
#define COACH6 6
#define COACH7 7
#define COACH8 8
#define COACHZP 9

#define ADU_BOARD_ID 0
#define CTU_BOARD_ID 1

/*the number of channel  have voltage and current*/
#define VOL_CHAN_NUM 40
#define CURR_CHAN_NUM 14

#define  ALL_CHAN_NUM 54


/**********************************************************************
*
*Global Struct Define Section
*
*********************************************************************/

typedef struct
{
	uint8_t BLVDSTOPType_U8;/*0xC2*/
	uint8_t BLVDSTOPVersion_U8;/*0x11*/
	uint8_t BLVDSTOPBoardAddre_U8;/*0x04*/
	uint8_t BLVDSLength_U8;/*0x*/
	uint8_t BLVDSReser_U8[8];/*0x00*/
	uint8_t BLVDSData_U8[244];

}BRAM_PACKET_DATA_U8;/*256Byte*/
typedef struct
{
	uint32_t BLVDSTOP_U32;/*0xC21104*/
	uint32_t BLVDSReser_U32[2];/*0x00*/
    uint32_t BLVDSData_U32[61];

}BRAM_PACKET_DATA;/*256Byte*/

typedef struct 
{

	uint32_t EventNum_U32;
	uint16_t FrameLength_U16;

}CHAN_TOP; 

typedef struct 
{
	int16_t CH_Voltage_I16;//

	uint16_t LRtime_U16;     //ms
    //uint32_t OperaNum_U32;
    uint16_t MAX10Time_U16;
}VOL_CHAN_VALUE;//sizeof CURR_CHAN_VALUE is 16

typedef struct 
{
	  int16_t CH_Current_I16;//
}CURR_CHAN_VALUE;//sizeof VOL_CHAN_VALUE is 8

typedef struct
{
	//CHAN_TOP ChanTop_ST;
	VOL_CHAN_VALUE VolChan_ST[VOL_CHAN_NUM]; 
	CURR_CHAN_VALUE CurrChan_ST[CURR_CHAN_NUM]; 
}CHAN_DATA;

typedef struct 
{
	int16_t CH_Voltage_I16;//
	int16_t CH_Current_I16;//bit 0: 1 for overvoltage

}CURR_CHAN_CALIB;

typedef struct
{
	//CHAN_TOP ChanTop_ST;
	int16_t VolChanCalib_I16[VOL_CHAN_NUM];
	int16_t CurrChanCalib_I16[CURR_CHAN_NUM]; 
 
}CHAN_CALIB_DATA;

typedef struct 
{
	uint16_t LowCurr_H;
	uint16_t HighCurr_L;
	uint16_t HighCurr_H;

}CURR_CHAN_THREHOLD_VALUE;
// typedef struct 
// {
// 	uint8_t HighVol_H;
// 	uint8_t HighVol_L;
// 	uint8_t LowVol_H;

// }CHAN_V_THREHOLD_VALUE;

typedef struct 
{
	uint16_t LowVol_H; //Vol uint is 100mv
	uint16_t HighVol_L;
	uint16_t HighVol_H;
	uint16_t SlugVol;// the huicha judge
	CURR_CHAN_THREHOLD_VALUE CurrChan_ThreValue_ST[CURR_CHAN_NUM];
	uint16_t SlugCurr;
}CHAN_THREHOLD_VALUE;

typedef struct 
{
    uint8_t WarmNum_U8;
	uint8_t RealRecFlag_U8;
	uint8_t LRFlag_U8;
	uint8_t RealRecorChanName[ALL_CHAN_NUM];
	uint32_t Channel_OperaNum_U32[VOL_CHAN_NUM];	

}CHAN_STATUS_INFO;


typedef struct 
{
	uint8_t RealFileCrtNum_U8;
	uint8_t RealRecChanName[ALL_CHAN_NUM]; //which chan have select to record
	uint8_t RealChanCrtFilFlag[ALL_CHAN_NUM];//once creat a real file set to 1
    uint8_t RealFileNum_U8;

}REAL_CHAN_INFO;


typedef enum _DEBUG_TYPE
{
    RELEASE_DEBUG  = 0,
	TIME_DEBUG = 1,
	BRAM_DEBUG = 2,
	EADS_DEBUG = 3,
	TCP_DEBUG = 4,
	SEM_DEBUG = 5,
	XML_DEBUG = 6,
	ALRM_DEBUG = 7,
	FILE_DEBUG = 8,
	LR_DEBUG = 9,
	ROMTUP_DEBUG = 10,
	GPIO_DEBUG = 11,
	OVERVOL_DEBUG = 12,
	POWTIME_DEBUG = 13,
	CURR_DEBUG = 14,
	LINECAlib_DEBUG = 15,
	TRDP_DEBUG = 16,

}DEBUG_TYPE_ENUM;

typedef enum _COMMU_MCU
{

	TMS570_MCU = 1,
	MAX10_MCU= 2,

}COMMU_MCU_ENUM;

typedef enum _BRAM_RETN_STATUS
{

	RETURN_ELSE_ERROR = -3,
	RETURN_CRC_ERROR = -2,
	RETURN_ERROR = -1,
	RETURN_OK= 0,
 	RETURN_BUSY= 1,

}BRAM_RETN_ENUM;

typedef enum _CMD_STATUS
{

	CMD_ERROR = 1,
	CMD_OK = 2,
	CMD_BUSY = 3,
	CMD_CRC_ERROR = 4,
	CMD_SUCCESS = 5,

}CMD_STATUS_ENUM;

typedef struct 
{
	uint16_t Total_Ver_U16;

	uint16_t Linux_Ver_U16;
	uint16_t SySCfgFile_Ver_U16;
	uint16_t SftCfgFile_Ver_U16;
	uint16_t RrdCfgFile_Ver_U16;
	uint16_t SysPrmFile_Ver_U16;
	uint16_t EADS_RunVer_U16;
	uint16_t Boot_Ver_U16;

	uint16_t FPGA_Ver_U16;
	uint16_t Urmdik_Ver_U16;
	uint16_t DevicTree_Ver_U16;
	uint16_t TMS570_RTSVer_U16;
	uint16_t TMS570_IECVer_U16;
	uint16_t MAX1_Ver_U16;
}VERSION;

typedef struct
{
   uint16_t TrainNum_U16;
   uint8_t  CoachNum_U8;

}TRAIN_INFO;


typedef struct
{
	FILE *EventFile_fd;
	FILE *OperNumFile_fd;
	FILE *LRRecFile_fd;
	FILE *ChanCalibFile_fd;
	FILE *RealRecFile_fd[ALL_CHAN_NUM];

}FILE_FD;

typedef struct
{
	uint32_t EventSaveNum_U32;  /*the save number of event data */
	uint32_t LREventSaveNum_U32;/*the save number of event LR data */
	uint32_t RealRecorNum_U32[ALL_CHAN_NUM]; //the save number of 75 Chans

}FILE_SAVE_INFO;


typedef struct
{
    pthread_t ReadDataThread;
	pthread_t RealWaveThread;
	pthread_t ModbusThread;
	pthread_t FileSaveThread;
	pthread_t TRDPThread;
	pthread_t LedThread;
}PTHREAD_INFO;



/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/

//extern uint8_t g_file_Flag_U8; /*when a new file is ture,else  is false*/
//extern uint8_t g_file_Name_U8[40];
//extern uint32_t g_frame_EventNum_U32; /*the event data number*/

/**********************************************************************
*
*Global Prototype Declare Section
*
*********************************************************************/

/**********************************************************************
*
*Multi-Include-Prevent End Section
*
*********************************************************************/
#endif 
