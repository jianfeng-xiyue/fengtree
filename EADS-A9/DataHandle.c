/**********************************************************************
*File name 		:   DataHandle.c
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/1/29
*Description    :   Include the voltage change from 24bit to 16bit decimal value,judge the overvoltage overcurrent, 
*                   judge the channel operation,     
*                  20180224: 
*                   1.simulater bram data
*	                2. judge vol and current, set the channel status
*	                3. Calutulater LRtime and operatureNum
*	                4.save 5 frame to file and save the g_ChanData_ST to FrameSave_ST
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/05  Create
*REV1.0.1     feng    2019/5/21  Add VolState to Digial Data and send TRDP
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
#include "DataHandle.h"
 
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/
//2020026

//20200326
extern BLVDS_BRD_DATA g_BrdRdData_ST;
extern BLVDS_BRD_DATA g_BrdRdBufData_ST;
extern CHAN_DATA  g_ChanData_ST[CHAN_BUFFER_NUM];
extern CHAN_DATA  g_ChanRealBuf_ST[REAL_BUFFER_NUM];
extern CHAN_DATA  g_ChanData_ProcBuf_ST[CHAN_BUFFER_NUM];  /*304 byte*/
extern CHAN_DATA  g_ChanSendBuf_ST[CHAN_BUFFER_NUM];  /*304 byte*/

extern CHAN_STATUS_INFO  g_ChanStatuInfo_ST;
extern CHAN_CALIB_DATA  g_ChanCalib110VData_ST;
extern FILE_FD g_FileFd_ST;

extern RECORD_XML g_Rec_XML_ST;
extern VERSION  g_Version_ST;

// extern TRDP_DATA g_Trdp_SendData_ST;

