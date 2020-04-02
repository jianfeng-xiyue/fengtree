/**********************************************************************
*File name 		:   BramDataProc.c
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/5/16
*Description    :   Include the Write data to 570,Read data from 570,
*                   Write data to MAX10,Read data from MAX10
*                   Read 570 RTC information and set system time

*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/05/16  Create
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
#include "BramDataProc.h"
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/
extern DEBUG_TYPE_ENUM g_DebugType_EU;

extern BLVDS_BRD_DATA g_BrdRdBufData_ST;
extern BLVDS_BRD_DATA g_BrdBufData_ST;

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
//20200325

int8_t g_bram_MemFd_I8 = 0;
uint8_t *g_bram_RDFlagAddr;
uint8_t *g_bram_WRFlagAddr;
uint8_t *g_bram_RD_A_BLVDSBlckAddr;

/**********************************************************************
*
*Static Variable Define Section
*
*********************************************************************/
static uint8_t s_bram_WRRDErrNum_U32 = 0;
static BRAM_ADDRS s_Bram_A_BLVDSBlckAddr_ST = {0};

/**********************************************************************
*Name           :   BRAM_RETN_ENUM BramReadDataExtraWiOutLife(uint32_t *Inbuff,uint32_t *Outbuff)
*Function       :   Extract the data of ReadData,without the CurrePackNum judge
*Para           :   uint32_t *Inbuff 
*                   uint32_t *Outbuff
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/3/29  Create
*********************************************************************/
BRAM_RETN_ENUM ExtraBoardData(uint32_t *Inbuff,uint32_t *Outbuff)
{
    uint16_t PacketLength,CpLength;
    BRAM_RETN_ENUM ErrorCode = 0;
    BRAM_PACKET_DATA *BramPacketData_ST_p;
    BramPacketData_ST_p = (BRAM_PACKET_DATA *)Inbuff;
    PacketLength = (BramPacketData_ST_p -> BLVDSTOP_U32 >> 24) & 0xFF;
    ////PacketLength != 0 mean,the bram have valid data
    //extract the BLVDS invalid data
    if(PacketLength != 0)
    {
        memcpy(Outbuff,&BramPacketData_ST_p -> BLVDSData_U32,PacketLength);     
        ErrorCode = RETURN_OK;
    }
    /*when power on,the read length is 0,should be wait bram fresh by FPGA*/
    return ErrorCode;
}
/**********************************************************************
*Name           :  BramRDMAX10ADCData(uint32_t BlockAddr,uint32_t BlockFlagAddr,
*                   uint8_t ChanNum,uint32_t *ReadDataBuf)
*Function       :   Read the MAX ADC data
*Para           :   uint32_t BlockAddre , BRAM_A9_RD_A_BLVDS_BLOCK0
*               :   uint32_t BlockFlagAddr BRAM_A9_RD_A_BLVDS_FlagBLOCK0
*               :   uint8_t ChanNum  :the Block number
*               :   uint8_t U32Length : LCU_BOARD_LENGTH
*               :   uint32_t *ReadDataBuf,
*Return         :   int8_t 0,success;else false.
*Version        :   REV1.0.0       
*Author         :   feng
*History:
*REV1.0.0     feng    2018/05/07  Create
*********************************************************************/
int8_t BoardDataRead(BRAM_ADDRS *BramAddrs_p,uint32_t *ReadData)
{
    uint32_t ReadDataBuf[64] = {0};
    uint16_t RDLengthTemp  = 0;
    int8_t Error = 0,FedError = 0;
    // ReadBuf = g_bram_DataRead_U32; 
    /*receive the feed back update command*/
    Error = BramReadWithChek(BramAddrs_p,ReadDataBuf);  
    if(Error != 0)
    {
        printf("BramReadWithChek error\n");
        return -1;
    }
    FedError = ExtraBoardData(ReadDataBuf,ReadData); 
    return 0;
}
// /**********************************************************************
// *
// *Function Define Section
// *********************************************************************/
/**********************************************************************
*Name           :   BRAM_RETN_ENUM BramReadDataExtraCMD(uint32_t *Inbuff,uint16_t CurrPackNum)
*Function       :   Extract the CMD data of ReadData,If the CurrPackNum is equal to the CurrPackNumTemp
*                   meaning 570 has receiver success,otherwise is failure.
*Para           :   uint32_t *Inbuff 
*                   uint16_t TotalPackNum   The  total write packet number
*                   uint16_t CurrPackNum    The current write packet number
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/3/29  Create
*********************************************************************/
BRAM_RETN_ENUM BramReadDataExtraCMD(uint32_t *Inbuff,uint16_t CurrPackNum)
{

    uint16_t TotalPackNumTemp = 0,CurrPackNumTemp = 0;
    uint8_t UpDateTypeTemp = 0,UpDateStatTemp = 0,UpDateStat = 0;
    BRAM_RETN_ENUM ErrorCode  = 0;
    BRAM_PACKET_DATA *BramPacketData_ST_p;
    BramPacketData_ST_p = (BRAM_PACKET_DATA *)Inbuff;

    CurrPackNumTemp = (BramPacketData_ST_p -> BLVDSReser_U32[1] & 0xFFFF);
    UpDateStatTemp =  (BramPacketData_ST_p -> BLVDSReser_U32[1] >> 16) & 0xFF;
    //UpDateTypeTemp = (BramPacketData_ST_p -> BLVDSReser_U32[1] >> 24) & 0xFF;

    UpDateStat =  (CMD_STATUS_ENUM)UpDateStatTemp;

    /*check program update whether right*/
    if(CurrPackNumTemp == CurrPackNum)
    {
        if(CMD_OK == UpDateStat)
        {
              
            ErrorCode = RETURN_OK;
            s_bram_WRRDErrNum_U32 = 0;
        }
        /*CRC error ,repeat write*/
        else if(CMD_CRC_ERROR == UpDateStat)
        {
            ErrorCode = RETURN_CRC_ERROR;
            s_bram_WRRDErrNum_U32 ++;
        }
        /*another error ,repeat write*/
        else if(CMD_ERROR == UpDateStat) 
        {
            ErrorCode = RETURN_ERROR;
            s_bram_WRRDErrNum_U32 ++;
        }
        else if (CMD_BUSY == UpDateStat)/*for max 10*/
        {
            ErrorCode = RETURN_BUSY;
            s_bram_WRRDErrNum_U32 = 0; 
        }
        /*for reset the max10 program*/

    }
    else
    {

        ErrorCode = RETURN_ELSE_ERROR;
        s_bram_WRRDErrNum_U32 ++;
    }

    return ErrorCode;
}
/**********************************************************************
*Name           :   int8_t BramWriteAssigVal(uint16_t Length,uint32_t *Inbuf,
*                   uint8_t ChanNum,uint16_t CurrPackNum,uint8_t CurrCMD,COMMU_MCU_ENUM TargeMCU)
*                   
*Function       :   AssigVal to Bram data
*Para           :   uint16_t Length: data length no include frame top
*               :   uint32_t *Inbuf: the data 
*               :   uint8_t ChanNum: Write which Chan 
*                   uint16_t CurrPackNum:
*                   uint8_t CurrCMD :
*                   COMMU_MCU_ENUM TargeMCU :MAX and TMS570
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/05/07  Create
*********************************************************************/
int8_t BramWriteAssigVal(BRAM_CMD_PACKET *CmdPact_p,uint32_t *Outbuf,uint32_t *Inbuf)
{

    uint16_t i = 0;
    uint32_t TempCrcValue_U32;
    uint32_t Framelen = 0;
    uint32_t Temp32Value = 0;
    //uint8_t TempBlockID_U8 = 0;
    BRAM_PACKET_DATA *BramPacketData_ST_p;
    BramPacketData_ST_p = (BRAM_PACKET_DATA *)Outbuf;
    
    /*top */
    if(CmdPact_p -> TargeMCU == TMS570_MCU)
    {
        Temp32Value = 0x11C2;
    }
    else if(CmdPact_p -> TargeMCU == MAX10_MCU)
    {
        Temp32Value = 0x1144;
    }
    else
    {
        Temp32Value = 0x0;
    }
    /*ChanNum is occupy 6 bit */
    Temp32Value += ((CmdPact_p -> ChanNum_U8) << 18);
    Framelen = ((CmdPact_p -> PacktLength_U32) + BRAM_PCKT_TOP_LNGTH_U32) * 4;
    Temp32Value += (Framelen << 24);
    BramPacketData_ST_p -> BLVDSTOP_U32 = Temp32Value;

    BramPacketData_ST_p -> BLVDSReser_U32[0] = 0;
    Temp32Value =  CmdPact_p -> BlockNum_U16;
    Temp32Value += ((CmdPact_p -> PacktCMD_U8) << 24);
    BramPacketData_ST_p -> BLVDSReser_U32[1] = Temp32Value;
    /*data, memcp uinit is byte,so the lenth is Length*4*/
    memcpy(&BramPacketData_ST_p -> BLVDSData_U32,Inbuf,(CmdPact_p -> PacktLength_U32) * 4); 
    TempCrcValue_U32 = Crc32CalU32Bit(Outbuf,(CmdPact_p -> PacktLength_U32) + BRAM_PCKT_TOP_LNGTH_U32);
    /*small end*/
    /*crc*/
    BramPacketData_ST_p -> BLVDSData_U32[CmdPact_p -> PacktLength_U32]= TempCrcValue_U32;
    //printf("Write CrcValue_U32 %x\n",TempCrcValue_U32); 
    return 0;
}






