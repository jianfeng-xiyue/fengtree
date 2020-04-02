/**********************************************************************
*File name 		:   DataHandle.h
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/1/29
*Description    :    Include the voltage change from 24bit to 16bit decimal value,judge the overvoltage overcurrent, 
*                   judge the channel operation, and save the g_ChanData_ST to FrameSave_ST function
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/05  Create
*REV1.0.1     feng    2018/03/04  Change the the channel high status sequence
*
*
*********************************************************************/

/**********************************************************************
*
*Multi-Include-Prevent Sart Section
*
*********************************************************************/
#ifndef _DATAHANDLE_H
#define _DATAHANDLE_H

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
#include "FileSave.h"
#include "ModifConfig.h"
#include "xml.h"
#include "Bram.h"

/**********************************************************************
*
*Global Macro Define Section
*
*********************************************************************/

/*the voltage judge*/

#define LOWVOL_L 0


#define VOL_NEGA_MIN -200  //when voltage is low,add the fite if the voltage is >20V or <-20V
#define VOL_COR_JUDEG_MAX 200
/*the current judge*/

#define LOWCURR_L 0
#define CUR_NEGA -10


//20200326
/*ADC data Trans Type*/
#define NORMTRANTYPE 0
#define CALIBTRANTYPE 1

#define VOL_NEGA -50
#define VOL_COR_JUDEG 50

#define CURR_NEGA -10
#define CURR_COR_JUDEG 5



/**********************************************************************
*
*Global Struct Define Section
*
*********************************************************************/

typedef struct 
{

	uint8_t ChanOverCurrNum_U8;
    uint8_t ChanElmtOverCurrNum_U8;
    uint8_t ChanElmtUndrCurrNum_U8;
	uint8_t ChanOverCurrFlag_U8;
	uint8_t ChanUndrCurrFlag_U8;
	uint8_t ChanHighCurrSteadNum_U8;
	uint8_t ChanUndrCurrSteadNum_U8;
	uint8_t ChanLowCurrSteadNum_U8;
	uint8_t ChanCurrStatus_U8; //0 1 status

}CURR_CHAN_JUDGE;


typedef struct 
{

	uint8_t ChanOverVolNum_U8;
	uint8_t ChanElmtOverVolNum_U8;
	uint8_t ChanOverVolFlag_U8; /*for over voltage process*/
	uint8_t ChanUndrVolFlag_U8;
	uint8_t ChanHighVolSteadNum_U8;
	uint8_t ChanUndrVolSteadNum_U8;
	uint8_t ChanElmtUndrVolNum_U8;
	uint8_t ChanLowVolSteadNum_U8;
	uint32_t ChanStartTime_U32; /*the unit is us,need 32bit*/
	uint32_t ChanEndTime_U32;
	uint16_t ChanLR_U16;
	uint8_t ChanVolStatus_U8; /*for channel status*/  	//0 1 status
	uint8_t ChanOperFlag_U8;/*when operate, it set 1*/

}VOL_CHAN_JUDGE;

typedef struct 
{
	VOL_CHAN_JUDGE VolChanJudge[VOL_CHAN_NUM];
	CURR_CHAN_JUDGE CurrChanJudge[CURR_CHAN_NUM];
	
}CHAN_JUDGE;


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
*Global Prototype D1eclare Section
*
*********************************************************************/
int8_t DataProcPowerOn(CHAN_STATUS_INFO * ChanStatInfo_ST_p, CHAN_CALIB_DATA * ChCalibDtap, uint8_t ChanNumTmp,uint8_t CoachNum);
int8_t EADSDataProc(uint8_t ChanNumTmp,uint8_t CoachType);
int8_t EADSDataBuf(uint8_t ChanNumTmpP);
int8_t ChanDataFilt(CHAN_DATA * InChdata,CHAN_DATA * OutChData,uint8_t ChanNumTmp,uint8_t EADSType);

/**********************************************************************
*
*Multi-Include-Prevent End Section
*
*********************************************************************/
#endif 
