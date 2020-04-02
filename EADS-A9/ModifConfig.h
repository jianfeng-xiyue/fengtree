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
#ifndef _MODIFCONFIG_H
#define _MODIFCONFIG_H

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
#include "xml.h"
/**********************************************************************
*
*Global Macro Define Section
*
*********************************************************************/
#define CHAN_OPER_NUM_FILE "yaffs/REC_FASTFLT/ChanOperaNum.dat" //"/home/feng/Desktop/xilinx_code/yaffs/REC_FASTFLT/ChanOperaNum.dat" ///

#define CHAN_CALIB_110V_FILE "/tffs0/ChanCalib110VValue.dat"

#define CHAN_CALIB_FILE "/tffs0/ChanCalibValue.dat" 
#define LOGI_PRMTABLE_FILE "/tffs0/UsrSYSPrmTable.dat" ///home/feng/Desktop/xilinx_code

/*configfile Threshold value look up content*/
#define CHANNEL_JUDGE_VALUE "[CHANNEL JUDGE VALUE]"

/*configfile Operate Number value look up content*/
#define CHANNEL_OPERATE_NUMBER "[CHANNEL OPERATION NUMBER]"
/*the number line of HRESHOLDVALUE  of every channel in ChanConfig.ini file  */
#define THRESHOLDVALUE_VI_LENGTH 8
#define THRESHOLDVALUE_V_LENGTH  4
/*configfile Channel wave record status look up content*/
#define CHANNEL_WAVERECORD_STATUS "[CHANNEL WAVERECORD STATUS]"
#define CHANNEL_OFFSET_STATUS "[CHANNEL OFFSET STATUS]"

#define PARMTABLE_LENGTH 412
/********************************************************************
*
*Global Struct Define Section
*
*********************************************************************/


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

int8_t GetPTUChanThresholdValue(uint8_t *ConfPath_p,CHAN_THREHOLD_VALUE *OutputValue_p);
int8_t GetChanCalibData(uint8_t *Path,CHAN_CALIB_DATA *ChanCalibDat_ST_p);
int8_t GetChanOperateNum(uint32_t *Channel_OperNum_U32);
int8_t ChanDataPrint(CHAN_DATA *ChanData_ST_p,uint8_t EADSType);//uint16_t length
/**********************************************************************
*
*Multi-Include-Prevent End Section
*
*********************************************************************/
#endif 
