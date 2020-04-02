/**********************************************************************
*File name 		:   main.c
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/1/29
*Description    :   Creat a file,save the setvalue once and save the data
*                  20180224: 
*                   1.simulater bram data
*	                2. judge vol and current, set the channel status
*	                3. Calutulater LRtime and operatureNum
*	                4.save 5 frame to file and save the g_ChanData_ST to FrameSave_ST
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*REV1.0.1     feng    2018/02/20 Record eventfile and waverecord
*REV1.0.2     feng    2018/03/06 Record eventfile, waverecord by fwrite 
*                      Data have 25 channel with VI and 50 channel with V
*Rev1.0.3     feng    2018/08/08 mmap all block address at first                      
*
*********************************************************************/


/**********************************************************************
*
*Include file  section
*
*********************************************************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include "DataHandle.h"
#include "BaseDefine.h"
#include "Socket.h"
#include "BramDataProc.h"
#include "ADCDataProc.h"
#include "ModbusSer.h"
#include "Timer.h"
#include "GPIOControl.h"
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/

struct sigaction Alarm_act;

//20200325
BLVDS_BRD_DATA g_BrdRdBufData_ST = {0};
BLVDS_BRD_DATA g_BrdRdData_ST = {0};
CHAN_DATA  g_ChanData_ST[CHAN_BUFFER_NUM] = {0};  /*304 byte*/
CHAN_DATA  g_ChanRealBuf_ST[REAL_BUFFER_NUM] = {0};  /*REAL_BUFFER_NUM = 800*/
CHAN_DATA  g_ChanSendBuf_ST[CHAN_BUFFER_NUM] = {0};  /*304 byte*/
CHAN_DATA  g_ChanDataFilt_ST = {0}; /*the event save with filt*/
CHAN_CALIB_DATA  g_ChanCalib110VData_ST = {0};/*read the ChanCalibData from /tffs0/ChanCalibValue.dat"*/
CHAN_STATUS_INFO  g_ChanStatuInfo_ST = {0};/*include the real file save,Chan operate num*/

TRAIN_INFO g_TrainInfo_ST = {0};
VERSION  g_Version_ST = {0};

FILE_FD   g_FileFd_ST = {0};/*the save file fd*/
RECORD_XML g_Rec_XML_ST = {0};
/// TRDP_DATA g_Trdp_SendData_ST = {0};

FILE_SAVE_INFO g_FileSaveInfo_ST = {0};

modbus_t *g_ModbusCtx = NULL;
PTHREAD_INFO g_Pthread_ST = {0};


uint32_t g_LoopNum_U32 = 0;
uint8_t g_ProcNum_U8 = 0;
DEBUG_TYPE_ENUM g_DebugType_EU;/*g_DebugType_EU == 2,print time to test*/

uint32_t g_ADReadSleep_U32 = 0;
uint32_t g_EventSaveSlepNum_U32 = 0;
uint32_t g_TRDPUsec_U32 = 0;
uint8_t g_EADSType_U8 = 0;

int8_t  g_gpio_Rs485 = 0; //the sysfile fd
int8_t g_fd_TtyPs = 0;                            //文件描述符  

int8_t  g_gpio_PowDown = 0;
int8_t g_LifeFlag = 1;//to controll the pthread
int8_t g_EADSType = 0;

uint32_t g_frame_EventNum_U32;  /*the save number of event data */
uint8_t g_ChanRealWave[48] = {0}; /*for the channel real wave */
uint8_t g_ChanRealNum = 0;  /*for the channel real wave */

uint8_t g_socket_SendFlag = 0; /*for tcp modbus comunicate*/

sem_t g_ReadBrd_sem;
sem_t g_RealSend_sem;

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
void FuncUsage(void);
void *DataReadThreaFunc(void *arg); 
void *RealWaveThreadFunc(void *arg);
void *TRDPThreadFunc(void *arg);
void *ModbusThreadFunc(void *arg);

void *FileSaveThreaFunc(void *arg);
void * LEDBilnkPthread (void *arg);

int8_t ThreadInit(PTHREAD_INFO * pthread_ST_p);
int8_t  ThreadOff(FILE_FD * FileFd_p,PTHREAD_INFO  * pthread_ST_p);

