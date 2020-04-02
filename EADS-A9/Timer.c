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
*********************************************************************/

/**********************************************************************
*
*Include file  section
*
*********************************************************************/
#include "Timer.h"

/**********************************************************************
*
*Function Define Section
*
*********************************************************************/
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/
extern DEBUG_TYPE_ENUM g_DebugType_EU;
/**********************************************************************
*Name			:   void set_timer(uint8_t second,uint32_t usecond)  
*Function       :   Writ U32bit value to the phyaddr of Bram
*Para     		:   uint64_t phyaddr  the addres of Bram
*                   uint32_t val    the write value
*Return    		:   void.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*********************************************************************/
  
int8_t set_timer(uint32_t second,uint32_t usecond)  
{  
    struct itimerval itv;  
    itv.it_interval.tv_sec = second;  //设置为1秒
    itv.it_interval.tv_usec = usecond;  
    itv.it_value.tv_sec = second;  
    itv.it_value.tv_usec = usecond;  
    setitimer(ITIMER_REAL, &itv, NULL);  //此函数为linux的api,不是c的标准库函数
}  
/**********************************************************************
*Name			:   int8_t SetSystemTime(SYSTEM_TIME *DatStr)   
*Function       :   Set the Linux System Time
*Para     		:   SYSTEM_TIME *DatStr 
*Return    		:   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*********************************************************************/
int8_t SetSystemTime(SYSTEM_TIME *DatStr)  
{  
 
    uint16_t Year_U16;
    struct tm Time_ST;  
    struct timeval TimeVal_ST;  
    time_t timep;  
    Year_U16 = (DatStr -> Year_U8[0] << 8) + DatStr -> Year_U8[1];
    Time_ST.tm_year = Year_U16 - 1900; 
    Time_ST.tm_mon = DatStr -> Month_U8 - 1;
    Time_ST.tm_mday = DatStr -> Day_U8;
    Time_ST.tm_hour = DatStr -> Hour_U8;
    Time_ST.tm_min = DatStr -> Minute_U8;       
    Time_ST.tm_sec =DatStr -> Second_U8;  
  
    timep = mktime(&Time_ST);  
    TimeVal_ST.tv_sec = timep;  
    TimeVal_ST.tv_usec = 0;  
    if(settimeofday (&TimeVal_ST, (struct timezone *) 0) < 0)  
    {  
	    printf("Set system time Failed!\n");  
	    return -1;  
    }
    else
    {
    	printf("Set system time Success\n"); 
    }
    if(TIME_DEBUG == g_DebugType_EU)
    {
  
        printf("system time String %s\n",DatStr);
    }  
    return 0;  
}    



int threadDelay(const long lTimeSec, const long lTimeUSec)
{
    struct timeval timeOut;
    timeOut.tv_sec = lTimeSec;
    timeOut.tv_usec = lTimeUSec;
    if (0 != select(0, NULL, NULL, NULL, &timeOut))
    {
        return 1;
    }
    return 0;
}
    