extern DEBUG_TYPE_ENUM g_DebugType_EU;


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
static CHAN_JUDGE s_ChanJudge_ST = {0};
static CHAN_THREHOLD_VALUE s_channel_ThreValue_ST = {0}; /*in ModifConfig.h*/
//s_UndrVoltJudge_H is the 77V + 5V
//s_UndrVoltJudge_L is the 33V - 5V
static uint16_t s_OverVoltJudge = 0,s_UndrVoltJudge_H = 0,s_UndrVoltJudge_L = 0;//
static uint16_t s_OverCurrJudge[CURR_CHAN_NUM] = {0};
static uint16_t s_UndrCurrJudge_H[CURR_CHAN_NUM] = {0};
static uint16_t s_UndrCurrJudge_L[CURR_CHAN_NUM] = {0};
/**********************************************************************
*
*Function Define Section
*
*********************************************************************/
/**********************************************************************
*Name			:   int8_t RecordCFGXml(char *xmlFilePath,RECORD_XML *RecordXml_p)
*Function       :   Extra the xml information of every line
*Para     		:   char *xmlFilePath, the path of xml
*					RECORD_XML *RecordXml_p
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/5/27  Create
*********************************************************************/
int8_t RecordCFGXml(char *xmlFilePath,RECORD_XML *RecordXml_p,VERSION *Versionp)
{
	char *p;
    int xml_file =0 ;
	char array[RECORD_CNT][ZONE];
	uint8_t BitMask[ZONE] = {0};
	p = xml_to_mem(xmlFilePath,RECORD_CFG);
	memcpy(array,p,sizeof(array));

	Versionp -> RrdCfgFile_Ver_U16 = atoi(array[RrdCfgFileLineNum]);

	RecordXml_p -> Rec_Event_ST.RecTypeIdx = atoi(array[EventRecTypeId]);
	memcpy(&RecordXml_p -> Rec_Event_ST.RecPath,&array[EventRecPath],strlen(&array[EventRecPath][0]));
    memcpy(&RecordXml_p -> Rec_Event_ST.RecFileHead,&array[EventRecFileHead],strlen(&array[EventRecFileHead][0]));
	RecordXml_p -> Rec_Event_ST.RecTypeIdx = atoi(array[EventRecTypeId]);
	RecordXml_p -> Rec_Event_ST.RecMaxFileNum_U16 = atoi(array[EventRecMaxFileNum]);
	RecordXml_p -> Rec_Event_ST.RecTotalChn_U8 = atoi(array[EventRecTotalChn]);
	RecordXml_p -> Rec_Event_ST.RecChnNum_U8 = atoi(array[EventRecChnNum]);

	RecordXml_p -> Rec_Event_ST.RecChnBitMask1 = strtol(array[EventRecChnBitMask1],NULL,16);
	RecordXml_p -> Rec_Event_ST.RecChnBitMask2 = strtol(array[EventRecChnBitMask2],NULL,16);
	RecordXml_p -> Rec_Event_ST.RecChnBitMask3 = strtol(array[EventRecChnBitMask3],NULL,16);
	RecordXml_p -> Rec_Event_ST.RecChnBitMask4 = strtol(array[EventRecChnBitMask4],NULL,16);
	RecordXml_p -> Rec_Event_ST.RecChnBitMask5 = strtol(array[EventRecChnBitMask5],NULL,16);
	RecordXml_p -> Rec_Event_ST.RecChnBitMask6 = strtol(array[EventRecChnBitMask6],NULL,16);

	RecordXml_p -> Rec_Event_ST.RecInterval = atoi(array[EventRecInterval]);
	RecordXml_p -> Rec_Event_ST.RecBeforeNum = atoi(array[EventRecBeforeNum ]);
	RecordXml_p -> Rec_Event_ST.RecAfterNum = atoi(array[EventRecAfterNum]);
	RecordXml_p -> Rec_Event_ST.RecToTalNum = atoi(array[EventRecToTalNum]);
	if(XML_DEBUG == g_DebugType_EU)
	{
		printf("Rec_Event_ST\n");
		printf("RecChnNum_U8 %d\n", RecordXml_p -> Rec_Event_ST.RecChnNum_U8);
		printf("RecChnBitMask1 %x\n", RecordXml_p -> Rec_Event_ST.RecChnBitMask1);
		printf("RecChnBitMask2 %x\n", RecordXml_p -> Rec_Event_ST.RecChnBitMask2);
		printf("RecChnBitMask3 %x\n", RecordXml_p -> Rec_Event_ST.RecChnBitMask3);
		printf("RecChnBitMask4 %x\n", RecordXml_p -> Rec_Event_ST.RecChnBitMask4);
		printf("RecChnBitMask5 %x\n", RecordXml_p -> Rec_Event_ST.RecChnBitMask5);
		printf("RecChnBitMask6 %x\n", RecordXml_p -> Rec_Event_ST.RecChnBitMask6);
		printf("RecInterval %d\n", RecordXml_p -> Rec_Event_ST.RecInterval);
		printf("RecBeforeNum %d\n", RecordXml_p -> Rec_Event_ST.RecBeforeNum);
		printf("RecAfterNum %d\n", RecordXml_p -> Rec_Event_ST.RecAfterNum);
		printf("RecToTalNum %u\n", RecordXml_p -> Rec_Event_ST.RecToTalNum);

	}
    RecordXml_p -> Rec_Fast_ST.RecTypeIdx = atoi(array[FastRecTypeId]);
	memcpy(&RecordXml_p -> Rec_Fast_ST.RecPath,&array[FastRecPath],strlen(&array[FastRecPath][0]));
    memcpy(&RecordXml_p -> Rec_Fast_ST.RecFileHead,&array[FastRecFileHead],strlen(&array[FastRecFileHead][0]));
	RecordXml_p -> Rec_Fast_ST.RecTypeIdx = atoi(array[FastRecTypeId]);
	RecordXml_p -> Rec_Fast_ST.RecMaxFileNum_U16 = atoi(array[FastRecMaxFileNum]);
	RecordXml_p -> Rec_Fast_ST.RecTotalChn_U8 = atoi(array[FastRecTotalChn]);
	RecordXml_p -> Rec_Fast_ST.RecChnNum_U8 = atoi(array[FastRecChnNum]);

	RecordXml_p -> Rec_Fast_ST.RecChnBitMask1 = strtol(array[FastRecChnBitMask1],NULL,16);
	RecordXml_p -> Rec_Fast_ST.RecChnBitMask2 = strtol(array[FastRecChnBitMask2],NULL,16);
	RecordXml_p -> Rec_Fast_ST.RecChnBitMask3 = strtol(array[FastRecChnBitMask3],NULL,16);
	RecordXml_p -> Rec_Fast_ST.RecChnBitMask4 = strtol(array[FastRecChnBitMask4],NULL,16);
	RecordXml_p -> Rec_Fast_ST.RecChnBitMask5 = strtol(array[FastRecChnBitMask5],NULL,16);
	RecordXml_p -> Rec_Fast_ST.RecChnBitMask6 = strtol(array[FastRecChnBitMask6],NULL,16);

	RecordXml_p -> Rec_Fast_ST.RecInterval = atoi(array[FastRecInterval]);
	RecordXml_p -> Rec_Fast_ST.RecBeforeNum = atoi(array[FastRecBeforeNum ]);
	RecordXml_p -> Rec_Fast_ST.RecAfterNum = atoi(array[FastRecAfterNum]);
	RecordXml_p -> Rec_Fast_ST.RecToTalNum = atoi(array[FastRecToTalNum]);
	if(XML_DEBUG == g_DebugType_EU)
	{
		printf("Rec_Fast_ST\n");
		printf("RecChnNum_U8 %d\n", RecordXml_p -> Rec_Fast_ST.RecChnNum_U8);
		printf("RecChnBitMask1 %x\n", RecordXml_p -> Rec_Fast_ST.RecChnBitMask1);
		printf("RecChnBitMask2 %x\n", RecordXml_p -> Rec_Fast_ST.RecChnBitMask2);
		printf("RecChnBitMask3 %x\n", RecordXml_p -> Rec_Fast_ST.RecChnBitMask3);
		printf("RecChnBitMask4 %x\n", RecordXml_p -> Rec_Fast_ST.RecChnBitMask4);
		printf("RecChnBitMask5 %x\n", RecordXml_p -> Rec_Fast_ST.RecChnBitMask5);
		printf("RecChnBitMask6 %x\n", RecordXml_p -> Rec_Fast_ST.RecChnBitMask6);
		printf("RecInterval %d\n", RecordXml_p -> Rec_Fast_ST.RecInterval);
		printf("RecBeforeNum %d\n", RecordXml_p -> Rec_Fast_ST.RecBeforeNum);
		printf("RecAfterNum %d\n", RecordXml_p -> Rec_Fast_ST.RecAfterNum);
		printf("RecToTalNum %u\n", RecordXml_p -> Rec_Fast_ST.RecToTalNum);

	}
	RecordXml_p -> Rec_Real_ST.RecTypeIdx = atoi(array[RealRecTypeId]);
	memcpy(& RecordXml_p -> Rec_Real_ST.RecPath,&array[RealRecPath],strlen(&array[RealRecPath][0]));
    memcpy(& RecordXml_p -> Rec_Real_ST.RecFileHead,&array[RealRecFileHead],strlen(&array[RealRecFileHead][0]));
	RecordXml_p -> Rec_Real_ST.RecMaxFileNum_U16 = atoi(array[RealRecMaxFileNum]);
	RecordXml_p -> Rec_Real_ST.RecTotalChn_U8 = atoi(array[RealRecTotalChn]);
	RecordXml_p -> Rec_Real_ST.RecChnNum_U8 = atoi(array[RealRecChnNum]);

	RecordXml_p -> Rec_Real_ST.RecChnBitMask1 = strtol(array[RealRecChnBitMask1],NULL,16);
	RecordXml_p -> Rec_Real_ST.RecChnBitMask2 = strtol(array[RealRecChnBitMask2],NULL,16);
	RecordXml_p -> Rec_Real_ST.RecChnBitMask3 = strtol(array[RealRecChnBitMask3],NULL,16);
	RecordXml_p -> Rec_Real_ST.RecChnBitMask4 = strtol(array[RealRecChnBitMask4],NULL,16);
	RecordXml_p -> Rec_Real_ST.RecChnBitMask5 = strtol(array[RealRecChnBitMask5],NULL,16);
	RecordXml_p -> Rec_Real_ST.RecChnBitMask6 = strtol(array[RealRecChnBitMask6],NULL,16);

	RecordXml_p -> Rec_Real_ST.RecInterval = atoi(array[RealRecInterval]);
	RecordXml_p -> Rec_Real_ST.RecBeforeNum = atoi(array[RealRecBeforeNum ]);
	RecordXml_p -> Rec_Real_ST.RecAfterNum = atoi(array[RealRecAfterNum]);
	RecordXml_p -> Rec_Real_ST.RecToTalNum = atoi(array[RealRecToTalNum]);
	if(XML_DEBUG == g_DebugType_EU)
	{
		printf("Rec_Real_ST\n");
		printf("RecChnNum_U8 %d\n", RecordXml_p -> Rec_Real_ST.RecChnNum_U8);
		printf("RecChnBitMask1 %x\n", RecordXml_p -> Rec_Real_ST.RecChnBitMask1);
		printf("RecChnBitMask2 %x\n", RecordXml_p -> Rec_Real_ST.RecChnBitMask2);
		printf("RecChnBitMask3 %x\n", RecordXml_p -> Rec_Real_ST.RecChnBitMask3);
		printf("RecChnBitMask4 %x\n", RecordXml_p -> Rec_Real_ST.RecChnBitMask4);
		printf("RecChnBitMask5 %x\n", RecordXml_p -> Rec_Real_ST.RecChnBitMask5);
		printf("RecChnBitMask6 %x\n", RecordXml_p -> Rec_Real_ST.RecChnBitMask6);
		printf("RecInterval %d\n", RecordXml_p -> Rec_Real_ST.RecInterval);
		printf("RecBeforeNum %d\n", RecordXml_p -> Rec_Real_ST.RecBeforeNum);
		printf("RecAfterNum %d\n", RecordXml_p -> Rec_Real_ST.RecAfterNum);
		printf("RecToTalNum %u\n", RecordXml_p -> Rec_Real_ST.RecToTalNum);

	}
}
/**********************************************************************
*Name			:   int8_t RecChanNameExtr(uint16_t *BufferIn,uint8_t *RecorChanName)
*Function       :   Extra the Chan mask value to get every chan name 
*Para     		:   uint16_t *BufferIn
*					uint8_t *RecorChanName 
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/5/7  Create
*********************************************************************/
int8_t RecChanNameExtr(uint16_t *BufferIn,uint8_t *RecorChanName)
{
    uint8_t RecTotalNum = 0;
    uint16_t ChanBitTem = 0;
    uint8_t i = 0,j = 0;
	
    for(i = 0;i < 6;i++)//6 word
    {
        ChanBitTem = *BufferIn;
        for(j = 0;j < 16;j++)//1 byte mean 8 channel
        {           
            //C code is start from channel 0,but Drive is start from channel 1
            if((ChanBitTem >> j) & 1)
            {                  	
            	*RecorChanName = j + i * 16;
            	if(XML_DEBUG == g_DebugType_EU)
            	{
					printf("RecorChanName %d\n",*RecorChanName);
            	} 
            	RecorChanName++;
            } 
        }
        BufferIn++;        	
  	}
}
/**********************************************************************
*Name			:   int8_t SysXmlParInit(char *xmlFilePath)
*Function       :   Init the xml parameter
*Para     		:   char *xmlFilePath 
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/5/27  Create
*********************************************************************/
int8_t SysXmlParInit(char *xmlFilePath,RECORD_XML *RecordXml_p,VERSION *Version_p)
{
    if(XML_DEBUG == g_DebugType_EU)
	{
		printf("xml file %s \n", xmlFilePath);
	}
    RecordCFGXml(xmlFilePath,RecordXml_p,Version_p);

}
/**********************************************************************
*Name			:   int8_t SysThreValuInit(void)

*Function       :  Init the  OverVoltage ,UnderVoltage,OverCurrJudge,UndrCurrJudge Judge 
*Para     		:   
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/2/27  Create
*********************************************************************/
int8_t SysThreValuInit(void)
{
    uint16_t  j = 0;
    /*the ThreValue  judge,Calculate the OverVoltJudge ,s_OverCurrJudge with Slug */
    s_OverVoltJudge = s_channel_ThreValue_ST.HighVol_H - s_channel_ThreValue_ST.SlugVol;
    s_UndrVoltJudge_H = s_channel_ThreValue_ST.HighVol_L + s_channel_ThreValue_ST.SlugVol;
    s_UndrVoltJudge_L = s_channel_ThreValue_ST.LowVol_H - s_channel_ThreValue_ST.SlugVol;

    for(j = 0;j < CURR_CHAN_NUM;j++)
    {
        s_OverCurrJudge[j] =  s_channel_ThreValue_ST.CurrChan_ThreValue_ST[j].HighCurr_H - s_channel_ThreValue_ST.SlugCurr;
        s_UndrCurrJudge_H[j] =  s_channel_ThreValue_ST.CurrChan_ThreValue_ST[j].HighCurr_L + s_channel_ThreValue_ST.SlugCurr;
        s_UndrCurrJudge_L[j] =  s_channel_ThreValue_ST.CurrChan_ThreValue_ST[j].LowCurr_H - s_channel_ThreValue_ST.SlugCurr;

    }

}
/**********************************************************************
*Name			:   uint8_t DataProcPowerOn(uint32_t *Channel_OperaNum_U32,CHAN_THREHOLD_VALUE *ChanThreValue_ST_p,uint8_t *ChanRecordStatus)
*Function       :   when power on,read the OperateNum threshold value and Waverecord status of 75 channel 
*Para     		:   uint32_t *Channel_OperaNum_U32: OperateNum
*					uint8_t *ChanRecordStatus:  Not uses
					uint8_t ChanNumTmp:   Not use
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/02/27  Create
*********************************************************************/
int8_t DataProcPowerOn(CHAN_STATUS_INFO * ChanStatInfo_ST_p, CHAN_CALIB_DATA * ChCalibDtap, uint8_t ChanNumTmp,uint8_t EADSType)
{
    uint8_t i =0,j=0;
	
    if(EADSType == CTU_BOARD_ID)
    {
		SysXmlParInit(RCD_CFG_ZOGPEI_COACH,&g_Rec_XML_ST,&g_Version_ST);
    }
	else
	{
       SysXmlParInit(RCD_CFG_COACH,&g_Rec_XML_ST,&g_Version_ST);
	}

    GetChanOperateNum(&ChanStatInfo_ST_p -> Channel_OperaNum_U32[0]); 
    GetPTUChanThresholdValue(LOGI_PRMTABLE_FILE,&s_channel_ThreValue_ST);
    SysThreValuInit();//must after GetPTUChanThresholdValue 
    //GetChanWaveRecordStatus(CHAN_OPER_NUM_FILE, CHANNEL_WAVERECORD_STATUS,ChanRecordStatus);
    GetChanCalibData(CHAN_CALIB_110V_FILE,ChCalibDtap);
    if(EADS_DEBUG == g_DebugType_EU)
    {
	    printf("LowVOl %d\n",s_channel_ThreValue_ST.LowVol_H);
	    printf("HigVOl_L %d\n",s_channel_ThreValue_ST.HighVol_L);
		printf("HigVOl_H %d\n",s_channel_ThreValue_ST.HighVol_H);		   
		printf("SLugVOl %d\n",s_channel_ThreValue_ST.SlugVol);
	    for(i = 0;i < CURR_CHAN_NUM;i++) 
		{  

			printf("%d LowCurr_H %d\n",i, s_channel_ThreValue_ST.CurrChan_ThreValue_ST[i].LowCurr_H);
			printf("%d HighCurr_L %d\n",i,s_channel_ThreValue_ST.CurrChan_ThreValue_ST[i].HighCurr_L);
		    printf("%d HighCurr_H %d\n",i,s_channel_ThreValue_ST.CurrChan_ThreValue_ST[i].HighCurr_H);

        }
        printf("%d SLugCurr %d\n",i,s_channel_ThreValue_ST.SlugCurr);
        
	}
    return 0;
}


