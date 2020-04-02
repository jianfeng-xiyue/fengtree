/**********************************************************************
*File name 		:   FileSave.c
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/1/29
*Description    :   Include file creat,assign value to FRAME struct,transfer char to ASCII and write file functions.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*REV1.0.1     feng    2018/02/20 Record eventfile and waverecord
*REV1.0.2     feng    2018/03/01 optimization write file by puts
*REV1.0.3     feng    2018/03/04 optimization write file by fwrite and data proce
*REV1.0.4     feng    2018/03/09 Directory memory check and delete the earliest directory
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
#include "FileSave.h"
 
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/



//extern CHAN_THREHOLD_VALUE g_channel_ThreValue_ST;
//20200328
extern DEBUG_TYPE_ENUM g_DebugType_EU;
extern FILE_FD   g_FileFd_ST;
extern RECORD_XML g_Rec_XML_ST;
extern TRAIN_INFO g_TrainInfo_ST;

extern uint32_t g_frame_EventNum_U32; /*the event data number*/
extern CHAN_DATA g_ChanData_ST[CHAN_BUFFER_NUM];
extern CHAN_DATA  g_ChanRealBuf_ST[REAL_BUFFER_NUM];
extern CHAN_DATA  g_ChanSendBuf_ST[CHAN_BUFFER_NUM];  /*304 byte*/
extern CHAN_DATA  g_ChanDataFilt_ST;
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
*Name			:   int8_t DriveRealFileTopSet(DRIVE_FILE_TOP  *DriveTop_ST_p) 
*Function       :   Set the Real File Top Data According Record.xml
*               :   
*Para     		:   DRIVE_FILE_TOP  *DriveTop_ST_p
*
*Return    		:   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/7/7  Create
*
*********************************************************************/
int8_t DriveRealFileTopSet(DRIVE_FILE_TOP  *DriveTop_ST_p)
{   
	// int i;
	// time_t timep_ST;
 //    struct tm *now_time_p_ST;
 //    time(&timep_ST); 
 //    now_time_p_ST = localtime(&timep_ST); /*change to the local time*/
 //    FILE_TYPE_ENUM FileType;
 //    FileType = REAL_FILE;

	// DriveTop_ST_p ->RecEnDianType_U16 = 0x01; //small End
	// DriveTop_ST_p ->RecHeadSize_U16=0x82; // the top size by byte
	// DriveTop_ST_p ->RecHeadVer_U16=0x1030; 
	// DriveTop_ST_p ->RecordType_U16 = REAL_FILE; //  TYPE 1=Event 2=Log 3=Fast 4=Real
	// DriveTop_ST_p ->ProjectNo_U16 = 0x00;
	// DriveTop_ST_p ->DCUType_U16 = 0x00;
	// DriveTop_ST_p ->VehicleNo_U16= g_TrainInfo_ST.TrainName_U16;
	// DriveTop_ST_p ->DCUNO_U16=0x00;
	// DriveTop_ST_p ->RecordTotalChnNum_U16 = g_Rec_XML_ST.Rec_Real_ST.RecTotalChn_U8; //96 number
	// DriveTop_ST_p ->RecordChnNum_U16 = g_Rec_XML_ST.Rec_Real_ST.RecChnNum_U8 ;      //real chanal num is 8 +70=78
	// DriveTop_ST_p ->RecordInterval_U16= g_Rec_XML_ST.Rec_Real_ST.RecInterval;
	  
	// DriveTop_ST_p ->ChnMask1_U16 = g_Rec_XML_ST.Rec_Real_ST.RecChnBitMask1; 
	// DriveTop_ST_p ->ChnMask2_U16 = g_Rec_XML_ST.Rec_Real_ST.RecChnBitMask2;// Analog signal
	// DriveTop_ST_p ->ChnMask3_U16 = g_Rec_XML_ST.Rec_Real_ST.RecChnBitMask3;
	// DriveTop_ST_p ->ChnMask4_U16 = g_Rec_XML_ST.Rec_Real_ST.RecChnBitMask4;
	// DriveTop_ST_p ->ChnMask5_U16 = g_Rec_XML_ST.Rec_Real_ST.RecChnBitMask5;
	// DriveTop_ST_p ->ChnMask6_U16 = g_Rec_XML_ST.Rec_Real_ST.RecChnBitMask6;

	// DriveTop_ST_p ->RecordBefore_U16= g_Rec_XML_ST.Rec_Real_ST.RecBeforeNum;  //fault-before point number
	// DriveTop_ST_p ->RecordAfter_U16= g_Rec_XML_ST.Rec_Real_ST.RecAfterNum;    //fault-after point number
	// DriveTop_ST_p ->RecordTotal_U16= g_Rec_XML_ST.Rec_Real_ST.RecToTalNum;   //wave count number/ log count number /not event count number

 //   //uint8_T
	// DriveTop_ST_p ->RecordTime_MT= (1 + now_time_p_ST->tm_mon);
	// DriveTop_ST_p ->RecordTime_YY = (uint8_t)((1900 + now_time_p_ST->tm_year) - 2000);
	// DriveTop_ST_p ->RecordTime_HH = (now_time_p_ST->tm_hour);
	// DriveTop_ST_p ->RecordTime_DD = (now_time_p_ST->tm_mday);
	// DriveTop_ST_p ->RecordTime_SS = (now_time_p_ST->tm_sec);
	// DriveTop_ST_p ->RecordTime_MN = (now_time_p_ST->tm_min);
	// for(i=0;i<16;i++)
	// {
	// 	DriveTop_ST_p ->EnvData_U16[i] = 0x00;
	// 	DriveTop_ST_p ->FaultCode_U16[i] = 0x00;
	//  }

	// DriveTop_ST_p ->HavePulse_U16=0x00;
	// DriveTop_ST_p ->RecordTotalChnNumPulse_U16=0x00;
	// DriveTop_ST_p ->RecordChnNumPulse_U16=0x00;;
	// DriveTop_ST_p ->RecordIntervalPulse_U16=0x00;

	// DriveTop_ST_p ->PulseChnMask1_U16=0x00;
	// DriveTop_ST_p ->PulseChnMask2_U16=0x00;
	// DriveTop_ST_p ->PulseChnMask3_U16=0x00;
	// DriveTop_ST_p ->PulseChnMask4_U16=0x00;
	// DriveTop_ST_p ->PulseChnMask5_U16=0xEEEE;
	// DriveTop_ST_p ->PulseChnMask6_U16=0xEEEE;
	// return 0;
}
/**********************************************************************
*Name			:   int8_t DriveRealFileTopSave(uint8_t ChanNum)  
*Function       :   Set Real File Top of Drive,and Write to file g_fd_RealRecName[ChanNum]
*               :   
*Para     		:   uint8_t ChanNum: Real File of the ChanNum
*Return    		:   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/7/7  Create
*
*********************************************************************/
int8_t DriveRealFileTopSave(uint8_t ChanNum)
{
    // int8_t fwerr = 0;
    // DRIVE_FILE_TOP DriveRealTop_ST = {0};
    // DriveRealFileTopSet(&DriveRealTop_ST);
    // fwerr = fwrite(&DriveRealTop_ST,sizeof(DRIVE_FILE_TOP),1,g_fd_RealRecName[ChanNum]);
    // if(fwerr)
    // {
    // 	return 0;
    // }
    // else
    // {
    // 	return -1;
    // } 

}
/**********************************************************************
*Name			:   int8_t DriveEventFileTopSet(DRIVE_FILE_TOP  *DriveTop_ST_p)  
*Function       :   Set the Event File Top Data According Record.xml 
*Para     		:   DRIVE_FILE_TOP  *DriveTop_ST_p
*
*Return    		:   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/3/7  Create
*
* Note          :   ChnMask_U16 is Set for Drive warm file analyze,VI chan take 2 bit 
*********************************************************************/
int8_t EventFileTopSet(TRAIN_INFO * TranInfo_ST_p, RECORD_XML * RecXm_p,DRIVE_FILE_TOP  *DriveTop_ST_p)
{   
	int i;
	time_t timep_ST;
    struct tm *now_time_p_ST;
    time(&timep_ST); 
    now_time_p_ST = localtime(&timep_ST); /*change to the local time*/
    FILE_TYPE_ENUM FileType;
    FileType = EVENT_FILE;

    DriveTop_ST_p ->RecEnDianType_U16 = 0x01; //small End
	DriveTop_ST_p ->RecHeadSize_U16=0x82; // the top size by byte
	DriveTop_ST_p ->RecHeadVer_U16=0x1030; 
	DriveTop_ST_p ->RecordType_U16 = FileType; //  TYPE 1=Event 2=Log 3=Fast 4=Real
	DriveTop_ST_p ->ProjectNo_U16 = 0x00;
	DriveTop_ST_p ->DCUType_U16 = 0x00;
	DriveTop_ST_p ->VehicleNo_U16 = TranInfo_ST_p -> TrainNum_U16;
	DriveTop_ST_p ->DCUNO_U16 = 0x00;
	DriveTop_ST_p ->RecordTotalChnNum_U16 = RecXm_p -> Rec_Event_ST.RecTotalChn_U8; //96 number
	DriveTop_ST_p ->RecordChnNum_U16 = RecXm_p -> Rec_Event_ST.RecChnNum_U8 ;      //Event chanal num is 8 +70=78
	DriveTop_ST_p ->RecordInterval_U16= RecXm_p -> Rec_Event_ST.RecInterval;
	  
	DriveTop_ST_p ->ChnMask1_U16 = RecXm_p -> Rec_Event_ST.RecChnBitMask1; 
	DriveTop_ST_p ->ChnMask2_U16 = RecXm_p -> Rec_Event_ST.RecChnBitMask2;// Analog signal
	DriveTop_ST_p ->ChnMask3_U16 = RecXm_p -> Rec_Event_ST.RecChnBitMask3;
	DriveTop_ST_p ->ChnMask4_U16 = RecXm_p -> Rec_Event_ST.RecChnBitMask4;
	DriveTop_ST_p ->ChnMask5_U16 = RecXm_p -> Rec_Event_ST.RecChnBitMask5;
	DriveTop_ST_p ->ChnMask6_U16 = RecXm_p -> Rec_Event_ST.RecChnBitMask6;

	DriveTop_ST_p ->RecordBefore_U16= RecXm_p -> Rec_Event_ST.RecBeforeNum;  //fault-before point number
	DriveTop_ST_p ->RecordAfter_U16= RecXm_p ->  Rec_Event_ST.RecAfterNum;    //fault-after point number
	DriveTop_ST_p ->RecordTotal_U16= RecXm_p -> Rec_Event_ST.RecToTalNum;   //wave count number/ log count number /not event count number

   //uint8_T
	DriveTop_ST_p ->RecordTime_MT= (1 + now_time_p_ST->tm_mon);
	DriveTop_ST_p ->RecordTime_YY = (uint8_t)((1900 + now_time_p_ST->tm_year) - 2000);
	DriveTop_ST_p ->RecordTime_HH = (now_time_p_ST->tm_hour);
	DriveTop_ST_p ->RecordTime_DD = (now_time_p_ST->tm_mday);
	DriveTop_ST_p ->RecordTime_SS = (now_time_p_ST->tm_sec);
	DriveTop_ST_p ->RecordTime_MN = (now_time_p_ST->tm_min);
	for(i=0;i<16;i++)
	{
		DriveTop_ST_p ->EnvData_U16[i] = 0x00;
		DriveTop_ST_p ->FaultCode_U16[i] = 0x00;
	}

	DriveTop_ST_p ->HavePulse_U16=0x00;
	DriveTop_ST_p ->RecordTotalChnNumPulse_U16=0x00;
	DriveTop_ST_p ->RecordChnNumPulse_U16=0x00;;
	DriveTop_ST_p ->RecordIntervalPulse_U16=0x00;

	DriveTop_ST_p ->PulseChnMask1_U16=0x00;
	DriveTop_ST_p ->PulseChnMask2_U16=0x00;
	DriveTop_ST_p ->PulseChnMask3_U16=0x00;
	DriveTop_ST_p ->PulseChnMask4_U16=0x00;
	DriveTop_ST_p ->PulseChnMask5_U16=0xEEEE;
	DriveTop_ST_p ->PulseChnMask6_U16=0xEEEE;
	return 0;
}