/**********************************************************************
*
*Static Variable Define Section
*
*********************************************************************/
int main(int argc, char *argv[])
{	

    uint16_t main_i = 0,main_j = 0;
   
    struct timeval Time_ST,TimeEnd_ST;
    /*for EADS test*/

    int8_t res=0;

    uint8_t PowDowIOValue = 0;

    g_LifeFlag = 1;//PowerDown clear


   // SocketConnect(int8_t serverfd_I8)
   
    g_TrainInfo_ST.TrainNum_U16 = 4085;

    uint16_t PowDebug = 0;
	
    if (argc < 8)
    {
    	FuncUsage();
    }


    g_TrainInfo_ST.CoachNum_U8 = (uint32_t)strtoul(argv[1], NULL, 0);
	g_ProcNum_U8 =(uint8_t)strtoul(argv[2], NULL, 0);
	g_DebugType_EU =  (DEBUG_TYPE_ENUM)strtoul(argv[3], NULL, 0);
    g_ADReadSleep_U32 = (uint16_t)strtoul(argv[4], NULL, 0);
    g_EventSaveSlepNum_U32 = (uint32_t)strtoul(argv[5], NULL, 0);
    PowDebug = (uint16_t)strtoul(argv[6], NULL, 0);
    g_TRDPUsec_U32 = (uint32_t)strtoul(argv[7], NULL, 0);

    g_EADSType_U8 = g_TrainInfo_ST.CoachNum_U8;

    if(COACHZP == g_TrainInfo_ST.CoachNum_U8)
    {
        g_EADSType_U8 = CTU_BOARD_ID;
    }
    else
    {
        g_EADSType_U8  = ADU_BOARD_ID;
    }
     
    BLVDSReadInit(g_EADSType_U8);
    printf("g_TrainInfo_ST.CoachNum_U8 %d\n",g_TrainInfo_ST.CoachNum_U8);
    printf("g_ProcNum_U8 %d\n",g_ProcNum_U8);
    printf("g_DebugType_EU %d\n",g_DebugType_EU);
    printf("g_ADReadSleep_U32 %d\n",g_ADReadSleep_U32);
    printf("g_EventSaveSlepNum_U32 %d\n",g_EventSaveSlepNum_U32);
    printf("PowDebug %d\n",PowDebug);
    printf("g_TRDPUsec_U32 %d\n",g_TRDPUsec_U32);
    if(g_TrainInfo_ST.CoachNum_U8 == 0)
    {
       printf("g_TrainInfo_ST.CoachNum_U8 %d\n,error",g_TrainInfo_ST.CoachNum_U8); 
       exit(0);
    }
    else
    {
       
	    //// OperNumFileSave();//creat Chan Oper Num file;
        printf("DataProcPowerOn\n");
        DataProcPowerOn(&g_ChanStatuInfo_ST,&g_ChanCalib110VData_ST,g_ProcNum_U8,g_EADSType_U8);
        FileCreatePowOn(&g_FileFd_ST,&g_Rec_XML_ST,&g_TrainInfo_ST);

        ThreadInit(&g_Pthread_ST);
	    ////  DriveLRFileTopSave();
       ///// GPIO_SysFlOpen();
        /////  GPIO_PowDowOpen(); 
        while(1)
        {                         
            //FileCreateByTime();
            res = sem_wait(&g_ReadBrd_sem);
            if (res != 0) 
            {
                printf("sem_wait errno %d\n",errno);
                
            }
            if(g_DebugType_EU == SEM_DEBUG)
            {
                printf("g_ReadBrd_sem wait %d\n",g_ReadBrd_sem);               
            }
            if(g_DebugType_EU ==  EADS_DEBUG)
            {
                printf("Start  EADSDataProc\n");
            }
            EADSDataProc(g_ProcNum_U8,g_EADSType_U8);         
            EADSDataBuf(g_ProcNum_U8);
            if(g_socket_SendFlag == 1)
            {   
                sem_post(&g_RealSend_sem);
                if(g_DebugType_EU == SEM_DEBUG)
                {
                    printf("g_RealSend_sem post %d\n",g_RealSend_sem);               
                }
            }
            /**************
            g_frame_PowerDownNum_U32++;
            if(g_frame_PowerDownNum_U32 >= POWOFF_SAVE_NUM)//normal save
            {
                //DriveOperNumDataSave(g_channel_OperaNum_U32);
                printf("OperNumFileSave\n");
                OperNumFileSave();                
                g_frame_PowerDownNum_U32 = 1;
            }
            //g_LoopNum_U32--;
            if(TIME_DEBUG == g_DebugType_EU)
            {
                gettimeofday(&TimeEnd_ST,NULL);
                printf("Save tim:%d \n",TimeEnd_ST.tv_usec- Time_ST.tv_usec);

            } *********/ 
        }
		/*********
        if(g_channel_RealRecFileNum)
        {
            for(main_i = 0;main_i < ALL_CHAN_NUM;main_i++)
            {
                if(1 == g_channel_RealRecStartFlag[main_i])
                {
                    
                    fclose(g_fd_RealRecName[main_i]);                     
                }
            }               
        }**************/
        printf("Waiting for threads to finish...\n");

		ThreadOff(&g_FileFd_ST,&g_Pthread_ST);
        BramCloseMap();
        fclose(g_FileFd_ST.EventFile_fd);
        printf("All done\n");
    }
    
}