/**********************************************************************
*Name			:   int8_t AllChanDataTrans
*Function       :   trans 5 Frame channel data of MAX10-1 MAX10-2 Data,group the VI data,V chan data
*Para     		:   uint8_t TranNum,
*                   uint8_t TrnsType: 0 is Tran with Calib Value;
*                                  1 is Tran without Calib Value for Calib Data Save
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/20  Create
*********************************************************************/
int8_t VolChanDataTrans(BLVDS_BRD_DATA * BrdData_ST_p, CHAN_DATA * OutBuf_p,uint8_t TranNum,uint8_t TrnsType)
{
    
    uint8_t ChanNum;
    ChanNum = VOL_CHAN_NUM; //a channel length is uint16
    uint8_t i = 0,j= 0 ;

    for(i = 0; i< TranNum;i++)
    {

	    if(BRAM_DEBUG == g_DebugType_EU)
	    {
             printf("%d times Trans VolChan\n",i);
            for(j = 0 ;j<ChanNum;j++)
            {
            	
            	printf("byte %d Value %08x\n",j,g_BrdRdData_ST.Board0_Data_U32[i][j]);
            }	    	
	    }
	   //to copy the source read data to avoid update,cannot add this
	     //memcpy(&g_BrdRdData_ST.Board0_Data_U32[ReadNum_U8][0],&g_BrdRdBufData_ST.Board0_Data_U32[ReadNum_U8][0],ADU_BRD_RD_LENGTH_U32);
 
	    //when have a config file,every time trans one channel  
	    ADC7913VDataTranInt(&BrdData_ST_p -> Board0_Data_U32[i][0],OutBuf_p,ChanNum,TrnsType);
        OutBuf_p++;
    }
    //MAXLifeRead(g_bram_RDMAX_1Data_U32[0][26],&g_ChanData_ST[0],VIChan);

    return 0;
}
/**********************************************************************
*Name			:   int8_t AllChanDataTrans
*Function       :   trans 5 Frame channel data of MAX10-1 MAX10-2 Data,group the VI data,V chan data
*Para     		:   uint8_t TranNum,
*                   uint8_t TrnsType: 0 is Tran with Calib Value;
*                                  1 is Tran without Calib Value for Calib Data Save
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/20  Create
*********************************************************************/
int8_t CurrChanDataTrans(BLVDS_BRD_DATA * BrdData_ST_p, CHAN_DATA * OutBuf_p,uint8_t TranNum,uint8_t TrnsType)
{
    
    uint8_t ChanNum =0;
    ChanNum = CURR_CHAN_NUM;
    uint8_t i = 0,j= 0 ;

    for(i = 0; i< TranNum;i++)
    {

	    if(BRAM_DEBUG == g_DebugType_EU)
	    {
            printf("%d times Trans CurrChan\n",i);
            for(j = 0 ;j<ChanNum;j++)
            {
            	
            	printf("byte %d Value %08x\n",j,g_BrdRdData_ST.Board1_Data_U32[i][j]);
            }	    	
	    }
	    //when have a config file,every time trans one channel  
	    ADC7913IDataTranInt(&BrdData_ST_p -> Board1_Data_U32[i][0],OutBuf_p,ChanNum,TrnsType);
        OutBuf_p++;
    }
    //MAXLifeRead(g_bram_RDMAX_1Data_U32[0][26],&g_ChanData_ST[0],VIChan);
    
    return 0;
}