/**********************************************************************
*Name			:   int8_t DriveEventFileTopSave(void)
*Function       :   Set Event File Top of Drive,and Write to file g_fd_EventName;
*               :   
*Para     		:   
*Return    		:   uint8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/7/7  Create
*********************************************************************/
int8_t EventFileTopSave(FILE_FD *FileFd_p, RECORD_XML * RecXm_p,TRAIN_INFO * TranInfo_ST_p)
{
    int8_t fwerr = 0;
    DRIVE_FILE_TOP DriveEventTop_ST = {0};
    EventFileTopSet(TranInfo_ST_p,RecXm_p,&DriveEventTop_ST);
    fwerr = fwrite(&DriveEventTop_ST,sizeof(DRIVE_FILE_TOP),1,FileFd_p -> EventFile_fd);
    if(fwerr)
    {
    	return 0;
    }
    else
    {
    	return -1;
    } 

}
/**********************************************************************
*Name			:   int8_t DriveLRFileTopSave(void)
*Function       :   Set LR File Top of Drive,and Write to file g_fd_EventName;
*               :   
*Para     		:  
*Return    		:   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/7/7  Create
*********************************************************************/
int8_t DriveLRFileTopSave(void)
{
    /*
    int8_t fwerr = 0;
    DRIVE_FILE_TOP DriveLRTop_ST = {0};
    DriveLRFileTopSet(&DriveLRTop_ST);
    fwerr = fwrite(&DriveLRTop_ST,sizeof(DRIVE_FILE_TOP),1,g_fd_LRRecName);
	//to immedicate write
    fflush(g_fd_LRRecName);
    if(fwerr)
    {
    	return 0;
    }
    else
    {
    	return -1;
    } 
    */
}
/**********************************************************************
*Name			:   int8_t DriveOperNumFileTopSet(DRIVE_FILE_TOP  *DriveTop_ST_p) 
*Function       :   Set the Chan Oper Num File Top Data,use Fast file type 
*Para     		:   DRIVE_FILE_TOP  *DriveTop_ST_p
*
*Return    		:   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/3/7  Create
*REV1.0.1     feng    2018/8/7  for 3che
*Note           :     ChnMask_U16 is Set for Drive warm file analyze,VI chan take 2 bit 
*********************************************************************/
int8_t DriveOperNumFileTopSet(DRIVE_FILE_TOP  *DriveTop_ST_p)
{   
	int i;
	time_t timep_ST;
    struct tm *now_time_p_ST;
    time(&timep_ST); 
    now_time_p_ST = localtime(&timep_ST); /*change to the local time*/
   /* FILE_TYPE_ENUM FileType;
    FileType = FAST_FILE;

    DriveTop_ST_p ->RecEnDianType_U16 = 0x01; //small End
	DriveTop_ST_p ->RecHeadSize_U16 = 0x82; // the top size by byte
	DriveTop_ST_p ->RecHeadVer_U16 = 0x1030; 
	DriveTop_ST_p ->RecordType_U16 = FileType; //  TYPE 1=Event 2=Log 3=Fast 4=Real
	DriveTop_ST_p ->ProjectNo_U16 = 0x00;
	DriveTop_ST_p ->DCUType_U16 = 0x00;
	DriveTop_ST_p ->VehicleNo_U16 = g_TrainInfo_ST.TrainName_U8;
	DriveTop_ST_p ->DCUNO_U16 = 0x00;
	DriveTop_ST_p ->RecordTotalChnNum_U16 = g_Rec_XML_ST.Rec_Event_ST.RecTotalChn_U8; //96 number
	DriveTop_ST_p ->RecordChnNum_U16 = g_Rec_XML_ST.Rec_Fast_ST.RecChnNum_U8 ;      //Event chanal num is 8 +70=78
	DriveTop_ST_p ->RecordInterval_U16= g_Rec_XML_ST.Rec_Fast_ST.RecInterval;
	  
	DriveTop_ST_p ->ChnMask1_U16 = g_Rec_XML_ST.Rec_Fast_ST.RecChnBitMask1; 
	DriveTop_ST_p ->ChnMask2_U16 = g_Rec_XML_ST.Rec_Fast_ST.RecChnBitMask2;// Analog signal
	DriveTop_ST_p ->ChnMask3_U16 = g_Rec_XML_ST.Rec_Fast_ST.RecChnBitMask3;
	DriveTop_ST_p ->ChnMask4_U16 = g_Rec_XML_ST.Rec_Fast_ST.RecChnBitMask4;
	DriveTop_ST_p ->ChnMask5_U16 = g_Rec_XML_ST.Rec_Fast_ST.RecChnBitMask5;
	DriveTop_ST_p ->ChnMask6_U16 = g_Rec_XML_ST.Rec_Fast_ST.RecChnBitMask6;

	DriveTop_ST_p ->RecordBefore_U16= g_Rec_XML_ST.Rec_Fast_ST.RecBeforeNum;  //fault-before point number
	DriveTop_ST_p ->RecordAfter_U16= g_Rec_XML_ST.Rec_Fast_ST.RecAfterNum;    //fault-after point number
	DriveTop_ST_p ->RecordTotal_U16= g_Rec_XML_ST.Rec_Fast_ST.RecToTalNum;   //wave count number/ log count number /not event count number
   //uint8_T
	DriveTop_ST_p ->RecordTime_MT= (1 + now_time_p_ST->tm_mon);
	DriveTop_ST_p ->RecordTime_YY = (uint8_t)((1900 + now_time_p_ST->tm_year) - 2000);
	DriveTop_ST_p ->RecordTime_HH = (now_time_p_ST->tm_hour);
	DriveTop_ST_p ->RecordTime_DD = (now_time_p_ST->tm_mday);
	DriveTop_ST_p ->RecordTime_SS = (now_time_p_ST->tm_sec);
	DriveTop_ST_p ->RecordTime_MN = (now_time_p_ST->tm_min);
	for(i=0;i<16;i++)
	{
		DriveTop_ST_p ->EnvData_U16[i] = 0x00;
		DriveTop_ST_p ->FaultCode_U16[i] = 0x00;
	 }

	DriveTop_ST_p ->HavePulse_U16=0x00;
	DriveTop_ST_p ->RecordTotalChnNumPulse_U16=0x00;
	DriveTop_ST_p ->RecordChnNumPulse_U16=0x00;;
	DriveTop_ST_p ->RecordIntervalPulse_U16=0x00;

	DriveTop_ST_p ->PulseChnMask1_U16=0x00;
	DriveTop_ST_p ->PulseChnMask2_U16=0x00;
	DriveTop_ST_p ->PulseChnMask3_U16=0x00;
	DriveTop_ST_p ->PulseChnMask4_U16=0x00;
	DriveTop_ST_p ->PulseChnMask5_U16=0xEEEE;
	DriveTop_ST_p ->PulseChnMask6_U16=0xEEEE;

	return 1;*/
}
/**********************************************************************
*Name			:   int8_t DriveOperNumFileTopSave(void)
*Function       :   Set the Open Num Drive,and Write to file g_fd_OperNumName;
*               :   
*Para     		:  
*Return    		:   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/7/7  Create
*
*********************************************************************/
int8_t DriveOperNumFileTopSave(void)
{
    /*int8_t fwerr = 0;
    DRIVE_FILE_TOP DriveOperNumTop_ST = {0};
    DriveOperNumFileTopSet(&DriveOperNumTop_ST);
    fwerr = fwrite(&DriveOperNumTop_ST,sizeof(DRIVE_FILE_TOP),1,g_fd_OperNumName); 
    if(fwerr)
    {
    	return 0;
    }
    else
    {
    	return -1;
    }*/
}
/**********************************************************************
*Name			:   int8_t MultiDircCreate(uint8_t *DirPath)  
*Function       :   if DirPath directory no exist creat multi directory 
*               :   if DirPath directory is exist,jump out
*Para     		:   uint8_t *DirPath ./datafile/20180308
*Return    		:   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/3/7  Create
*
*********************************************************************/
int8_t MultiDircCreate(uint8_t *DirPath)   
{  
    uint16_t i,Len;  
    uint8_t Str[200] = {0}; 
    uint8_t  Status =0;
    Len=strlen(DirPath);       
    strncpy(Str, DirPath, Len);  

    //printf("MultiDircCreate DirPath%s\n", Str);
    for(i = 0; i<Len; i++ )  
    {  
        if( Str[i]=='/' )  
        {  
            Str[i] = '\0'; 

            if( access(Str,0)!=0 )  
            {   
                Status = mkdir( Str, 0777 ); 
                if (Status != 0)
				{
				    
				    if(g_DebugType_EU == FILE_DEBUG)
				    {
				    	printf("Create %s is failed.\n",Str);
				    }				    	
				    
				}
				else
				{  
					if(g_DebugType_EU == FILE_DEBUG)
				    {
				    	printf("Create %s done.\n",Str);
				    }
				    
				} 
            }
            else
            {
				if(g_DebugType_EU == FILE_DEBUG)
				{
					printf("Directory %s is exist.\n",Str);
				}				

            } 

            Str[i]='/';  
        }  
    }
    /*create the last level directory*/  
    if( Len > 0 && access(Str,0)!=0 )  
    {  
        mkdir(Str, 0777 );
        if(g_DebugType_EU == FILE_DEBUG)
		{
		    printf("make Directory %s success\n",Str ); 	
		} 
 
    }  
    return 0;  
}  
/**********************************************************************
*Name			:   int8_t DirectCreat(uint8_t *DirPath) 
*Function       :   Only creat one level directory of DirPath,
*Para     		:   
*
*Return    		:   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/3/7  Create
*
*********************************************************************/
int8_t DirectCreat(uint8_t *DirPath)
{
	uint8_t  status =0;
	status = access(DirPath,F_OK);
	if( status != 0 )
	{
		printf("%s not exist,creat\n",DirPath);
		    	
		status = mkdir(DirPath,S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IXOTH);
		    	
		if (status != 0)
		{
		    printf("Create %s is failed.\n",DirPath);
		    return -1;

		}
		else
		{
		    printf("Create %s done.\n",DirPath);
		    return 0;
		}
	}
}
/**********************************************************************
*Name			:   unsigned long GetFileSize(char *Path)
*Function       :   Get the file size
*Para     		:   char *Path
*
*Return    		:   return FileSize;  
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/03/27  Create
*********************************************************************/
unsigned long GetFileSize(char *Path)  
{  
    unsigned long FileSize = -1;      
    struct stat StatBuff;  
    if(stat(Path, &StatBuff) < 0)
    {  
        return FileSize;  
    }
    else
    {  
        FileSize = StatBuff.st_size;  
    }  
    return FileSize;  
}  
/**********************************************************************
*Name			:   int8_t GetMemSize(uint8_t *Path,uint32_t TotalSize_MB,uint32_t FreeSize_MB)
*Function       :   Get the memory size of Path directory
*Para     		:   uint8_t *Path,
*					uint32_t TotalSize_MB,
*					uint32_t FreeSize_MB
*Return    		:   int8_t 1,success;0 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/3/5  Create

*********************************************************************/
int8_t GetMemSize(uint8_t *Path,uint32_t TotalSize_MB,uint32_t *FreeSize_MB)
{
	struct statfs DiskInfo; 
	unsigned long long TotalBlocks = 0;
	unsigned long long TotalSize = 0;
	unsigned long long FreeDisk = 0;
	if(statfs(Path, &DiskInfo) == -1) 
	{
  		return -1;
 	}
	TotalBlocks = DiskInfo.f_bsize; /*the blocks size,unit is Byte*/ 
	TotalSize = DiskInfo.f_blocks * TotalBlocks;/*the number of blocks*/  
	TotalSize_MB = TotalSize >> 20;  /*the unit is MByte*/
	FreeDisk = DiskInfo.f_bfree * TotalBlocks;  /*the number of free blocks*/  
	*FreeSize_MB = FreeDisk >> 20;  /*the unit is MByte*/
	printf ("/ total=%dMB, free=%dMB\n",TotalSize_MB, *FreeSize_MB); 
	return 0;

}