void FuncUsage(void)
{
    printf("For example:\n");
    printf("./EADS_Demo..<g_coachName> <Update Tartget)> <DebugTimeTest_EU>\n");        
    printf(" <g_ADReadSleep_U32> <g_EventSaveSlepNum_U32> <PowDebug>\n");
    printf(" <g_RTUBaud_U32> <g_TRDPUsec_U32> <g_TrdpTarget_U8> <g_DMU_Senor_num>\n");  
    exit(0);
}


int8_t ThreadInit(PTHREAD_INFO      * pthread_ST_p)
{
       int8_t res=0;

		res = sem_init(&g_ReadBrd_sem, 0, 0);
        if(g_DebugType_EU == SEM_DEBUG)
        {
            printf("g_ReadBrd_sem init %x\n",g_ReadBrd_sem);               
        }
        if (res != 0) 
        {
            perror("ReadMAXData Semaphore initialization failed");
            exit(EXIT_FAILURE);
        }
        res = sem_init(&g_RealSend_sem, 0, 0);
        if(g_DebugType_EU == SEM_DEBUG)
        {
            printf("g_RealSend_sem init %x\n",g_RealSend_sem);               
        }
        if (res != 0) 
        {
            perror("SendData Semaphore initialization failed");
            exit(EXIT_FAILURE);
        }
		/*Create the Pthread*/
        res = pthread_create(&pthread_ST_p -> ReadDataThread, NULL, DataReadThreaFunc, NULL);
        if (res != 0) 
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
        res = pthread_create(&pthread_ST_p -> RealWaveThread, NULL, RealWaveThreadFunc, NULL);
        if (res != 0) 
        {
            perror("RealWaveThreadFunc creation failed");
            exit(EXIT_FAILURE);
        }
        
        // // res = pthread_create(&pthread_ST_p -> TRDPThread, NULL, TRDPThreadFunc, NULL);
        // // if (res != 0) 
        // // {
        // //     perror("TRDPThreadFunc creation failed");
        // //     exit(EXIT_FAILURE);
        // // }
 
        res = pthread_create(&pthread_ST_p -> FileSaveThread, NULL, FileSaveThreaFunc, NULL);
        if (res != 0) 
        { 
            perror("FileSaveThreaFun creation failed");
            exit(EXIT_FAILURE);
        }
        res = pthread_create(&pthread_ST_p -> LedThread, NULL, LEDBilnkPthread, NULL);
        if (res != 0) 
        { 
            perror("FileSaveThreaFun creation failed");
            exit(EXIT_FAILURE);
        }
        pthread_detach(pthread_ST_p -> RealWaveThread);
        //// pthread_detach(TRDPThread);
}


int8_t  ThreadOff(FILE_FD * FileFd_p,PTHREAD_INFO  * pthread_ST_p)
{
    void *thread_result;
	int8_t res=0;

	res = pthread_join(pthread_ST_p -> FileSaveThread, &thread_result);
	if (res == 0)
	{
		printf("thread_join FileSaveThread success\n");
	}
	else 
	{
		perror("pthread_join FileSaveThread failed\n"); 		  
	}
	res = pthread_join(pthread_ST_p ->ReadDataThread, &thread_result);
	if (res == 0)
	{
		printf("thread_join ReadDataThread success\n");
	}
	else 
	{
		perror("pthread_join ReadDataThread failed\n"); 		  
	}

    res = pthread_join(pthread_ST_p -> LedThread, &thread_result);
    if (res == 0)
    {
        printf("thread_join LedThread success\n");
    }
    else 
    {
        perror("pthread_join LedThread failed\n");           
    }
	sem_destroy(&g_ReadBrd_sem);
	sem_destroy(&g_RealSend_sem); 

}