/**********************************************************************
*Name			:   int8_t ChanCalibDataTrans
*Function       :   trans 1 Frame channel data for Calib data
*Para     		:   CHAN_DATA *g_ChanData_ST_p
*                   uint8_t TrnsType: 0 is Tran with Calib Value;
*                   1 is Tran without Calib Value for Calib Data Save
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/08/20  Create
*********************************************************************/
int8_t ChanCalibDataTrans(CHAN_DATA *g_ChanData_ST_p, uint8_t TrnsType,uint8_t NumTemp)
{
    
    
    return 0;
}
/**********************************************************************
*Name			:   int8_t ChanCalibDataFilt
*Function       :   trans 1 Frame channel data for Calib data
*Para     		:   CHAN_DATA *g_ChanData_ST_p
*                   uint8_t TrnsType: 0 is Tran with Calib Value;
*                   1 is Tran without Calib Value for Calib Data Save
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/08/20  Create
*********************************************************************/
int8_t ChanCalibDataFilt(CHAN_DATA *ChanDataIn_ST_p, CHAN_DATA *ChanDataOut_ST_p,uint8_t ChanNumTmp,uint8_t EADSType)
{
    
    uint32_t TemCurr[16] = {0},TemVol[50] = {0};
    uint8_t i = 0,j = 0;
    for(j = 0;j < ChanNumTmp;j++)
    {
	    for(i = 0;i < VOL_CHAN_NUM;i++)
	    {  
	        TemVol[i] += ChanDataIn_ST_p -> VolChan_ST[i].CH_Voltage_I16; 	       
	    }      	
	    ChanDataIn_ST_p ++;
    }
    for(i = 0;i < VOL_CHAN_NUM;i++)
	{  

	    ChanDataOut_ST_p -> VolChan_ST[i].CH_Voltage_I16 = TemVol[i] / ChanNumTmp;

    }

	if(EADSType == CTU_BOARD_ID)
	{
		/*set ChanDataIn_ST_p to original*/
		ChanDataIn_ST_p -= ChanNumTmp; 
		for(j = 0;j < ChanNumTmp;j++)
	    {

		    for(i = 0;i < CURR_CHAN_NUM;i++)
		    { 
		    	TemCurr[i] += ChanDataIn_ST_p -> CurrChan_ST[i].CH_Current_I16; 
		    	
		    }
		    ChanDataIn_ST_p++;
	    }
		for(i = 0;i < CURR_CHAN_NUM;i++)
	    {
	    	ChanDataOut_ST_p -> CurrChan_ST[i].CH_Current_I16 = TemCurr[i] / ChanNumTmp;
	    }
		
	}
    
    return 0;
}