/**********************************************************************
*Name			:   int8_t RemoveDirWithFile(char *DirPath)
*Function       :   recursion delete  directory and file in the  *Path 
*Para     		:   uint8_t *Path,
*					uint32_t TotalSize_MB,
*					uint32_t FreeSize_MB
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/3/5  Create
*********************************************************************/
int8_t RemoveDirWithFile(char *DirPath)
{
    char CurDir[] = ".";
    char UpDir[] = "..";
    char DirName[128];
    DIR *dirp;
    DIR  *DirOpenInfo_p = NULL;
    struct dirent *DirInfo_p;
    struct stat DirStat;

    /* if DirPath no exist ,return*/
    if ( 0 != access(DirPath, F_OK) ) 
    {
        perror("access stat error");
        return 0;
    }

    /* get the infomation of DirPath errror，return */
    if ( 0 > stat(DirPath, &DirStat) ) {
        perror("get directory stat error");
        return -1;
    }
   /*is the regular file,delete*/
    if ( S_ISREG(DirStat.st_mode) ) 
    {  
        remove(DirPath);
    } 
    /*is the directory,recursion delete*/
    else if ( S_ISDIR(DirStat.st_mode) ) 
    {   
        DirOpenInfo_p = opendir(DirPath);
        while ( (DirInfo_p = readdir(DirOpenInfo_p)) != NULL ) 
        {
            // skip "."" (current directory) and  ".. "(up directory)
            if ( (0 == strcmp(CurDir, DirInfo_p->d_name)) || (0 == strcmp(UpDir, DirInfo_p->d_name)) )
            {
                continue;
            }
			memset(DirName,0,sizeof(DirName));
            sprintf(DirName, "%s/%s", DirPath, DirInfo_p->d_name);
            printf("delete file %s\n", DirName);
            RemoveDirWithFile(DirName);   /*recursion delete*/
        }
        closedir(DirOpenInfo_p);
        rmdir(DirPath);     /*delele empty Path*/
    } 
    else 
    {
        perror("unknow file type!"); 
    }
    return 0;
}
/**********************************************************************
*Name			:   int8_t DeleteEarliestDir(uint8_t *FilePath_p)
*Function       :   Delete the earliest directory  in FilePath_p
*Para     		:   uint8_t *FilePath_p,  ./datafile
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/3/9  Create
*
*********************************************************************/
int8_t DeleteEarliestDir(uint8_t *FilePath_p)
{

    struct stat     FileStat;
    struct dirent   *DirInfo_p; 
    DIR  *DirOpenInfo_p = NULL; 
    uint8_t  FileFullName[100] = {0};
    uint8_t  EarlDirc[100] = {0};
    uint8_t  DirCompNum = 0;
    uint32_t TmpTimeSec ;
	struct timeval Nowtime_ST; 

	if((DirOpenInfo_p = opendir(FilePath_p)) == NULL)
	{

		printf("cannot open directory: %s\n",FilePath_p);
		return -1;
	}
	DirOpenInfo_p = opendir(FilePath_p);/* /datfile */
	/*chdir(FilePath_p);*/ //chage the directory to FilePath,so FileFullName can not have FilePath_p
	while((DirInfo_p = readdir(DirOpenInfo_p))!=NULL) 
	{ 

		/*Skip the current directory and up directory*/
        memset(FileFullName,0,sizeof(FileFullName));
		sprintf(FileFullName,"%s/%s",FilePath_p,DirInfo_p->d_name);/* /datfile/20180308 */
		memset(&FileStat,0,sizeof(struct stat));
		stat(FileFullName,&FileStat);/*get file infomation*/

		/* in the directory:datafile have /datafile/. and /datafile/..*/
		if(S_ISDIR(FileStat.st_mode))
		{
			if(strcmp(DirInfo_p->d_name,".")==0  || strcmp(DirInfo_p->d_name,"..")==0)
			{ 
				continue;
					           
			}
			/*cpy the first DirInfo_p->d_name to EarlDirc*/
			if(DirCompNum == 0)	
			{
				memcpy(EarlDirc,FileFullName,strlen(FileFullName));
				printf("FileFullName %s\n",FileFullName);
             		
			}		
			/*FileFullName is earlier than EarlDirc,EarlDirc first is 0*/
			if (strncmp(FileFullName,EarlDirc,strlen(FileFullName)) < 0)
            {
            	memcpy(EarlDirc,FileFullName,strlen(FileFullName)); 
            	printf("FileFullName %s\n",FileFullName);
            	printf("DirCompNum %d\n",DirCompNum);
            }
            DirCompNum ++; 
		}  
	}
	closedir(DirOpenInfo_p);
	/*if the datafile directory has only directory no delete*/
	if(DirCompNum > 1)
	{
		RemoveDirWithFile(EarlDirc);
		printf("Delete Early Directory:%s\n",EarlDirc);
	}
	return 0;
}
/**********************************************************************
*Name			:   int8_t EventFileCreateByNum(void)
*Function       :   Creat Event file by Eventsave Num,For PTU analysis
*Para     		:   void 
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/6/29  Create
*note Para  can improve with Freesize
*********************************************************************/
int8_t EventFileCreateByNum(FILE_FD * file_P,RECORD_XML * RrdXml_p,TRAIN_INFO * TranInfo_p)
{	
	//FILE *fd;
	uint8_t File_EventName_U8[200] = {0};
	uint8_t File_Directory_U8[200] = {0};
	uint8_t TimeString_U8[40] = {0};
	uint32_t TotalSizeMB_U32 = 0,FreeSizeMB_U32 = 0;
	time_t timep_ST;   
    struct tm *now_time_p_ST;
     //should make sure
    GetMemSize(RrdXml_p -> Rec_Event_ST.RecPath,TotalSizeMB_U32,&FreeSizeMB_U32);
	if(FreeSizeMB_U32 < RESER_SPACE)
	{
		DeleteEarliestDir(RrdXml_p -> Rec_Event_ST.RecPath);
	}
	time(&timep_ST);
	now_time_p_ST = localtime(&timep_ST); /*change to the local time*/
	sprintf(TimeString_U8,"%04d%02d%02d%02d%02d%02d",
	   		    (1900 + now_time_p_ST->tm_year), 
	   		    (1 + now_time_p_ST->tm_mon), now_time_p_ST->tm_mday,
	   		    now_time_p_ST->tm_hour, now_time_p_ST->tm_min, now_time_p_ST->tm_sec); 
    /* no year month folder	
    sprintf(File_Directory_U8,"%s",&g_Rec_XML_ST.Rec_Event_ST.RecPath);   
    MultiDircCreate(File_Directory_U8);
    sprintf(File_EventName_U8,"%s%s%04d_%02d-%s.dat",&g_Rec_XML_ST.Rec_Event_ST.RecPath,
	  &g_Rec_XML_ST.Rec_Event_ST.RecFileHead,g_TrainInfo_ST.TrainName_U8,g_file_CoachName_U8,TimeString_U8);*/
     // add year month folder

 	sprintf(File_Directory_U8,"%s%0.8s",&RrdXml_p-> Rec_Event_ST.RecPath[0],TimeString_U8); 
 	//for ubuntu linux test 
    ////sprintf(File_Directory_U8,"%s%0.8s","/home/feng/Desktop/xilinx_code/yaffs/",TimeString_U8);//for ubuntu linux test  
    MultiDircCreate(File_Directory_U8);
	

   	sprintf(File_EventName_U8,"%s%0.8s/%s%04d_%02d_%0.8s-%s.dat",&RrdXml_p -> Rec_Event_ST.RecPath[0],
	   TimeString_U8,&RrdXml_p->Rec_Event_ST.RecFileHead,TranInfo_p -> TrainNum_U16,TranInfo_p -> CoachNum_U8,TimeString_U8,&TimeString_U8[8]);
    //for ubuntu linux test
    // sprintf(File_EventName_U8,"%s%0.8s/%s%04d_%02d_%0.8s-%s.dat","/home/feng/Desktop/xilinx_code/yaffs/",
	   // TimeString_U8,&RrdXml_p->Rec_Event_ST.RecFileHead,TranInfo_p -> TrainNum_U16,TranInfo_p -> CoachNum_U8,TimeString_U8,&TimeString_U8[8]);


    //g_fd_EventName = fopen(File_EventName_U8, "a+"); 
    file_P -> EventFile_fd = fopen(File_EventName_U8, "a+");
	if(file_P ->EventFile_fd == NULL)
	{
		perror("creat Eventfile.dat file failed");
		return -1;

	}
	if(FILE_DEBUG == g_DebugType_EU)
	{	
		printf("creat file %s\n",File_EventName_U8);

	}
	EventFileTopSave(file_P,RrdXml_p,TranInfo_p);

}