void *DataReadThreaFunc(void *arg) 
{
    struct timeval A_Time_ST,A_TimeEnd_ST;
    uint8_t i = 0;
	uint8_t BLVDSReadNum = 0;
    while(g_LifeFlag > 0)
    {

        if(g_DebugType_EU == SEM_DEBUG)
        {
            gettimeofday(&A_Time_ST,NULL);
        }        
        threadDelay(0,g_ADReadSleep_U32);
		BLVDSDataReadThreaFunc(BLVDSReadNum,g_EADSType_U8);
		BLVDSReadNum++;	
        if(BLVDSReadNum == g_ProcNum_U8)
        {    
            
            BLVDSReadNum = 0;
            if(g_DebugType_EU ==  EADS_DEBUG)
            {
                printf("ReadThreaFunc finish \n");
            }
            sem_post(&g_ReadBrd_sem);

            if(g_DebugType_EU == SEM_DEBUG)
            {
                printf("g_ReadBrd_sem post %x\n",g_ReadBrd_sem);               
            }
        }
       if(g_DebugType_EU == SEM_DEBUG)
        {
            
            gettimeofday(&A_TimeEnd_ST,NULL);
             //printf("Time %d\n",A_TimeEnd_ST.tv_usec );
            printf("g_ReadBrd_sem Time %d\n", (A_TimeEnd_ST.tv_usec - A_Time_ST.tv_usec));
        }

    }
    printf("exit DataRead thread\n");
    pthread_exit(NULL);
}


int RealthreadDelay(const long lTimeSec, const long lTimeUSec)
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

void *RealWaveThreadFunc(void *arg)
{
    int total;
    int i,j;
    int serverfd,clientfd;
    int16_t WriteSize = 0;
    int16_t ReadSize = 0;
    char readbuf[100];
    uint8_t  returnbuf[100]={0};
    
    struct sockaddr_in server,client;
    struct sigaction action;
    // action.sa_handler = handle_pipe;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    // sigaction(SIGPIPE, &action, NULL);
    action.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &action, 0);

    if(-1==( serverfd= socket(AF_INET,SOCK_STREAM,0)))
    {
        perror("socket error\n");
        exit(1);
    }
    bzero(&server,sizeof(server));

    server.sin_family=AF_INET;
    server.sin_port=htons(TCP_PORT);
    server.sin_addr.s_addr=INADDR_ANY;//inet_addr(ip);
    int opt = 1;
    setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if(-1 == bind(serverfd,(struct sockaddr *)&server,sizeof(server)))
    {
        perror("bind error\n");
        exit(1);
    }

    if(-1 == listen(serverfd,10))
    {
        perror("listen error\n");
        exit(1);
    }
    if(g_DebugType_EU == TCP_DEBUG)
    {
        printf("sart listen....r...........\n");  
    }

    socklen_t len = sizeof(client);
    bzero(&client,len); 
    while(g_LifeFlag > 0)
    {

        if(-1==(clientfd = accept(serverfd,(struct sockaddr *)&client,&len)))
        {
            perror("accept error\n");
            close(serverfd);
            return ;
        }
        g_socket_SendFlag  = 1;
        if(EADS_DEBUG == g_DebugType_EU)
        {
                
            printf(" clientfd : %d\n",clientfd);
            printf(" client port : %d \n",ntohs(client.sin_port));  
        }
        printf(" communicate succeeded\n");
        if(read(clientfd ,readbuf,sizeof(readbuf))>0)
        {            
            if(TCP_DEBUG == g_DebugType_EU)
            {
                for (i=0;i<20;i++)
                {
                    printf("%d,data %02x \n",i,readbuf[i]);
                } 
            }

            
            //clear for next wave connect and send data.
            memset(g_ChanRealWave,0,sizeof(g_ChanRealWave));
            g_ChanRealNum = 0;
            RealTimeWaveExtr(readbuf,g_ChanRealWave,&g_ChanRealNum);
        }
        while(g_socket_SendFlag)
        {
        
            //printf("g_sendNum %d \n",g_sendNum);
            
            // memset(readbuf,0,sizeof(readbuf));
            // ReadSize = read(fd,readbuf,sizeof(readbuf));
         //    printf(" ReadSize %d,data %02x \n",ReadSize,readbuf[0]);
            //1 chan real Num == 2 byte
            //
            sem_wait(&g_RealSend_sem);
            if(g_DebugType_EU == SEM_DEBUG)
            {
                printf("g_RealSend_sem wait %x\n",g_RealSend_sem);               
            }
            for(i =0;i < g_ProcNum_U8;i++)
            {
                memset(returnbuf,0,sizeof(returnbuf));
                RealWaveData(returnbuf,g_ChanRealWave,&g_ChanSendBuf_ST[i],g_ChanRealNum);
                if(TCP_DEBUG == g_DebugType_EU)
                {
                        for(j = 0;j < 2 * g_ChanRealNum;j++)
                        {                           
                            printf("%d,data %02x \n",j,returnbuf[j]);                      
                            //printf("i %d g_ChanRealWave %d \n",i,g_ChanRealWave[i]);
                        } 
                }
                WriteSize = write(clientfd,returnbuf,2 * g_ChanRealNum);               
                if(WriteSize > 0)
                {
                    //usleep(g_EventNum_U32);
                }
                //when Drive disconnect ,break out
                else if(WriteSize == 0)
                {
                     printf("client close...\n");
                     g_socket_SendFlag  = 0;
                    //close(fd);               
                    break;
                }
                //when Drive disconnect ,break out,WriteSize is -1
                else
                {
                    printf("WriteSize %d\n",WriteSize);
                    printf("client error...\n");
                    g_socket_SendFlag  = 0;
                    break;
                }    
            }          
        }
        printf("send complete,close clientfd\n");
        close(clientfd);
        
    }
    printf("------------>server close\n");
    close(serverfd);
    pthread_exit(NULL);
    return 0;
        
}