/**********************************************************************
*Name			:   int8_t ChanDataJudge
*Function       :   Judge the channels data of 3Coach
*Para     		:   CHAN_DATA *g_ChanData_ST_p
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/6/29  Create
*REV1.0.1     feng    2018/9/12   Add UnderVoltage judge,
*Under Current not use.
*********************************************************************/
int8_t ChanDataJudge(CHAN_DATA *TemChanData_ST_p, uint8_t EADSType)
{
    uint8_t i = 0,j =0;
	
    /*start of for(i = 0;i <  VOL_CHAN_NUM;i++) */
 	for(i = 0;i < VOL_CHAN_NUM;i++) 
	{
		
		/*voltage judge*/
		//when overvol, the voltage must lower than slugVol to disable warm
	   if((TemChanData_ST_p -> VolChan_ST[i].CH_Voltage_I16 <= s_OverVoltJudge)&&(s_ChanJudge_ST.VolChanJudge[i].ChanOverVolFlag_U8 == 1))
		{
            s_ChanJudge_ST.VolChanJudge[i].ChanElmtOverVolNum_U8++;
            //printf("Vol i,%d, ChanElmtOverVolNum\n",i);

		}
		//when under vol, the voltage must bigger than slugVol to disable warm
	    else if((TemChanData_ST_p -> VolChan_ST[i].CH_Voltage_I16 >= s_UndrVoltJudge_H)&&(s_ChanJudge_ST.VolChanJudge[i].ChanUndrVolFlag_U8 == 1))
		{
            s_ChanJudge_ST.VolChanJudge[i].ChanElmtUndrVolNum_U8++;
            //printf("Vol i,%d, ChanElmtOverVolNum\n",i);

		}
		//when under vol, the voltage must bigger than slugVol to disable warm
	    else if((TemChanData_ST_p -> VolChan_ST[i].CH_Voltage_I16 <= s_UndrVoltJudge_L)&&(s_ChanJudge_ST.VolChanJudge[i].ChanUndrVolFlag_U8 == 1))
		{
            s_ChanJudge_ST.VolChanJudge[i].ChanElmtUndrVolNum_U8++;
            //printf("Vol i,%d, ChanElmtOverVolNum\n",i);

		}
		/*overvol judge*/

		if(TemChanData_ST_p -> VolChan_ST[i].CH_Voltage_I16 >= s_channel_ThreValue_ST.HighVol_H)
		{
			
			s_ChanJudge_ST.VolChanJudge[i].ChanOverVolNum_U8++;
			if(EADS_DEBUG == g_DebugType_EU)
			{
				printf("VChan:%d,CH_Voltage_I16:%d\n",i, TemChanData_ST_p -> VolChan_ST[i].CH_Voltage_I16);
			    printf("OverVol:%d\n",s_channel_ThreValue_ST.HighVol_H);
			}

		}
		/*highvol_l judge*/
		else if( TemChanData_ST_p -> VolChan_ST[i].CH_Voltage_I16 >= s_channel_ThreValue_ST.HighVol_L)
		{
            s_ChanJudge_ST.VolChanJudge[i].ChanHighVolSteadNum_U8++;

		}
		/*lowvol_h judge*/
		else if( TemChanData_ST_p -> VolChan_ST[i].CH_Voltage_I16 >= s_channel_ThreValue_ST.LowVol_H)
		{
            s_ChanJudge_ST.VolChanJudge[i].ChanUndrVolSteadNum_U8++;

		}
		/*lowvol_l judge*/
		else if( TemChanData_ST_p -> VolChan_ST[i].CH_Voltage_I16 >= LOWVOL_L)
		{
            s_ChanJudge_ST.VolChanJudge[i].ChanLowVolSteadNum_U8++;

		}
		else if(TemChanData_ST_p -> VolChan_ST[i].CH_Voltage_I16 >= VOL_NEGA) //-5V
		{
            TemChanData_ST_p -> VolChan_ST[i].CH_Voltage_I16 = 0; //Filt
            s_ChanJudge_ST.VolChanJudge[i].ChanLowVolSteadNum_U8++;

		}
		/*voltage under 0V*/
		else 
		{

			// s_ChanJudge_ST.VolChanJudge[i].ChanLowVolSteadNum_U8++;
		}

	}/*end of for(i = 0;i < VOL_CHAN_NUM;i++) */		
    
    /***must judge every channel for overcurrent and overvoltage 
    cannot  use break */
    if(EADSType == CTU_BOARD_ID)
    {
		for(i = 0;i < CURR_CHAN_NUM ;i++) 
		  {
		
			  /*current judge*/
			  //when overcurrent, the current must lower than slugVol to disable warm
			  if((TemChanData_ST_p -> CurrChan_ST[i].CH_Current_I16 <= s_OverCurrJudge[i])&&(s_ChanJudge_ST.CurrChanJudge[i].ChanOverCurrFlag_U8 == 1))
			  {
				  s_ChanJudge_ST.CurrChanJudge[i].ChanElmtOverCurrNum_U8++;
		
			  }
			  // //when Undr current, the current must bigger than slugVol to disable warm
			  // else if((TemChanData_ST_p -> CurrChan_ST[i].CH_Current_I16 >= s_UndrCurrJudge_H[i])&&(s_ChanJudge_ST.CurrChanJudge[i].ChanUndrCurrFlag_U8 == 1))
			  // {
		//	   s_ChanJudge_ST.CurrChanJudge[i].ChanElmtUndrCurrNum_U8++;
		
			  // }
			  // //when Undr current, the current must bigger than slugVol to disable warm
			  // else if((TemChanData_ST_p -> CurrChan_ST[i].CH_Current_I16 <= s_UndrCurrJudge_L[i])&&(s_ChanJudge_ST.CurrChanJudge[i].ChanUndrCurrFlag_U8 == 1))
			  // {
		//			 s_ChanJudge_ST.CurrChanJudge[i].ChanElmtUndrCurrNum_U8++;
		
			  // }
			  /*overcurr judge*/
			  if(TemChanData_ST_p -> CurrChan_ST[i].CH_Current_I16 >= s_channel_ThreValue_ST.CurrChan_ThreValue_ST[i].HighCurr_H)
			  {
				  
				  s_ChanJudge_ST.CurrChanJudge[i].ChanOverCurrNum_U8++;
				  if(EADS_DEBUG == g_DebugType_EU)
				  {
					  printf("judge Curr Chan:%d CH_Current_I16:%d\n",i, TemChanData_ST_p -> CurrChan_ST[i].CH_Current_I16);
					  printf("OverCur:%d\n",s_channel_ThreValue_ST.CurrChan_ThreValue_ST[i].HighCurr_H);
				  }
		
			  }
			  /*highcurr_l judge*/
			  else if( TemChanData_ST_p -> CurrChan_ST[i].CH_Current_I16 >= s_channel_ThreValue_ST.CurrChan_ThreValue_ST[i].HighCurr_L)
			  {
				  s_ChanJudge_ST.CurrChanJudge[i].ChanHighCurrSteadNum_U8++;
		
			  }
			  // /*lowcurr_h judge*/
			  // else if( TemChanData_ST_p -> CurrChan_ST[i].CH_Current_I16 >= s_channel_ThreValue_ST.CurrChan_ThreValue_ST[i].LowCurr_H)
			  // {
		//			 s_ChanJudge_ST.CurrChanJudge[i].ChanUndrCurrSteadNum_U8++;
		
			  // }
			  /*lowcurr_l judge*/
			  else if( TemChanData_ST_p -> CurrChan_ST[i].CH_Current_I16 >= LOWCURR_L) //0ml
			  {
				  s_ChanJudge_ST.CurrChanJudge[i].ChanLowCurrSteadNum_U8++;
		
			  }
			  else if(TemChanData_ST_p -> CurrChan_ST[i].CH_Current_I16 >= CUR_NEGA)
			  {
				  TemChanData_ST_p -> CurrChan_ST[i].CH_Current_I16 = 0;
				  s_ChanJudge_ST.CurrChanJudge[i].ChanLowCurrSteadNum_U8++;
			  }
			  /*current under 0V*/
			  else 
			  {
 
			  }
		
		  }/*end of for(i = 0;i <  VOL_CHAN_NUM;i++) */
		
    
    }
      return 0;    
}
/**********************************************************************
*Name			:   uint8_t AllChanDataJudge
*Function       :   Judge 5 Frame channels data of 3Coach
*Para     		:   CHAN_DATA *g_ChanData_ST_p 
*					uint8_t ChanNumTmp  Frame Number
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/20  Create
*********************************************************************/
int8_t AllChanDataJudge(CHAN_DATA *ChanData_ST_p,uint8_t ChanNumTmp,uint8_t EADSType)
{
    
    uint8_t i = 0;
    for(i = 0; i < ChanNumTmp; i++)
    {
    	ChanDataJudge(ChanData_ST_p,EADSType);
        ChanData_ST_p++;
    }
    return 0;

}
/**********************************************************************
*Name			:   int8_t ChanDataProce
*Function       :   Proce the  Voltage ,Current level
*Para     		:   uint8_t ChanNumTmp 
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/22  Create
*********************************************************************/
int8_t ChanDataProce(uint8_t ChanNumTmp,uint8_t EADSType)
{

	uint8_t i;
	struct timeval Start_ST;
	/*one by one judge the Voltage channel*/
    for(i = 0; i < VOL_CHAN_NUM; i++)
    {
    	/*Voltage Process*/
	    	/*Elimate the OverVoltage warm*/
	    	if(s_ChanJudge_ST.VolChanJudge[i].ChanElmtOverVolNum_U8 > 0)
	    	{
	    		//printf("VI i,%d ChanElmtOverVolNum,%d\n",i,s_ChanJudge_ST.VolChanJudge[i].ChanElmtOverVolNum_U8);
	    		if (ChanNumTmp == s_ChanJudge_ST.VolChanJudge[i].ChanElmtOverVolNum_U8)
	    		{
	    			//lower than the Slug Voltage
	    			s_ChanJudge_ST.VolChanJudge[i].ChanOverVolFlag_U8 = 0;
	    		}
	    		//printf("ChanOverVolFlag_U8,%d\n",s_ChanJudge_ST.VolChanJudge[i].ChanOverVolFlag_U8 );
	    		s_ChanJudge_ST.VolChanJudge[i].ChanElmtOverVolNum_U8 = 0;
	    	}
	    	/*Elimate the UndrVoltage warm*/
	    	else if(s_ChanJudge_ST.VolChanJudge[i].ChanElmtUndrVolNum_U8 > 0)
	    	{
	    		//printf("VI i,%d ChanElmtOverVolNum,%d\n",i,s_ChanJudge_ST.VolChanJudge[i].ChanElmtOverVolNum_U8);
	    		if (ChanNumTmp == s_ChanJudge_ST.VolChanJudge[i].ChanElmtUndrVolNum_U8)
	    		{
	    			//lower than the Slug Voltage
	    			s_ChanJudge_ST.VolChanJudge[i].ChanUndrVolFlag_U8 = 0;
	    		}
	    		//printf("ChanOverVolFlag_U8,%d\n",s_ChanJudge_ST.VolChanJudge[i].ChanOverVolFlag_U8 );
	    		s_ChanJudge_ST.VolChanJudge[i].ChanElmtUndrVolNum_U8 = 0;
	    	}
	    	/*Judge Whether is overvol*/
	    	if(s_ChanJudge_ST.VolChanJudge[i].ChanOverVolNum_U8 > 0)
	    	{
	    		if (ChanNumTmp == s_ChanJudge_ST.VolChanJudge[i].ChanOverVolNum_U8)
	    		{
	    			
	    			s_ChanJudge_ST.VolChanJudge[i].ChanOverVolFlag_U8 = 1;
	                //for low voltage to over volatege, operate num ++
	    			if (0 == s_ChanJudge_ST.VolChanJudge[i].ChanVolStatus_U8)
	    			{
	    				s_ChanJudge_ST.VolChanJudge[i].ChanOperFlag_U8 = 1;
	    			}
	    			//to avod when overvoltage to high voltage,the operate ++
	    			s_ChanJudge_ST.VolChanJudge[i].ChanVolStatus_U8 = 1;
	    		}
	    		s_ChanJudge_ST.VolChanJudge[i].ChanOverVolNum_U8 = 0;
	    	}/*end of ChanOverVolNum_U8>0 */
	    	/*Judge Whether is Undrvol*/
	        else if(s_ChanJudge_ST.VolChanJudge[i].ChanUndrVolSteadNum_U8 > 0)
	    	{
	    		if (ChanNumTmp == s_ChanJudge_ST.VolChanJudge[i].ChanUndrVolSteadNum_U8)
	    		{
	    			
	    			s_ChanJudge_ST.VolChanJudge[i].ChanUndrVolFlag_U8 = 1;
	    		}
	    		s_ChanJudge_ST.VolChanJudge[i].ChanUndrVolSteadNum_U8 = 0;
	    	}/*end of ChanOverVolNum_U8>0 */
	    	/*Judge Whether is high vol Status*/
	    	/*when vol from 0 to 1,the s_ChanJudge_ST.VolChanJudge[i].ChanVolStatus_U8 =1, else keep 0*/
	        else if (s_ChanJudge_ST.VolChanJudge[i].ChanHighVolSteadNum_U8 > 0)
	        {
	        	if (ChanNumTmp == s_ChanJudge_ST.VolChanJudge[i].ChanHighVolSteadNum_U8)
	    		{
	    			/*the voltage tran from 0 to 1*/
	    			if (0 == s_ChanJudge_ST.VolChanJudge[i].ChanVolStatus_U8)
	    			{
	    				s_ChanJudge_ST.VolChanJudge[i].ChanVolStatus_U8 = 1;
	                  /*  s_ChanJudge_ST.VolChanJudge[i].ChanLRFlag_U8 = 1; //to step into LRtime end
	                    s_ChanJudge_ST.VolChanJudge[i].ChanOperFlag_U8 = 1;
	                    s_ChanJudge_ST.VolChanJudge[i].ChanStartTime_U32 =g_ChanData_ST[0].CurrChan_ST[i].MAX10Time_U16;                 
	    				//gettimeofday(&Start_ST, NULL );
	    				if(LR_DEBUG == g_DebugType_EU)
	    				{
	    					printf("i %d S sec:%d\n",i,s_ChanJudge_ST.VolChanJudge[i].ChanStartTime_U32); 
		
	    			    } */ 
	    			} 			
	    		}
	        }
	        /*Judge Whether is Low vol Status*/
	        /*when vol from 1 to 0,the s_ChanJudge_ST.VolChanJudge[i].ChanVolStatus_U8 =0, else keep 1*/
	        else if(s_ChanJudge_ST.VolChanJudge[i].ChanLowVolSteadNum_U8 > 0)
	        {
	        	if (ChanNumTmp == s_ChanJudge_ST.VolChanJudge[i].ChanLowVolSteadNum_U8)
	    		{
	    			/*the voltage tran from 1 to 0*/
	    			if (1 == s_ChanJudge_ST.VolChanJudge[i].ChanVolStatus_U8)
	    			{
	    				
	    				s_ChanJudge_ST.VolChanJudge[i].ChanVolStatus_U8 = 0;  			    
	   			    }   			
	    		}
	        }
        /*end of voltage process*/
        /*clear the All Number*/
        s_ChanJudge_ST.VolChanJudge[i].ChanHighVolSteadNum_U8 = 0;
   		s_ChanJudge_ST.VolChanJudge[i].ChanLowVolSteadNum_U8 = 0;
    }/*end of for(i = 0; i < CHAN_BUFFER_NUM; i++)*/
  
	if(EADSType == CTU_BOARD_ID)
	{
		for(i = 0; i < CURR_CHAN_NUM; i++)
	    {
	
	        /*end of voltage process*/
			
	        /*Start of Current Process*/
	        /*Elimate the OverCurrent warm*/
	    	if(s_ChanJudge_ST.CurrChanJudge[i].ChanElmtOverCurrNum_U8 > 0)
	    	{
	    		if (ChanNumTmp == s_ChanJudge_ST.CurrChanJudge[i].ChanElmtOverCurrNum_U8)
	    		{
	    			
	    			s_ChanJudge_ST.CurrChanJudge[i].ChanOverCurrFlag_U8 = 0;
	    		}
	    		s_ChanJudge_ST.CurrChanJudge[i].ChanElmtOverCurrNum_U8 = 0;
	    	}
	        /*Judge Whether is overcurrent*/
	    	if(s_ChanJudge_ST.CurrChanJudge[i].ChanOverCurrNum_U8 > 0)
	    	{
	    		if (ChanNumTmp == s_ChanJudge_ST.CurrChanJudge[i].ChanOverCurrNum_U8)
	    		{
	    			
	    			s_ChanJudge_ST.CurrChanJudge[i].ChanOverCurrFlag_U8 = 1;
	    			
	    		}
	    		s_ChanJudge_ST.CurrChanJudge[i].ChanOverCurrNum_U8 = 0;
	    	}/*end of ChanOverCurrNum_U8>0 */
	    	/*Judge Whether is high current Status*/
	        else if(s_ChanJudge_ST.CurrChanJudge[i].ChanHighCurrSteadNum_U8 > 0)
	        {
	        	if (ChanNumTmp == s_ChanJudge_ST.CurrChanJudge[i].ChanHighCurrSteadNum_U8)
	    		{
	    			/*the current stran from 0 to 1*/
	    			if(CURR_DEBUG == g_DebugType_EU)
	    			{
	    			 	printf("%d,HiGHCURR\n",i);
	    			}
	    			if(0 == s_ChanJudge_ST.CurrChanJudge[i].ChanCurrStatus_U8)
	    			{
	    				s_ChanJudge_ST.CurrChanJudge[i].ChanCurrStatus_U8 = 1;
            
	  				}/*end of if (0 == s_ChanJudge_ST.CurrChanJudge[i].ChanCurrStatus_U8)*/   			
	    		}
	        }/*end of else if (s_ChanJudge_ST.CurrChanJudge[i].ChanHighCurrSteadNum_U8)*/
	        /*Judge Whether is Low vol Status*/
	        else if (s_ChanJudge_ST.CurrChanJudge[i].ChanLowCurrSteadNum_U8 > 0)
	        {
	        	if (ChanNumTmp == s_ChanJudge_ST.CurrChanJudge[i].ChanLowCurrSteadNum_U8)
	    		{
	    			if(CURR_DEBUG == g_DebugType_EU)
	    			{
	    			 	printf("%d,LowCurr\n",i);
	    			}
	    			/*the current tran from 1 to 0*/
	    			if (1 == s_ChanJudge_ST.CurrChanJudge[i].ChanCurrStatus_U8)
	    			{
	    				
	    				s_ChanJudge_ST.CurrChanJudge[i].ChanCurrStatus_U8 = 0;
	    				if(LR_DEBUG == g_DebugType_EU)
		    			 {
		    			 	printf("%d,HIGh to Low Curr \n",i);
		    			 }
	   			    
	   			    }   			
	    		}
	        }
	        /*end of current process*/
	        /*clear the All Number*/
	   		s_ChanJudge_ST.CurrChanJudge[i].ChanHighCurrSteadNum_U8 = 0;
	   		s_ChanJudge_ST.CurrChanJudge[i].ChanLowCurrSteadNum_U8 = 0;
	    }/*end of for(i = 0; i < VOL_CHAN_NUM; i++*/
	}
	
    	/*one by one judge every channel*/
	 return 0;
	
}