int8_t BramOpenMap(void)
{
    int8_t err = 0;
    g_bram_MemFd_I8 = BramOpen();
    if(-1 == g_bram_MemFd_I8)
    {
        err = -1;
    }
    g_bram_RD_A_BLVDSBlckAddr = BramMap(g_bram_MemFd_I8,BRAM_A9_RD_A_BLVDS_BLOCK0,BRAM_4BLCK_LNGTH_U8);
    if(g_bram_RD_A_BLVDSBlckAddr == MAP_FAILED)
    {
        err = -1;
    }
    g_bram_RDFlagAddr = BramMap(g_bram_MemFd_I8,BRAM_A9_RD_A_TMS_FLAG,300);//both TMS and blvsd  use this addres        
    if(g_bram_RD_A_BLVDSBlckAddr == MAP_FAILED)
    {
         err = -1;
    }
    g_bram_WRFlagAddr = BramMap(g_bram_MemFd_I8,BRAM_A9_WR_A_TMS_FLAG,300);//when run mamap,to avoid mamap many times
    if(g_bram_RD_A_BLVDSBlckAddr == MAP_FAILED)
    {
         err = -1;
    }
    return err;
}
 


int8_t BramCloseMap(void)
{
    CloseBram((void *)g_bram_RD_A_BLVDSBlckAddr,BRAM_4BLCK_LNGTH_U8);
    CloseBram((void *)g_bram_RDFlagAddr,300); 
    CloseBram((void *)g_bram_WRFlagAddr,300);
    close(g_bram_MemFd_I8);
    return 0;
}