void *ModbusThreadFunc(void *arg)
{
    
    // int ModbusSocket = -1;
    // int ModbusRes;
    // int8_t ModCalibFlag[6] = {0};
    // modbus_mapping_t *ModbusMap_p;
    // modbus_t *ModbusCtx;
    // uint8_t *ModbusQuery;
    // int8_t Rterr = 0;
    // CHAN_DATA ChanCalib_ST = {0};
    // CHAN_DATA ChanADCValue_ST[5] = {0};
    // CHAN_DATA ChanADCFiltValue_ST = {0};
    // uint8_t ADCTranType_U8 = 0;
    // ADCTranType_U8 = CALIBTRANTYPE;
    // //const char *IP = "192.168.1.11";/*modbus server IP*/
    // const int PORT = 502;
    // /*uint16_t UT_REGISTERS_TAB[] = { 0x1234, 0x5678, 0x9ABC  };mapping data*/
    // ModbusQuery = malloc(MODBUS_TCP_MAX_ADU_LENGTH);
    // ModbusCtx = modbus_new_tcp(NULL, PORT);
    // printf("PORT:%d\n",PORT );
    // modbus_set_debug(ModbusCtx, FALSE);
    // VersionInit(&g_Version_ST);
    
    // /*modbus mapping init*/
    // ModbusMap_p = modbus_mapping_new_start_address(UT_BITS_ADDRESS_TCP,            UT_BITS_NB_TCP,
    //                                                 UT_INPUT_BITS_ADDRESS_TCP,      UT_INPUT_BITS_NB_TCP,
    //                                                 UT_REGISTERS_ADDRESS_TCP,       UT_REGISTERS_NB_TCP,
    //                                                 UT_INPUT_REGISTERS_ADDRESS_TCP, UT_INPUT_REGISTERS_NB_TCP);
    // if(ModbusMap_p == NULL) /*modbus mapping init failure*/
    // {
    //     fprintf(stderr, "Failed to allocate the mapping: %s\n",
    //             modbus_strerror(errno));
    //     modbus_free(ModbusCtx);
    //     pthread_exit(NULL);
    // }
    // /*start to listen when the first time of click the connect button */
    // ModbusSocket = modbus_tcp_listen(ModbusCtx, 1);
    // /*run forever when listened until close application*/
    // while(g_LifeFlag > 0) 
    // {
    //     if(-1 == modbus_tcp_accept(ModbusCtx, &ModbusSocket))
    //     {
    //         /*accept none*/
    //     }
    //     else /*accept data*/
    //     {
    //         ModCalibFlag[0] = 1;
    //         ModCalibFlag[1] = 1;
    //         ModCalibFlag[2] = 1;
    //         ModCalibFlag[3] = 1;
    //         ModCalibFlag[4] = 1;
    //         ModCalibFlag[5] = 1;
    //         VersionSet(ModbusMap_p,&g_Version_ST);
    //         while(1) 
    //         {
    //             /*the event about modbus receive */
    //             do 
    //             {
    //                 ModbusRes = modbus_receive(ModbusCtx, ModbusQuery);
    //                 /* Filtered queries return 0 */
    //             }while (ModbusRes == 0);/*require length*/
    //             if (ModbusRes == -1 && errno != EMBBADCRC) /* The connection is not closed on errors*/
    //             {
    //                 /* Quit */
    //                 break;
    //             }
    //             if((ModbusMap_p -> tab_registers[2]) && (ModCalibFlag[0] == 1))  //8002                 
    //             {
    //                 //for ChanCalibFile Once accept
    //                 ModCalibFlag[0] = 0;
    //                 printf("ChanCalibFileCreat\n");
    //                 // ChanCalibFileCreat(CHAN_CALIB_110V_FILE);
    //                 // ChanCalibDataTrans(&ChanADCValue_ST[0],ADCTranType_U8,g_ProcNum_U8);  
    //                 // ChanCalibDataFilt(&ChanADCValue_ST[0], &ChanADCFiltValue_ST,g_ProcNum_U8);    
    //                 // AllChanDataCalib(ADCVOLT_110V,ADCCURR_110V,&ChanCalib_ST,&ChanADCFiltValue_ST,1); //high vol calib
    //                 // Rterr = ChanCalibDataSave(&ChanCalib_ST);
    //                 // fclose(g_FileFd_ST.ChanCalibFile_fd);
    //                 //Fwrite ok,then set the Light On
    //                 if(Rterr  == 0)
    //                 {
    //                     ModbusMap_p-> tab_registers[280] = 1;//8280 
    //                 }
    //                 //else, set the Light Off
    //                 else
    //                 {
    //                     ModbusMap_p-> tab_registers[280] = 0;
    //                 }
                    
    //             }
    //             else if((ModbusMap_p -> tab_registers[2] == 0)&&(ModCalibFlag[0] == 0))
    //             {
    //                 ModCalibFlag[0] = 1;
    //                 ModbusMap_p-> tab_registers[280] = 0;
    //             }
    //             else if((ModbusMap_p -> tab_registers[24]) && (ModCalibFlag[1] == 1))  //8024                
    //             {
    //                 //for ChanCalibFile Once accept
    //                 ModCalibFlag[1] = 0;
    //                 printf("Chan\n");
    //                 //Fwrite ok,then set the Light On
    //                 if(Rterr  == 0)
    //                 {
    //                     ModbusMap_p-> tab_registers[281] = 1;//8280 
    //                 }
    //                 //else, set the Light Off
    //                 else
    //                 {
    //                     ModbusMap_p-> tab_registers[281] = 0;
    //                 }
                    
    //             }
    //             else if((ModbusMap_p -> tab_registers[24] == 0)&&(ModCalibFlag[1] == 0))
    //             {
    //                 ModCalibFlag[1] = 1;
    //                 ModbusMap_p-> tab_registers[281] = 0;
    //             }
    //             else if((ModbusMap_p -> tab_registers[25]) && (ModCalibFlag[2] == 1))  //8025                
    //             {
    //                 //for ChanCalibFile Once accept
    //                 ModCalibFlag[2] = 0;
    //                 printf("ChanCalibFileCreat\n");
    //                 //Fwrite ok,then set the Light On
    //                 if(Rterr  == 0)
    //                 {
    //                     ModbusMap_p-> tab_registers[282] = 1;//8280 
    //                 }
    //                 //else, set the Light Off
    //                 else
    //                 {
    //                     ModbusMap_p-> tab_registers[282] = 0;
    //                 }
                    
    //             }
    //             else if((ModbusMap_p -> tab_registers[25] == 0)&&(ModCalibFlag[2] == 0))
    //             {
    //                 ModCalibFlag[2] = 1;
    //                 ModbusMap_p-> tab_registers[282] = 0;
    //             }
    //             else if((ModbusMap_p -> tab_registers[26]) && (ModCalibFlag[3] == 1))  //8026                
    //             {
    //                 //for ChanCalibFile Once accept
    //                 ModCalibFlag[3] = 0;
    //                 printf("ChanCalibFileCreat\n");
    //                 //Fwrite ok,then set the Light On
    //                 if(Rterr  == 0)
    //                 {
    //                     ModbusMap_p-> tab_registers[283] = 1;//8280 
    //                 }
    //                 //else, set the Light Off
    //                 else
    //                 {
    //                     ModbusMap_p-> tab_registers[283] = 0;
    //                 }
                    
    //             }
    //             else if((ModbusMap_p -> tab_registers[26] == 0)&&(ModCalibFlag[3] == 0))
    //             {
    //                 ModCalibFlag[3] = 1;
    //                 ModbusMap_p-> tab_registers[283] = 0;
    //             }
    //             else if((ModbusMap_p -> tab_registers[27]) && (ModCalibFlag[4] == 1))  //8027                 
    //             {
    //                 //for ChanCalibFile Once accept
    //                 ModCalibFlag[4] = 0;
    //                 printf("ChanCalibFileCreat\n");
    //                 //Fwrite ok,then set the Light On
    //                 if(Rterr  == 0)
    //                 {
    //                     ModbusMap_p-> tab_registers[284] = 1;//8280 
    //                 }
    //                 //else, set the Light Off
    //                 else
    //                 {
    //                     ModbusMap_p-> tab_registers[284] = 0;
    //                 }
                    
    //             }
    //             else if((ModbusMap_p -> tab_registers[27] == 0)&&(ModCalibFlag[4] == 0))
    //             {
    //                 ModCalibFlag[4] = 1;
    //                 ModbusMap_p-> tab_registers[284] = 0;
    //             }
    //             else if((ModbusMap_p -> tab_registers[28]) && (ModCalibFlag[5] == 1))  //8028                
    //             {
    //                 //for ChanCalibFile Once accept
    //                 ModCalibFlag[5] = 0;
    //                 printf("ChanCalibFileCreat\n");
    //                 //Fwrite ok,then set the Light On
    //                 if(Rterr  == 0)
    //                 {
    //                     ModbusMap_p-> tab_registers[285] = 1;//8280 
    //                 }
    //                 //else, set the Light Off
    //                 else
    //                 {
    //                     ModbusMap_p-> tab_registers[285] = 0;
    //                 }
                    
    //             }
    //             else if((ModbusMap_p -> tab_registers[28] == 0)&&(ModCalibFlag[5] == 0))
    //             {
    //                 ModCalibFlag[5] = 1;
    //                 ModbusMap_p-> tab_registers[285] = 0;
    //             }
    //             else
    //             {

    //             }                               
    //             /*the event about modbus reply*/ 
    //             ModbusRes = modbus_reply(ModbusCtx, ModbusQuery, ModbusRes, ModbusMap_p);
    //             if(ModbusRes == -1) 
    //             {
    //                 break;
    //             }
    //         }
    //     }
    // }
    // /*close connection*/
    // printf("Quit the loop: %s\n", modbus_strerror(errno));
    // close(ModbusSocket);
    // modbus_mapping_free(ModbusMap_p);
    // free(ModbusQuery);
    // pthread_exit(NULL);
    // printf("Modbus close\n");

}