/**********************************************************************
*Name			:   int8_t FileCreatePowOn(void)
*Function       :   when application run,creat  Event and LR file
*Para     		:   void 
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/6/29  Create
*********************************************************************/
int8_t FileCreatePowOn(FILE_FD * file_P,RECORD_XML * RrdXml_p,TRAIN_INFO * TranInfo_p)
{

 
    EventFileCreateByNum(file_P,RrdXml_p,TranInfo_p);
    ///// LRFileCreateByNum();
    //creat yaffs//REC_FASTFLT for num operature
    MultiDircCreate(&RrdXml_p -> Rec_Fast_ST.RecPath[0]);
    //EventLRFileCreateByNum(); 
	return 0;

}
/**********************************************************************
*Name			:   int8_t ChanCalibFileCreat(void)
*Function       :   Creat the /tffs0/ChanCalibValue.dat for save the calib value
*Para     		:   void 
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/6/29  Create
*********************************************************************/
int8_t ChanCalibFileCreat(uint8_t *path)
{

 //    g_fd_ChanCalibName = fopen(path,"w+"); //every time creat the file and write replace
	// if(g_fd_ChanCalibName == NULL)
	// {
	// 	perror("creat ChanCalib.dat file failed");

	// }
	// else
	// {
	// 	printf("Create file %s success\n",path);
	// }
	// //FrameFileTopSet();
	// //FrameValueSave(); 
	// return 0;

}
/**********************************************************************
*Name			:   int8_t ChanCalibDataSet(int16_t *BuffIn,CHAN_DATA *g_ChanData_ST_p)
*Function       :   Set the all Chan Calib data  
*Para     		:   int16_t *BuffIn :the data write to file
*                   CHAN_DATA *g_ChanData_ST_p :the Calib data Save ST 
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/6/29  Create
*********************************************************************/
int8_t ChanCalibDataSet(int16_t *BuffIn,CHAN_DATA *g_ChanData_ST_p)
{
		
	// uint8_t i_U8 = 0,j= 0;
	// /*frame_data assgin value*/
	// //should optimize
	// for(j = 0;j < VOL_CHAN_NUM;j++)
	// { 

 //  		BuffIn[2 * j] = g_ChanData_ST_p -> CurrChan_ST[j].CH_Voltage_I16;
 //  		if(EADS_DEBUG  == g_DebugType_EU)
 // 		{
 // 			printf("j %d, VOlBuffer %d\n",2 * j,BuffIn[2 * j]);
 // 		}	
 // 		BuffIn[1 + 2 * j] = g_ChanData_ST_p -> CurrChan_ST[j].CH_Current_I16;
 // 		if(EADS_DEBUG  == g_DebugType_EU)
 // 		{
 // 			printf("j %d, CurrBuffer %d\n",1+2 * j,BuffIn[1 + 2 * j]);
 // 		}

	// }
	// for(j = 0;j < VOL_CHAN_NUM;j++)
	// { 

 //  		BuffIn[50 + j] = g_ChanData_ST_p -> VolChan_ST[j].CH_Voltage_I16;
 //  		if(EADS_DEBUG  == g_DebugType_EU)
 // 		{
 // 			printf("j %d, VolBuffer %d\n",50 + j,BuffIn[50 + j]);
 // 		}		
 //    }
	// return 0;
}
/**********************************************************************
*Name			:   int8_t ChanCalibDataSave(CHAN_DATA *CalibDataSave_p) 
*Para     		:   CHAN_DATA *CalibDataSave_p :the Calib data Save ST 
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/6/29  Create
*********************************************************************/
int8_t ChanCalibDataSave(CHAN_DATA *CalibDataSave_p)
{
 //    int16_t CalibData[100] = {0};
 //    int8_t Fwrtn = 0;
 //    ChanCalibDataSet(CalibData,CalibDataSave_p);
 //    if(EADS_DEBUG  == g_DebugType_EU)
 //    {
 //    	printf("DriveLRbuf\n");   	
    	
 //    }
 //    //this should note the 2 parameterW
 //    Fwrtn = fwrite(&CalibData,2,VOL_CHAN_NUM * 2 + VOL_CHAN_NUM,g_fd_ChanCalibName); 
	// if(Fwrtn)
	// {
	// 	return 0;
	// }
	// else
	// {
	// 	return -1;
	// }	
}
/**********************************************************************
*Name			:   int8_t RealFileCreat(uint8_t ChanName)
*Function       :   Creat a new .dat file by new hour,the file name is time
*Para     		:   void 
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*REV1.0.1     feng    2018/5/9  Not save valut for EADS test

*********************************************************************/
int8_t RealFileCreat(uint8_t ChanName)
{
	// //FILE *fd;
	// uint8_t File_RealRecName_U8[200] = {0};
	// uint8_t File_Directory_U8[200] = {0};
	// uint8_t TimeString_U8[40] = {0};
	// uint32_t TotalSizeMB_U32 = 0,FreeSizeMB_U32 = 0;
	// time_t timep_ST;   
 //    struct tm *now_time_p_ST;
	// time(&timep_ST);
	// now_time_p_ST = localtime(&timep_ST); /*change to the local time*/
	     
	// sprintf(TimeString_U8,"%04d%02d%02d%02d%02d%02d",
	//    		    (1900 + now_time_p_ST->tm_year), 
	//    		    (1 + now_time_p_ST->tm_mon), now_time_p_ST->tm_mday,
	//    		    now_time_p_ST->tm_hour, now_time_p_ST->tm_min, now_time_p_ST->tm_sec); 
  
 //    /*  //without year month day folder

 // 	sprintf(File_Directory_U8,"%s",&g_Rec_XML_ST.Rec_Real_ST.RecPath);
 //    MultiDircCreate(File_Directory_U8);
 //    sprintf(File_RealRecName_U8,"%s%s%04d_%02d_CH_%d-%s.dat",&g_Rec_XML_ST.Rec_Real_ST.RecPath,
	// 	&g_Rec_XML_ST.Rec_Real_ST.RecFileHead,g_TrainInfo_ST.TrainName_U8,g_file_CoachName_U8,ChanName,TimeString_U8);
	// */
 //    //with year month day folder
 // 	sprintf(File_Directory_U8,"%s%0.8s",&g_Rec_XML_ST.Rec_Real_ST.RecPath[0],TimeString_U8);
 //    MultiDircCreate(File_Directory_U8);
	// sprintf(File_RealRecName_U8,"%s%0.8s/%s%04d_%02d_CH_%d_%0.8s-%s.dat",&g_Rec_XML_ST.Rec_Real_ST.RecPath[0],
	// 	TimeString_U8,&g_Rec_XML_ST.Rec_Real_ST.RecFileHead,g_TrainInfo_ST.TrainName_U16,g_file_CoachName_U8,ChanName,TimeString_U8,&TimeString_U8[8]);
	// g_fd_RealRecName[ChanName] = fopen(File_RealRecName_U8, "a+"); 
	// if(g_fd_RealRecName[ChanName] == NULL)
	// {
	// 	perror("power on creat RealRec.dat file failed");
	// 	return -1;
	// 		/*exit(1)*/
	// }
	// else
	// {
	// 	//printf("Create file %s success\n", File_RealRecName_U8);
	// }
	// DriveRealFileTopSave(ChanName);
	// return 0;
}     
/**********************************************************************
*Name			:   int8_t EventDataSet(DRIVE_FILE_DATA  *DriveEvent_ST_p,
*                   CHAN_DATA *g_ChanData_ST_p)//uint16_t length
*Function       :   Set the Event data of 6 coach  
*Para     		:   DRIVE_FILE_DATA  *DriveEvent_ST_p   the address of Save DriveEventST
*                   CHAN_DATA *g_ChanData_ST_p   the address of Save g_ChanData_ST
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng

*History:
*REV1.0.0     feng    2018/8/29  Create
*********************************************************************/
int8_t EventDataSet(DRIVE_FILE_DATA  *DriveEvent_ST_p,CHAN_DATA *ChanData_ST_p,uint8_t EADSType)//uint16_t length
{
		
	uint8_t j= 0;
	for(j = 0;j < VOL_CHAN_NUM;j++)
	{ 
 
        DriveEvent_ST_p -> DriveAnalog_U16[j] = ChanData_ST_p -> VolChan_ST[j].CH_Voltage_I16 ;
	
	}
	if(EADSType == CTU_BOARD_ID)
	{
		for(j = 0;j < CURR_CHAN_NUM;j++)
		{ 
 
       	 	DriveEvent_ST_p -> DriveAnalog_U16[j + VOL_CHAN_NUM] = ChanData_ST_p -> CurrChan_ST[j].CH_Current_I16;
			       	 	    if(g_DebugType_EU == EADS_DEBUG)
			    {
			    	printf("DriveAnalog_U16 %d : %d \n",j + VOL_CHAN_NUM,DriveEvent_ST_p -> DriveAnalog_U16[j + VOL_CHAN_NUM]);

			    }
	
		}
        
	}
	return 0;
}