int8_t BLVDSReadInit(uint8_t EADSType)  
{  
    uint16_t i = 0;
    int8_t ReadErr = 0;
    BramOpenMap();
    s_Bram_A_BLVDSBlckAddr_ST.MapBlckAddr_p = g_bram_RD_A_BLVDSBlckAddr;
    s_Bram_A_BLVDSBlckAddr_ST.MapBlckFlgAddr_p = g_bram_RDFlagAddr;
    s_Bram_A_BLVDSBlckAddr_ST.BramBlckAddr = BRAM_A9_RD_A_BLVDS_BLOCK0;
    s_Bram_A_BLVDSBlckAddr_ST.BramBlckFlgAddr = BRAM_A9_RD_A_TMS_FLAG;   
    /*Read the BLVDS data*/
    /**************************** 
	ADU_BOARD_ID  0
	ATU_BOARD_ID   1
    ******************************/

    return ReadErr;
}
  
void SimulinkBVDSData(BLVDS_BRD_DATA * chandata_p,uint8_t ReadNum_U8,uint8_t EADSType)
{
    uint8_t i = 0;
    for(i = 0;i < 20;i++)
    {
        chandata_p -> Board0_Data_U32[ReadNum_U8][i] = 0x1e5e1de3 + i*10 + ReadNum_U8 *100;

    } 
    if(EADSType == CTU_BOARD_ID)
    {
        for(i = 0;i < 14;i++)
        {
            chandata_p -> Board1_Data_U32[ReadNum_U8][i] = 0x033b1de3 + ((i*10)<<4);

        } 
    }

}