void *FileSaveThreaFunc(void *arg) 
{
    struct timeval A_Time_ST,A_TimeEnd_ST;
    uint8_t i = 0;
    uint32_t DelayNum = 0;
    DelayNum = g_Rec_XML_ST.Rec_Event_ST.RecInterval * g_EventSaveSlepNum_U32;//g_EventSaveSlepNum_U32 is us
    while(g_LifeFlag > 0)
    {
        
        threadDelay(0,DelayNum);
        if(g_FileSaveInfo_ST.EventSaveNum_U32 >= EVENT_SAVE_NUM)//
        {
            //close the open file and creat new event file
            printf("g_FileSaveInfo_ST.EventSaveNum_U32 %d\n",g_FileSaveInfo_ST.EventSaveNum_U32);
            fclose(g_FileFd_ST.EventFile_fd);
            EventFileCreateByNum(&g_FileFd_ST,&g_Rec_XML_ST,&g_TrainInfo_ST);
            g_FileSaveInfo_ST.EventSaveNum_U32 = 0;
        }
       /******
       if(g_frame_LREventNum_U32 >= EVENT_SAVE_NUM)//
        { 
            fclose(g_fd_LRRecName);
            LRFileCreateByNum();
            DriveLRFileTopSave();
            g_frame_LREventNum_U32 = 0;
        }
        if(g_ChanStatuInfo_ST.LRFlag_U8)
        { 
            if(LR_DEBUG == g_DebugType_EU)
            {
                printf("DriveLRDataSave_3Coach\n");
            }
            g_ChanStatuInfo_ST.LRFlag_U8 = 0;
            DriveLRDataSave_3Coach();
            g_frame_LREventNum_U32++;
        }
        **********************/
        if(g_DebugType_EU == FILE_DEBUG)
        {
            printf(" STart EventDataSave\n");
        }
        EventDataSave(&g_FileFd_ST,g_ProcNum_U8,g_EADSType_U8);
        //printf("g_frame_EventTimeNum_U32 %d \n",g_frame_EventTimeNum_U32);  
        //g_frame_EventTimeNum_U32 = 0;
        g_FileSaveInfo_ST.EventSaveNum_U32++;
        if(g_DebugType_EU == TIME_DEBUG)
        {
            gettimeofday(&A_Time_ST,NULL);
            printf("FIle write %d \n",A_Time_ST.tv_usec);
        }
       // }
    }
    printf("exit File Write thread\n");
    pthread_exit(NULL);
}