/**********************************************************************
*Name			:   int8_t EventDataSave(uint8_t ChanNumTmp)
*Function       :   Save the Event data of 6 coach to file 
*Para     		:   uint8_t ChanNumTmp   the Filt channel number,from ChanNumTmp save one frame

*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng

*History:
*REV1.0.0     feng    2018/8/29  Create
*********************************************************************/
int8_t EventDataSave(FILE_FD * file_p, uint8_t ChanNumTmp,uint8_t EADSType)
{
    uint8_t i;
    int16_t fwerr = 0;
    DRIVE_FILE_DATA DriveEventData_ST[6] = {0};
	ChanDataFilt(&g_ChanData_ST[0],&g_ChanDataFilt_ST,ChanNumTmp,EADSType);
    // if(g_DebugType_EU == EADS_DEBUG)
    // {
    // 	printf("EventFile Chan DATA \n");
    // 	ChanDataPrint(&g_ChanDataFilt_ST,EADSType);//uint16_t length
    // }
    EventDataSet(&DriveEventData_ST[0],&g_ChanDataFilt_ST,EADSType);
    //when save digital data,should change the DriveEventData_ST[0].Analog[0]
    fwerr = fwrite(&DriveEventData_ST[0].DriveAnalog_U16[0],g_Rec_XML_ST.Rec_Event_ST.RecChnNum_U8 * 2,1,file_p -> EventFile_fd); 

    if(fwerr)
    {
	     return 0;
    }
    else
    {
    	return -1;
    }

}

