/**********************************************************************
*File name      :   ModifConfig.c
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
*
*********************************************************************/

/**********************************************************************
*
*Debug switch  section
*
*********************************************************************/


/**********************************************************************
*
*Include file  section
*
*********************************************************************/
#include "ModifConfig.h"
 
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/
extern RECORD_XML g_Rec_XML_ST;
extern uint16_t g_Ver_SysPrmFile_U16;
/**********************************************************************
*
*Local Macro Define Section
*
*********************************************************************/

/**********************************************************************
*
*Local Struct Define Section
*
*********************************************************************/

/**********************************************************************
*
*Local Prototype Declare Section
*
*********************************************************************/

/**********************************************************************
*
*Static Variable Define Section
*
*********************************************************************/

/**********************************************************************
*
*Function Define Section
*
*********************************************************************/

/**********************************************************************
*Name           :   int8_t GetChanCalibData(CHAN_CALIB_DATA *ChanCalibDat_ST_p)
*Function       :   Read the Chan Calibrate Data from /tffs0/ChanCalibValue.dat"
*Para           :   CHAN_CALIB_DATA *ChanCalibDat_ST_p: the Struct of ChanCalib
*Return         :   0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/7/27  Create
*
*********************************************************************/
int8_t GetChanCalibData(uint8_t *Path,CHAN_CALIB_DATA *ChanCalibDat_ST_p)
{
    FILE *fd_CalibName;
    fd_CalibName = fopen(Path, "r+"); //every time creat the file and write replace
    if(fd_CalibName == NULL)
    {
        perror("open ChanCalibValue.dat file failed");
        return -1;//must add reture to avoid run the next fread() to cause error

    }
    fread(ChanCalibDat_ST_p,2,ALL_CHAN_NUM,fd_CalibName);
    fclose(fd_CalibName);
    return 0;

}
/**********************************************************************
*Name           :   int8_t GetChanOperateNum(uint32_t *Channel_OperNum_U32)

*Function       :   Read the Chan Operatre Num from "/yaffs/ChanOperaNum.dat"
*Para           :   
*Return         :   uint8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/7/27  Create
*REV1.0.0     feng    2018/8/17  change for 6che
*
*********************************************************************/
int8_t GetChanOperateNum(uint32_t *Channel_OperNum_U32)
{
    FILE *fd_OperNumName;
    int8_t error =0;
    uint32_t freadNum = 0;
    uint32_t OpeNumTem[50] = {0};
    uint8_t i =0;
    fd_OperNumName = fopen(CHAN_OPER_NUM_FILE, "r+"); //every time creat the file and write replace
    if(fd_OperNumName == NULL)
    {
        perror("open ChanOperaNum.dat file failed");
        return -1;

    }
    error = fseek(fd_OperNumName,130,SEEK_SET);//skip drive top
    if(error  == -1)
    {
        perror("Fseek ChanOperaNum.dat  error");

    }
    freadNum = fread(OpeNumTem,4,g_Rec_XML_ST.Rec_Fast_ST.RecChnNum_U8,fd_OperNumName);
    // g_Rec_XML_ST.Rec_Fast_ST.RecChnNum_U8 - 1 ,because ChanOpeNum_P is 75
    // for(i = 0;i < g_Rec_XML_ST.Rec_Fast_ST.RecChnNum_U8 - 1;i++)
    // {
    //     *Channel_OperNum_U32 = OpeNumTem[i];
    //     Channel_OperNum_U32++;     
    // }
    // *Channel_OperNum_U32 = OpeNumTem[i];//for the last assig value
    //the 0-39  V channel
    for(i = 0;i < VOL_CHAN_NUM;i++)
    {
        *Channel_OperNum_U32 = OpeNumTem[i];
        Channel_OperNum_U32++;     
    }
    fclose(fd_OperNumName);
    return 0;

}
/**********************************************************************
*Name           :   int8_t GetPTUChanThresholdValue(uint8_t *ConfPath_p,CHAN_THREHOLD_VALUE *OutputValue_p)
*Function       :   Read the 75 Chan ThresholdValue from  "/tffs0/UsrSYSPrmTable.dat"   
*                   every channel has it own ThreholdValue.The ChaneConfig.ini file has 75 channel data         
*Para           :   uint8_t *ConfPath:  the file path
*                   CHAN_THREHOLD_VALUE *OutputValue_p:  save the operate Number 
*Return         :   0 success,-1,false;
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/07/27  Create
*
*********************************************************************/
int8_t GetPTUChanThresholdValue(uint8_t *ConfPath_p,CHAN_THREHOLD_VALUE *OutputValue_p)
{
    uint8_t  ChanValue[PARMTABLE_LENGTH]  = {0};
    uint16_t ValueTemp = 0;
    uint8_t j = 0; 
    FILE *fp = fopen(ConfPath_p, "r+");
    if(fp == NULL)
    {
        perror("open UsrSYSPrmTable.dat file failed");
        return -1;
    }
    fread(ChanValue, 1,PARMTABLE_LENGTH,fp);
    ValueTemp =  (ChanValue[4] << 8) + ChanValue[5];
    //g_Ver_SysPrmFile_U16 = ValueTemp;

    ValueTemp =  (ChanValue[8] << 8) + ChanValue[9];
    OutputValue_p -> LowVol_H =  ValueTemp;
    ValueTemp =  (ChanValue[10] << 8) + ChanValue[11];
    OutputValue_p -> HighVol_L =  ValueTemp;
    ValueTemp =  (ChanValue[12] << 8) + ChanValue[13];
    OutputValue_p -> HighVol_H =  ValueTemp;
    ValueTemp =  (ChanValue[14] << 8) + ChanValue[15];
    OutputValue_p -> SlugVol =  ValueTemp;
	
    for(j = 0; j < CURR_CHAN_NUM;j++)
    {
        ValueTemp = (ChanValue[16 + j * 6] << 8) + ChanValue[17 + j * 6];
        OutputValue_p -> CurrChan_ThreValue_ST[j].LowCurr_H =  ValueTemp;
        ValueTemp = (ChanValue[18 + j * 6] << 8) + ChanValue[19 + j * 6];
        OutputValue_p -> CurrChan_ThreValue_ST[j].HighCurr_L =  ValueTemp;
        ValueTemp = (ChanValue[20 + j * 6] << 8) + ChanValue[21 + j * 6];
        OutputValue_p -> CurrChan_ThreValue_ST[j].HighCurr_H =  ValueTemp; 
    }
	//need change the sysCfgFile
    ValueTemp = (ChanValue[100] << 8) + ChanValue[101];
    OutputValue_p -> SlugCurr =  ValueTemp;
    fclose(fp); 
    return 0;

}
/**********************************************************************
*Name           :   int8_t GetPTUChanThresholdValue(uint8_t *ConfPath_p,CHAN_THREHOLD_VALUE *OutputValue_p)
*Function       :   Read the 75 Chan ThresholdValue from  "/tffs0/UsrSYSPrmTable.dat"   
*                   every channel has it own ThreholdValue.The ChaneConfig.ini file has 75 channel data         
*Para           :   uint8_t *ConfPath:  the file path
*                   CHAN_THREHOLD_VALUE *OutputValue_p:  save the operate Number 
*Return         :   0 success,-1,false;
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/07/27  Create
*
*********************************************************************/


int8_t ChanDataPrint(CHAN_DATA *ChanData_ST_p,uint8_t EADSType)//uint16_t length
{
        
    uint8_t j= 0;
    for(j = 0;j < VOL_CHAN_NUM;j++)
    { 
 
       printf("VChan %d :%d\n",j,ChanData_ST_p -> VolChan_ST[j].CH_Voltage_I16); 
    
    }
    if(EADSType == CTU_BOARD_ID)
    {
        for(j = 0;j < CURR_CHAN_NUM;j++)
        { 
 
            printf("CurrChan %d :%d\n",j,ChanData_ST_p -> CurrChan_ST[j].CH_Current_I16);
    
        }
        
    }
    return 0;
}