void *TRDPThreadFunc(void *arg)
{
    
    // uint8_t TRDPRecDisLenth = 0;
    // uint8_t TRDPSendData[TRDP_SEND_DATA_SIZE] = {0};
    // uint8_t TRDPRecData[TRDP_REC_DATA_SIZE] = {0};
    // if(TRDP_DEBUG == g_DebugType_EU)
    // {
    //     printf("trdp_start\n");
    // }
    // if(0 != trdp_start())   
    // {
    //     /* start failed, do what you want(like give a error print) */
    //     printf("trdp_start failed \n");
    //     return 0;
    // }
    // while(g_LifeFlag > 0)
    // {
    //     /* do what you want */

    //     pdSendDataSet(TRDPSendData, &g_Trdp_SendData_ST);
    //     switch(g_TrainInfo_ST.CoachNum_U8)
    //     {
    //         case COACH1:
    //         if(TRDP_DEBUG == g_DebugType_EU)
    //         {
    //             printf("pdSend(EADS_COM_ID %d,\n",EADS_VEL1_COM_ID);
    //         }
    //         pdSend(EADS_VEL1_COM_ID,TRDPSendData);
    //         break;

    //         case COACH2:
    //         if(TRDP_DEBUG == g_DebugType_EU)
    //         {
    //             printf("pdSend(EADS_COM_ID %d,\n",EADS_VEL2_COM_ID);
    //         }
    //         pdSend(EADS_VEL2_COM_ID,TRDPSendData);
    //         break;
            
    //         case COACH3:
    //         if(TRDP_DEBUG == g_DebugType_EU)
    //         {
    //             printf("pdSend(EADS_COM_ID %d,\n",EADS_VEL3_COM_ID);
    //         }
    //         pdSend(EADS_VEL3_COM_ID,TRDPSendData);
    //         break;

    //         case COACH4:
    //         if(TRDP_DEBUG == g_DebugType_EU)
    //         {
    //             printf("pdSend(EADS_COM_ID %d,\n",EADS_VEL4_COM_ID);
    //         }
    //         pdSend(EADS_VEL4_COM_ID,TRDPSendData);
    //         break;

    //         case COACHZP:
    //         if(TRDP_DEBUG == g_DebugType_EU)
    //         {
    //             printf("pdSend(EADS_COM_ID %d,\n",EADS_ZP_COM_ID);
    //         }
    //         pdSend(EADS_ZP_COM_ID,TRDPSendData);
    //         break;
            
    //         default:
    //         printf("EADS_COM_ID error\n");
    //         break;

    //     }

    //     usleep(g_TRDPUsec_U32);
        
    //     pdRecv(CCU_COM_ID,TRDPRecData,TRDP_REC_DATA_SIZE);


    // }
    // trdp_stop();
    // printf("TRDP has Quit\n");
    // pthread_exit(NULL);
}
     
/* Blink LED */
void * LEDBilnkPthread (void *arg)
{
    uint32_t led_period;
    int tmp;
    tmp = open(LED_BRIGHTNESS, O_WRONLY);
    if (tmp < 0)
    {
        printf("Cannot open LED class\n");
        pthread_exit("led exit");
    }
    led_period =  PERIOD_COEFF;
    while (g_LifeFlag) 
    {

       
        write(tmp, "1", 2); //on
        usleep(led_period);
        write(tmp, "0", 2);//off
        usleep(led_period);
    }
}