/**********************************************************************
*Name			:   int8_t ChanDataAssigValue
*Function       :   Assign the Operate Number,Vol Current Status
*Para     		:   uint8_t ChanNumTmp
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/02/22  Create
*
*********************************************************************/
int8_t ChanDataAssigValue(CHAN_STATUS_INFO * ChanStatInfo_ST_p, uint8_t ChanNumTmp,uint8_t EADSType)
{
 	uint8_t i,j;
	

    for(i = 0;i < VOL_CHAN_NUM;i++)
 	{	    
	    /*for warm recored*/
 		if(s_ChanJudge_ST.VolChanJudge[i].ChanOverVolFlag_U8||s_ChanJudge_ST.VolChanJudge[i].ChanUndrVolFlag_U8)	    
	    {
	    	ChanStatInfo_ST_p -> RealRecorChanName[i] = 1;
	    	ChanStatInfo_ST_p -> WarmNum_U8++;
	    	ChanStatInfo_ST_p -> RealRecFlag_U8 = 1;
	    	//printf("Vol CHAN:%d\n",i);
	    }
	    /*when vol from 0 to 1,the g_channel_OperaNum_U32[i]++,else keep the same*/
        if(1 == s_ChanJudge_ST.VolChanJudge[i].ChanOperFlag_U8)
        {
        	ChanStatInfo_ST_p -> Channel_OperaNum_U32[i]++;
        	//printf("Vol CHAN:%d OperaNum %d\n",i,g_channel_OperaNum_U32[i + VOL_CHAN_NUM]);
			s_ChanJudge_ST.VolChanJudge[i].ChanOperFlag_U8 = 0;

        }
	}
	if(EADSType == CTU_BOARD_ID)
	{
	 	/*AssigValue for channel have voltage and current*/
	 	for(i = 0;i < CURR_CHAN_NUM;i++)
	 	{	   
		    /*for warm recored*/
	 		if(s_ChanJudge_ST.CurrChanJudge[i].ChanOverCurrFlag_U8)	    
		    {
		    	ChanStatInfo_ST_p -> RealRecorChanName[i] = 1;
		    	ChanStatInfo_ST_p -> WarmNum_U8++;
		    	ChanStatInfo_ST_p -> RealRecFlag_U8 = 1;

	 		}
	 	}

	}

	  return 0;
}
/**********************************************************************
*Name			:   int8_t ChanDataLowLevelCorrect_1Coach
*Function       :   if the ChanVolStatus_U8 is low,the voltage is {-5V ~ 5V},clear the current value
*Para     		:   uint8_t ChanNumTmp
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/22  Create
*********************************************************************/
int8_t ChanDataLowLevelCorrect(uint8_t ChanNumTmp,uint8_t EADSType) 
{
	uint8_t i,j;
 	/*AssigValue for channel have voltage and current*/
	for(i = 0;i < VOL_CHAN_NUM;i++)
 	{
	   
	   if(0 == s_ChanJudge_ST.VolChanJudge[i].ChanVolStatus_U8)
	    {
	        //+-5V
	        for(j = 0;j < ChanNumTmp;j++)
	        {
                if((g_ChanData_ST[j].VolChan_ST[i].CH_Voltage_I16 <= VOL_COR_JUDEG)&&
                    (g_ChanData_ST[j].VolChan_ST[i].CH_Voltage_I16 >= VOL_NEGA)) //  5V >~ >-5V 
	        	{
	        		g_ChanData_ST[j].VolChan_ST[i].CH_Voltage_I16 = 0;
	        	}
	        	
	        }
	    }
 	}/* end of for(i = 0;i < VOL_CHAN_NUM;i++)*/
	
}
/**********************************************************************
*Name			:   int8_t ChanSendDataBuf
*Function       :   copy the chandata to TCP sendbuf,to avoid proc and send the same data source  
*Para     		:   uint8_t ChanNumTmp  
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/07/09  Create
*
*********************************************************************/
int8_t ChanSendDataBuf(CHAN_DATA * InChanDatap,CHAN_DATA * SendBufChanDatap,int8_t ChanNumTmp)
{
	uint8_t i = 0;
	//should and lock
	for(i = 0;i < ChanNumTmp; i++)
	{
		memcpy(SendBufChanDatap,InChanDatap,sizeof(CHAN_DATA));	
		InChanDatap++;
		SendBufChanDatap++;	
	}
	return 0;

}
/**********************************************************************
*Name			:   ChanRealDataBuf(uint8_t ChanNumTmp)
*Function       :   copy the chandata to ChanRealDataBuf, when occur warm,save the 100 dian buff
*Para     		:   uint8_t ChanNumTmp
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/07/09  Create
*need test
*********************************************************************/
int8_t ChanRealDataBuf(CHAN_DATA * InChanDatap,CHAN_DATA * RelBufChanDatap,uint8_t ChanNumTmp,uint16_t RecBeforNum)
{
	uint8_t i = 0;
	CHAN_DATA RealBufTemp[600] = {0};
	static s_RealBufNum_U32 = 0;
	if(s_RealBufNum_U32  >= RecBeforNum) //100
	{
		//FIFO the last (s_RealBufNum_U32 -ChanNumTmp) g_ChanRealBuf_ST
		s_RealBufNum_U32  -= ChanNumTmp;
		RelBufChanDatap += ChanNumTmp; //push out the most earlist the ChanNumTmp
		memcpy(&RealBufTemp[0],RelBufChanDatap,sizeof(CHAN_DATA) * s_RealBufNum_U32 );
		
		RelBufChanDatap -= ChanNumTmp;
		memcpy(RelBufChanDatap,&RealBufTemp[0],sizeof(CHAN_DATA) * s_RealBufNum_U32 );
		
		RelBufChanDatap += s_RealBufNum_U32;
		memcpy(RelBufChanDatap,InChanDatap,sizeof(CHAN_DATA) * ChanNumTmp);
	    //keep the s_RealBufNum_U32  at the same value	    
	}
	else
	{
		RelBufChanDatap += s_RealBufNum_U32;
		memcpy(RelBufChanDatap,InChanDatap,sizeof(CHAN_DATA) * ChanNumTmp);	
		
	}
	s_RealBufNum_U32  += ChanNumTmp;
	return 0;

}