/**********************************************************************
*Name			:  uint8_t DriveLRDataSet(DRIVE_FILE_DATA  *DriveEvent_ST_p,CHAN_DATA *g_ChanData_ST_p)//uint16_t length
*Function       :   save the LR value to a LR file,use the Analog word  
*Para     		:   FRAME_DATA *FrameData_ST_p   the address of FRAME_DATA
*                   CHAN_DATA *g_ChanData_ST_p
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*REV1.0.1     feng    2018/02/02  Change (FRAME_DATA *FrameData_ST_p) to (FRAME_SAVE *FrameWaveSave_ST_p)
*********************************************************************/
int8_t DriveOperNumDataSet(DRIVE_FILE_OPRTNUM_DATA  *DriveOpeNum_ST_p,uint32_t *ChanOpeNum_p)//uint16_t length
{
		
	uint16_t i = 0,j= 0;
	uint32_t ChanOpNumTemp = 0;
    for(j = 0;j < VOL_CHAN_NUM;j++)// g_Rec_XML_ST.Rec_Fast_ST.RecChnNum_U8 - 1 ,because ChanOpeNum_P is 75
	{ 
        ChanOpNumTemp = *ChanOpeNum_p;
        DriveOpeNum_ST_p -> DriveAnalog_U16[i] = ChanOpNumTemp & 0xFFFF;  //one chanOpNum took 2 word
        i++;
	    DriveOpeNum_ST_p -> DriveAnalog_U16[i] = (ChanOpNumTemp >> 16)& 0xFFFF;
	    i++;
        ChanOpeNum_p++;
	}
	return 0;
}
/**********************************************************************
*Name			:   int8_t ChanOperNumFileCreat(void)
*Function       :   Creat the OperaNum file of 6 Coach
*Para     		:   void 
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/7/29  Create
*********************************************************************/
int8_t ChanOperNumFileCreat(FILE_FD * File_p)
{

 //    File_p-> OperNumFile_fd = fopen(CHAN_OPER_NUM_FILE, "w+"); //every time creat the file and write replace
	// if(File_p-> OperNumFile_fd == NULL)
	// {
	// 	perror("creat ChanOpeNumName.dat file failed");
	// 	return -1;
	// }
	// DriveOperNumFileTopSave(File_p);
	// return 0;
}
/**********************************************************************
*Name			:   int8_t DriveOperNumDataSave(uint32_t *ChanOperNum_p)
*Function       :   Save the  OperaNum  data to file of 6 Coach
*Para     		:   uint32_t *ChanOperNum_p,the g_OperNum_U32
*
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/7/29  Create
*********************************************************************/
int8_t DriveOperNumDataSave(FILE_FD * filep, uint32_t *ChanOperNum_p)
{
    uint8_t i;
    DRIVE_FILE_OPRTNUM_DATA DriveOperNumData_ST = {0};
    DriveOperNumDataSet(&DriveOperNumData_ST,ChanOperNum_p);
    if(EADS_DEBUG  == g_DebugType_EU)
    {
    	printf("DriveOperNumbuf\n");   	
    	
    }
    //this should note the 2 parameter
    fseek(filep -> OperNumFile_fd,sizeof(DRIVE_FILE_TOP),SEEK_SET);
    fwrite(&DriveOperNumData_ST.DriveAnalog_U16[0],g_Rec_XML_ST.Rec_Fast_ST.RecChnNum_U8 *4,1,filep -> OperNumFile_fd); 
    fclose(filep -> OperNumFile_fd); // to make every time save the data to file
	return 0;
}