int8_t BLVDSDataReadThreaFunc(uint8_t ReadNum_U8,uint8_t EADSType) 
{

    static uint8_t i = 0;
    int8_t ReadErr = 0;
    if(BRAM_DEBUG == g_DebugType_EU)
    {
        printf("Read ADU_BOARD BLVDS\n");

    }
    /*Read ADU_BOARD_ID */
    s_Bram_A_BLVDSBlckAddr_ST.DataU32Length = ADU_BRD_RD_LENGTH_U32 ;
    //s_Bram_A_BLVDSBlckAddr_ST.BoardType_Em = (BOARD_TYPE_ENUM)ADU_BOARD_TYPE;
    s_Bram_A_BLVDSBlckAddr_ST.ChanNum_U8 = ADU_BOARD_ID;
    //SimulinkBVDSData(&g_BrdRdBufData_ST,ReadNum_U8,EADSType);
    ReadErr = BoardDataRead(&s_Bram_A_BLVDSBlckAddr_ST,&g_BrdRdBufData_ST.Board0_Data_U32[ReadNum_U8][0]); 
   //in zongpei the have CTU,so  BrdNum is 2
    if(EADSType == CTU_BOARD_ID)
    {
       /*Read ADU_BOARD_ID */
	   s_Bram_A_BLVDSBlckAddr_ST.DataU32Length = CTU_BRD_RD_LENGTH_U32 ;
	   s_Bram_A_BLVDSBlckAddr_ST.ChanNum_U8 = CTU_BOARD_ID;
	   if(BRAM_DEBUG == g_DebugType_EU)
	   {
		  printf("Read CTU_BOARD BLVDS\n");
	
	   }
	   ReadErr = BoardDataRead(&s_Bram_A_BLVDSBlckAddr_ST,&g_BrdRdBufData_ST.Board1_Data_U32[ReadNum_U8][0]);

	}	
	return ReadErr;

}
/**********************************************************************
*Name           :   int8_t SetCMDto570(uint8_t ChanNum,uint8_t ChanCMD,uint16_t ChanPackNum)             
*Function       :   Write cmd data to 570 spc block ,and read data back to kown whether 570 is get                 
*Para           :   uint16_t Length
*                   uint32_t *WriteDataBuf  
*                   int8_t ChanNum  
*                   uint8_t PackCMD 
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/8/1  Create
*********************************************************************/
int8_t SetCMDto570(BRAM_ADDRS *RDSpcBlk_p,BRAM_ADDRS *WRSpcBlk_p,BRAM_CMD_PACKET *CmdPact_p,uint32_t *Inbuff,uint32_t *Outbuff)
{
    uint32_t ReadBuf[50] = {0};

    uint16_t RDLengthTemp  = 0,WRLengthTemp =0;
    int8_t Error = 0,FedError = 0;
    uint8_t SySTimeString[40] = {0};
    uint16_t Year_U16 = 0;
    uint8_t i = 0;

    Error = BramWriteWithChek(WRSpcBlk_p,Inbuff);
    if(Error != 0)
    {
        printf("BramWriteWithChek error\n");           
        return -1;
    }
    usleep(1000); //700~1000 us is ok
    /*receive the feed back update command*/
    //RDLengthTemp = Length + BRAM_PACKET_TOP_LENGTH + 4
    Error = BramReadWithChek(RDSpcBlk_p,Outbuff);
    if(Error != 0)
    {
        printf("BramReadWithChek error\n");      
        return -2;
    }
    FedError = BramReadDataExtraCMD(Outbuff,CmdPact_p -> BlockNum_U16);
    while(FedError != RETURN_OK)
    {
        /*Repeat Send data to 570 for CRC error*/
        while((s_bram_WRRDErrNum_U32 > 0)&&(s_bram_WRRDErrNum_U32 < RD_570FEDERR_NUM))
        {
            
            if(RETURN_CRC_ERROR == FedError)
            {
                printf("Repeat %d Send data to 570 for CRC err,The ChanPackNum:%d\n",s_bram_WRRDErrNum_U32,CmdPact_p -> BlockNum_U16);
                Error = BramWriteWithChek(WRSpcBlk_p,Inbuff);
                if(Error != 0)
                {
                    printf("BramWriteWithChek error\n");
                    return -1;
                }
                usleep(1000); 
                Error = BramReadWithChek(RDSpcBlk_p,Outbuff); 
                if(Error != 0)
                {
                    printf("BramReadWithChek error\n");
                    return -1;
                }
                FedError = BramReadDataExtraCMD(Outbuff,CmdPact_p -> BlockNum_U16);

            }
            /*Repeat Send data to 570 for error*/
            else if(RETURN_ERROR == FedError)
            {
                printf("Repeat %d Send data to 570 for error,The ChanPackNum:%d\n", s_bram_WRRDErrNum_U32,CmdPact_p -> BlockNum_U16);
                Error = BramWriteWithChek(WRSpcBlk_p,Inbuff);
                 if(Error != 0)
                {
                    printf("BramWriteWithChek error\n");
                    return -1;
                }
                usleep(1000); 
                Error = BramReadWithChek(RDSpcBlk_p,Outbuff);
                if(Error != 0)
                {
                    printf("BramReadWithChek error\n");
                    return -1;
                }
                FedError = BramReadDataExtraCMD(Outbuff,CmdPact_p -> BlockNum_U16);

            }
            /*Wait 570 write back,Repeate %d times read Bram*/
            /*Normally  it will repeat read 2 times,when wait 570 write back*/
            else
            {

                usleep(1000); 
                Error = BramReadWithChek(RDSpcBlk_p,Outbuff);
                if(Error != 0)
                {
                    printf("BramReadWithChek error\n");
                    return -1;
                }
                FedError = BramReadDataExtraCMD(Outbuff,CmdPact_p -> BlockNum_U16);
                //usleep(10);  
            }
        }/* end of while(s_bram_WRRDErrNum_U32 > 0)*/
        if(s_bram_WRRDErrNum_U32 == RD_570FEDERR_NUM)
        {
            printf("Write  one packet failed\n");
            s_bram_WRRDErrNum_U32 = 0;/*for repeat update,if no clear,when repeat update ,it will into forever cycle */
            return -1;
        }
        
    }/* end of while(FedError != RETURN_OK)*/
    return 0;
}
/**********************************************************************
*Name           :   int8_t SetCMDPackto570(uint16_t Length,uint8_t ChanNum,
*                   uint32_t *WriteDataBuf,uint8_t ChanCMD,uint16_t ChanPackNum)      
*Function       :   First Write the CMD packet to 570 ,
*                   Second, write  the Make Sure packet to 570             
*Para           :   uint16_t Length
*                   uint32_t *WriteDataBuf  
*                   int8_t ChanNum  
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/8/1  Create
*********************************************************************/
int8_t SetCMDPackto570(BRAM_ADDRS *RDSpcBlk_p,BRAM_ADDRS *WRSpcBlk_p,BRAM_CMD_PACKET *CmdPact_p,uint32_t *Inbuff,uint32_t *Outbuff)
{
    int8_t err = 0;
    uint16_t ChanSurePackNum = 0;
    uint8_t SureLenth = 0;
  
    err = SetCMDto570(RDSpcBlk_p,WRSpcBlk_p,CmdPact_p,Inbuff,Outbuff);
    if(err != 0)
    {
        printf("SetCMDPack data to570 faile\n");
    }
    CmdPact_p -> BlockNum_U16 = UPSURE_BLOCKNUM;
    usleep(1000);
    err = SetCMDto570(RDSpcBlk_p,WRSpcBlk_p,CmdPact_p,Inbuff,Outbuff);
    if(err != 0)
    {
        printf("SetCMDPack sure to570 faile\n");
    }
    return err;    

}
// /**********************************************************************
// *Name           :   int8_t Read570Version(uint16_t Length,uint8_t ChanNum,uint16_t *RTSVerion_p,uint16_t *IECVerion_p)             
// *Function       :   Read the IO state chan of 570, get the RTS version and IEC version               
// *Para           :   uint16_t Length  :the data length
// *                   uint8_t ChanNum: state chan 
// *                   uint16_t *RTSVerion_p 
// *                   uint16_t *IECVerion_p
// *Return         :   int8_t 0,success;-1 false.
// *Version        :   REV1.0.0       
// *Author:        :   feng
// *History:
// *REV1.0.0     feng    2018/9/12  Create
// *********************************************************************/
// int8_t Read570Version(uint16_t Length,uint8_t ChanNum,uint16_t *RTSVerion_p,uint16_t *IECVerion_p)
// {
//     int8_t err = 0;
//     uint32_t *ReadBuf = 0;
//     uint8_t RDLengthTemp = 0;
//     uint32_t ReadData[50] = {0};
//     uint16_t VersioTemp = 0;

//     ReadBuf = g_bram_DataRead_U32;  
//     /*receive the feed back update command*/
//     //RDLengthTemp = Length + BRAM_PACKET_TOP_LENGTH + 4
//     RDLengthTemp = Length + BRAM_PACKET_TOP_LENGTH + 1;//4 include 1 crc and 40 more data
//     err = BramReadWithChek(g_bram_RDSpclBlockAddr,g_bram_RDFlagAddr,
//                 BRAM_A9_RD_A_570_SPCBLOCK0,BRAM_A9_RD_A_570_SPCFLAG,ChanNum,ReadBuf,RDLengthTemp);
//     if(err != 0)
//     {
//         printf("BramReadWithChek error\n");      
//         return -2;
//     }
//     //extra time data 
//     err =  BramReadDataExtraWiOutLife(&g_bram_DataRead_U32[0],&ReadData[0]);
//     if(err != 0)
//     {
//         printf("BramReadDataExtraWiOutLife error\n");      
//         return -2;
//     }
//     VersioTemp = (ReadData[0] >> 16) & 0xffff;
//     *RTSVerion_p = VersioTemp;
//     VersioTemp = ReadData[1] & 0xffff;
//     *IECVerion_p = VersioTemp; 
//     return err;    

// }