/**********************************************************************
*Name			:   int8_t ChanDataFilt
*Function       :   Filt the data,filt ChanNumTmp frames to average value,  for the eventfile save,
*					save to  g_ChanData_ProcBuf_ST[BufNum],then save to Event File
**Para     		:   uint8_t BufNum  :g_ChanData_ProcBuf_ST[BufNum]
*					uint8_t ChanNumTmp : frames number
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/22  Create
*********************************************************************/
int8_t ChanDataFilt(CHAN_DATA * InChdata,CHAN_DATA * OutChData,uint8_t ChanNumTmp,uint8_t EADSType)
{
	uint8_t i,j;
	uint32_t TemVol = 0,TemCur = 0;
 	/*AssigValue for channel have voltage and current*/
	for(i = 0;i < VOL_CHAN_NUM;i++)
 	{
	    TemVol = 0; 
		for(j = 0;j < ChanNumTmp;j++)
		{
			TemVol +=  InChdata -> VolChan_ST[i].CH_Voltage_I16;
			InChdata++;
        }
        OutChData -> VolChan_ST[i].CH_Voltage_I16 = TemVol / ChanNumTmp;
        //notice     start from the fisrt ChanData[0 ]
		InChdata -= ChanNumTmp;
 	}/* end of for(i = 0;i < VOL_CHAN_NUM;i++)*/
	if(EADSType ==  CTU_BOARD_ID)
	{
        // start from the fisrt ChanData[0 ]
	 	for(i = 0;i < CURR_CHAN_NUM;i++)
	 	{
	        TemCur = 0;
		    for(j = 0;j < ChanNumTmp;j++)
	        {
	        	TemCur += InChdata -> CurrChan_ST[i].CH_Current_I16;
	        	InChdata++;
	        }
	        OutChData -> CurrChan_ST[i].CH_Current_I16 = TemCur / ChanNumTmp;
	         // notice    start from the fisrt ChanData[0 ]
		    InChdata -= ChanNumTmp;

	 	}/* end of for(i = 0;i <  VOL_CHAN_NUM;i++)*/

	}

}
/**********************************************************************
*Name			:   int8_t EADSDataProc(uint8_t ChanNumTmpP)
*Function       :   Proce the EADS data 
*Para     		:   uint8_t ChanNumTmpP : the frame number
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/22  Create
*REV1.0.1     feng    2018/08/22  delete the ChanDataLowLevelCorrect(ChanNumTmpP)

*REV1.0.2     feng    2019/5/21  Add VolState to Digial Data and send TRDP
*********************************************************************/
int8_t EADSDataProc(uint8_t ChanNumTmp,uint8_t EADSType_U8)
{
 
	uint8_t i = 0 ;
	for(i = 0;i < ChanNumTmp;i++)
	{
		memcpy(&g_BrdRdData_ST.Board0_Data_U32[i][0],&g_BrdRdBufData_ST.Board0_Data_U32[i][0],(ADU_BRD_RD_LENGTH_U32 << 2));
	}
	//whether should ad?????  ,to copy the source read data to avoid update,
	if(EADSType_U8 == CTU_BOARD_ID)
	{
		for(i = 0;i < ChanNumTmp;i++)
		{
			memcpy(&g_BrdRdData_ST.Board1_Data_U32[i][0],&g_BrdRdBufData_ST.Board1_Data_U32[i][0],(CTU_BRD_RD_LENGTH_U32 << 2));
		}
		CurrChanDataTrans(&g_BrdRdData_ST,&g_ChanData_ST[0],ChanNumTmp,NORMTRANTYPE);	

	}

    // if(g_DebugType_EU == EADS_DEBUG)
    // {
    // 	for(i = 0;i< ChanNumTmp;i++)
    // 	{
	   //   	printf("g_ChanData_ST %d DATA afer CurrChanDataTrans\n",i);
	   //  	ChanDataPrint(&g_ChanData_ST[i],EADSType_U8);//uint16_t length   		
    // 	}

    // }
	VolChanDataTrans(&g_BrdRdData_ST,&g_ChanData_ST[0],ChanNumTmp,NORMTRANTYPE);	
	AllChanDataJudge(&g_ChanData_ST[0],ChanNumTmp,EADSType_U8);
    ChanDataProce(ChanNumTmp,EADSType_U8);
    // if(g_DebugType_EU == EADS_DEBUG)
    // {
    // 	printf("g_ChanData_ST[0] DATA \n");
    // 	ChanDataPrint(&g_ChanData_ST[0],EADSType_U8);//uint16_t length
    // }
	ChanDataAssigValue(&g_ChanStatuInfo_ST,ChanNumTmp,EADSType_U8);
	//// ChanDataLowLevelCorrect(ChanNumTmp,EADSType_U8);//correct the current when -1 ~1 V
    
}

/**********************************************************************
*Name			:   int8_t EADSDataCorrect
*Function       :   Buf the Send and RealSave data
*Para     		:   uint8_t ChanNumTmpP 
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/02/22  Create
*********************************************************************/
int8_t EADSDataBuf(uint8_t ChanNumTmpP)
{
	/*avoid not send,and the data is update*/
	ChanSendDataBuf(&g_ChanData_ST[0],&g_ChanSendBuf_ST[0],ChanNumTmpP);
	ChanRealDataBuf(&g_ChanData_ST[0],&g_ChanRealBuf_ST[0],ChanNumTmpP,g_Rec_XML_ST.Rec_Real_ST.RecBeforeNum);
}