/**********************************************************************
*Name			:   int8_t OperNumFileSave(void)
*Function       :   Creat  the operateNum file,and save Opearate Num
*Para     		:   
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*s
*History:
*REV1.0.0     feng    2018/2/27  Create
*********************************************************************/
int8_t OperNumFileSave(FILE_FD * File_p, CHAN_STATUS_INFO * ChanStaInfo_p)
{
    int8_t err = 0;
    ChanOperNumFileCreat(File_p);
    err = DriveOperNumDataSave(File_p,&ChanStaInfo_p -> Channel_OperaNum_U32[0]);
    if(err != 0)
    {
    	printf("OperNumFileSave  error\n");
    	return -1;
    }
    return 0;
}

/**********************************************************************
*Name			:   int8_t DriveRealDataSet(DRIVE_FILE_DATA *DriveEvent_ST_p,CHAN_DATA *g_ChanData_ST_p,uint8_t RealChnName)
*Function       :   Set the menber value of  DriveAnalog_U16 
*Para     		:   DRIVE_FILE_DATA *DriveEvent_ST_p
*                   CHAN_DATA *g_ChanData_ST_p
*                   uint8_t RealChnName   save which Chan
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*********************************************************************/
int8_t DriveRealDataSet(DRIVE_FILE_DATA *DriveEvent_ST_p,CHAN_DATA *g_ChanData_ST_p,uint8_t RealChnName)//uint16_t length
{
		
	uint8_t i_U8 = 0,j= 0; 
	//
	if(RealChnName < VOL_CHAN_NUM)
	{
		j = RealChnName;
		DriveEvent_ST_p -> DriveAnalog_U16[0] = g_ChanData_ST_p -> VolChan_ST[j].CH_Voltage_I16;	
			
	}
	//in the drive config,the real wave select  2 analog channel
	else
	{
		j = RealChnName - VOL_CHAN_NUM;  /*RealChnName is between 0 ~ 54*/
		DriveEvent_ST_p -> DriveAnalog_U16[0] = g_ChanData_ST_p -> CurrChan_ST[j].CH_Current_I16 ;

	} 
	return 0;
}

/**********************************************************************
*Name			:   int8_t DriveRealDataSave(uint8_t ChanName,uint8_t ChanNumTmp)
*Function       :   Save the Real data to of File  
*Para     		:   uint8_t ChanName   save which Chan
*                   uint8_t ChanNumTmp the frame number of real data 
*Return    		:   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/7/29  Create
*********************************************************************/
int8_t DriveRealDataSave(uint8_t ChanName,uint8_t ChanNumTmp)
{
 //    uint8_t i;
 //    DRIVE_FILE_DATA DriveRealData_ST[6] = {0};
	// for (i = 0; i < ChanNumTmp; i++)
	// {
	// 	DriveRealDataSet(&DriveRealData_ST[0],&g_ChanData_ST[i],ChanName);
	// 	fwrite(&DriveRealData_ST[0].DriveAnalog_U16[0],4,1,g_fd_RealRecName[ChanName]); 
		
	// }
	// return 0;
}



int8_t RealFileProc(void)
{	
	// channel happen warm
	/*if(g_ChanStatuInfo_ST.RealRecFlag_U8)
	{
		g_ChanStatuInfo_ST.RealRecFlag_U8 = 0; 
		for(main_i = 0;main_i < ALL_CHAN_NUM;main_i++)
		{					
			if(1 == g_channel_RealRecorChanName[main_i]) g_ChanStatuInfo_ST.RealRecorChanName[main_i]
			{
			   //have creat file
				if(1 == g_channel_RealRecStartFlag[main_i])
				{
					// not reach the max Rec total number
					if(g_frame_RealRecorNum_U32[main_i] <= g_Rec_XML_ST.Rec_Real_ST.RecToTalNum)
					{
						DriveRealDataSave(main_i,g_ProcNum_U8);
						g_frame_RealRecorNum_U32[main_i] += g_ProcNum_U8;
					}
					//reach the max Rec total number,close file and creat new
					else
					{								
						fclose(g_fd_RealRecName[main_i]);
						g_channel_RealRecFileNum--;
						g_channel_RealRecStartFlag[main_i] = 0;
						g_frame_RealRecorNum_U32[main_i] = 0;
						// RealFileCreat(main_i);							   
					}
				}
				//run at first time and not have creat file
				else if(0 == g_file_RealSaveNum_U16[main_i])
				{
					RealFileCreat(main_i); //at first creat warm file,save the data before warm
					//g_file_RealSaveNum_U16[main_i]++;
					RealFileSaveBeforWarm(main_i);
					g_frame_RealRecorNum_U32[main_i] += g_Rec_XML_ST.Rec_Real_ST.RecBeforeNum;
					g_channel_RealRecStartFlag[main_i] = 1;   //mean creat the file
					g_channel_RealRecFileNum++;//the number of real rec file
					RealFileNum++;//
					g_file_RealSaveNum_U16[main_i] = 1; //mean have creat the file ,to avoid creat repeate warm file					 
				}					  
			}
			//jump into is the channel is normal, but another channel is warm
			else 
			{
				g_file_RealSaveNum_U16[main_i] = 0; //warm happen again,to creat file
				RealFileNum--;		  
			}
		}						 
	}
	//creat Real file and  g_ChanStatuInfo_ST.RealRecFlag_U8 clear
	else if(g_channel_RealRecFileNum > 0)
	{
		for(main_i = 0;main_i < ALL_CHAN_NUM;main_i++)
		{
			if(1 == g_channel_RealRecStartFlag[main_i])//mean have creat the file
			{
				if(g_frame_RealRecorNum_U32[main_i] <= g_Rec_XML_ST.Rec_Real_ST.RecToTalNum)
				{
					DriveRealDataSave(main_i,g_ProcNum_U8);
					g_frame_RealRecorNum_U32[main_i] += g_ProcNum_U8;
				}
				else 
				{
					fclose(g_fd_RealRecName[main_i]);
					g_frame_RealRecorNum_U32[main_i] = 0;
					g_channel_RealRecStartFlag[main_i] = 0;
					g_file_RealSaveNum_U16[main_i] = 0;
					g_channel_RealRecFileNum--;
					RealFileNum--;
				}						
			}
		}				
	}
	else if(RealFileNum > 0)//to clear have creat file flag
	{
		for(main_i = 0;main_i < ALL_CHAN_NUM;main_i++)
		{
			if(1 == g_file_RealSaveNum_U16[main_i])
			{
				g_file_RealSaveNum_U16[main_i] = 0; //warm happen again,to creat file
				RealFileNum--;	
			}
		}			  
	}  */

}




