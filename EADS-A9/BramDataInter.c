/**********************************************************************
*File name 		:   BramDataInter.c
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/3/29
*Description    :   Include the updata 570,Read Bram data,Write Bram Data

*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/03/29  Create
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
*Include file  sectiorn
*
*********************************************************************/
#include "BramDataInter.h"
 
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/
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
/*contain the num when FPGA calculate CRC Err*/
static uint8_t s_bram_WriteCRCErrNum_U8 = 0;
static uint8_t s_bram_ReadCRCErrNum_U8 = 0;
/**********************************************************************
*
*Function Define Section
*
*********************************************************************/
/**********************************************************************
*Name           :   int8_t BramReadDataWithCRC(uint32_t *Inbuff)
*Function       :   CRC the read data from BRAM
*Para           :   uint32_t *Inbuff  
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/1/29  Create
*********************************************************************/
int8_t BramReadDataWithCRC(uint32_t *Inbuff)
{
    uint16_t PacketLength = 0;
    uint32_t TempCrcValue_U32 = 0,PackeCrcValue_U32 = 0,LifeNum_U32 = 0;
    
    BRAM_PACKET_DATA *BramPacketData_ST_p;
    BramPacketData_ST_p = (BRAM_PACKET_DATA *)Inbuff;
    PacketLength = (BramPacketData_ST_p -> BLVDSTOP_U32 >> 24) & 0xFF;
    /*change to 32bit length*/
    PacketLength = PacketLength / 4;
    TempCrcValue_U32 = Crc32CalU32Bit(Inbuff,PacketLength);
    PackeCrcValue_U32 = (BramPacketData_ST_p -> BLVDSData_U32[PacketLength - BRAM_PCKT_TOP_LNGTH_U32]);//BLVDSData_U32 not include the TOP
    if(TempCrcValue_U32 == PackeCrcValue_U32)
    {
        return 0;
    }
    else
    {

        
        /*Now often happen crc err ,and repeate 1st read is OK*/
        if(BRAM_DEBUG == g_DebugType_EU)
        {       
            printf("Bram read Crc failed\n");
            printf("TempCrcValue_U32 %08x,PackeCrcValue_U32 %08x \n",TempCrcValue_U32,PackeCrcValue_U32); 
    
        }
    return -1;
       
    }
    
    
}

/**********************************************************************
*Name           :   int8_t BramBlockFlagWait
*Function       :   Wait the FPGA write Flag is clear.Then set the A9 read Block Flag
*Para           :   uint32_t mapbase        The mmap  Block Addres,
*               :   uint32_t BlockFlagAddre The Block Flag Addres,
*               :   uint16_t ChanNum        THe Channel number of Bram Block
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author         :   feng
*History:
*REV1.0.0     feng    2018/03/12  Create
*********************************************************************/
int8_t BramBlockRDFlagWait(BRAM_ADDRS *BramAddrs_p)
{
    uint32_t ChanAddress = 0,ChanFlagAddr = 0,ChanFedFlagAddr = 0;
    uint8_t WRFlagInt = 0,WRFlagBit = 0;
    uint8_t WRFlagTemp = 0,WRFlagValue = 0,i;
    uint16_t WaitFlagClerNum = 0;
    struct timeval TimeStar,TimeEnd;
    uint8_t ChanNum = 0;
    ChanNum = BramAddrs_p -> ChanNum_U8;

    WRFlagInt = ChanNum / 8;
    WRFlagBit = ChanNum % 8;
    
    ChanFlagAddr = BramAddrs_p -> BramBlckFlgAddr + WRFlagInt;
    ChanFedFlagAddr = ChanFlagAddr + BRAM_PCKT_LNGTH_U8;

     /*Wait the 570 write flag clear*/ 
    WaitFlagClerNum = 500;

    for(i = 0;i < WaitFlagClerNum;i++)
    {           
        WRFlagValue = BramReadU8(ChanFlagAddr,BramAddrs_p -> MapBlckFlgAddr_p);
        if(BRAM_DEBUG == g_DebugType_EU)
        {
            printf("WRFlagValue %x \n",WRFlagValue);
        } 
        WRFlagTemp = WRFlagValue >> WRFlagBit;
        WRFlagTemp = WRFlagTemp & 1; 
        // if(TIME_DEBUG == g_DebugType_EU)
        // {
        //     printf("i %d,Read WRFlag:%02x\n",i,WRFlagValue);    
        // }   
        if(WRFlagTemp == 0)
        {
            break;
        }
        if(i == 450)
        {
            return -1;             
        }
      
    }
    /*Set the ReadFlag bit*/
    WRFlagTemp = (1 << WRFlagBit); 
    BramWriteU8(ChanFedFlagAddr,BramAddrs_p -> MapBlckFlgAddr_p,WRFlagTemp);
    return 0;
}
/**********************************************************************
*Name           :   int8_t BramBlockFlagClear
*Function       :   Clear the A9 Read flag
*Para           :   uint32_t mapbase    The mmap Block Addres,
*               :   uint32_t BlockFlagAddre The Block Flag Addres,
*               :   uint16_t ChanNum        THe Channel number of Bram Block
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author         :   feng
*History:
*REV1.0.0     feng    2018/03/12  Create
*********************************************************************/
int8_t BramBlockFlagClear(BRAM_ADDRS *BramAddrs_p)
{
    uint32_t ChanFlagAddr = 0,ChanFedFlagAddr = 0;
    uint8_t WRFlagInt = 0,WRFlagBit = 0;
    uint8_t WRFlagTemp = 0,i;
    struct timeval TimeStar,TimeEnd;
    uint8_t ChanNum = 0;
    ChanNum = BramAddrs_p -> ChanNum_U8;

    WRFlagInt = ChanNum / 8;
    WRFlagBit = ChanNum % 8;
    ChanFlagAddr = BramAddrs_p -> BramBlckFlgAddr + WRFlagInt;
    ChanFedFlagAddr = ChanFlagAddr + BRAM_PCKT_LNGTH_U8;
   
    /*Set the ReadFlag bit*/
    WRFlagTemp = 0 ;
    // if(BRAM_DEBUG == g_DebugType_EU)
    // {
    //     gettimeofday(&TimeStar,NULL);
    // }   
    BramWriteU8(ChanFedFlagAddr,BramAddrs_p -> MapBlckFlgAddr_p,WRFlagTemp);//1us

    // if(BRAM_DEBUG == g_DebugType_EU)
    // {
    //     gettimeofday(&TimeEnd,NULL);
    //     printf("Clear time :%d \n",TimeEnd.tv_usec- TimeStar.tv_usec);
    // }

    return 0;
}
/**********************************************************************
*Name           :   int8_t BramBlockReadOpti
*Function       :   Read one  Block data from Bram, The channel address is BlockAddre + 256 * ChanNum
*Para           :   uint32_t BaseAddr     The mmap Block Addres,
*               :   uint32_t BlockAddre   The Block Addres,
*               :   uint16_t ChanNum      THe Channel number of Bram Block
*               :   uint32_t *Outbuf      Point to data save array 
*               :   uint16_t Length       lenth of the read data,
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author         :   feng
*History:
*REV1.0.0     feng    2018/03/12  Create
*********************************************************************/
int8_t BramBlockReadOpti(BRAM_ADDRS *BramAddrs_p,uint32_t *Outbuf)
{
    uint32_t ChanAddress = 0;
    uint8_t i;
    int8_t CrcErr = 0;
    struct timeval TimeStar,TimeEnd;
    uint16_t RdLength = 0;
    uint8_t BlockNum = 0;
    uint8_t *BlockMapAddr_p;
    uint8_t ChanNum = 0;
    ChanNum = BramAddrs_p -> ChanNum_U8;
   // BlockNum = (ChanNum >> 2);
    ChanAddress = BramAddrs_p -> BramBlckAddr + ChanNum * BRAM_PCKT_LNGTH_U8;
    //BlockMapAddr_p = BramAddrs_p -> MapBlckAddr_p + BlockNum * BRAM_4BLCK_LNGTH_U8;
    if(BRAM_DEBUG == g_DebugType_EU)
    {
        printf("BramBlockReadOpti ChanAddress %x BaseAddr %x\n",ChanAddress,BramAddrs_p -> MapBlckAddr_p);
    } 
    // /*read the 570 feed back data 16Byte*/
   // RdLength = BramAddrs_p -> DataU32Length + BRAM_PCKT_TOP_LNGTH_U32 + BRAM_PCKT_CRC_LNGTH_U32;
    /*20200117 delete the  BRAM_PCKT_TOP_LNGTH_U32 */
	RdLength = BramAddrs_p -> DataU32Length + BRAM_PCKT_CRC_LNGTH_U32;
    BramPackReadU32(ChanAddress,BramAddrs_p -> MapBlckAddr_p,Outbuf,RdLength);
    /*Clear the ReadFlag bit*/
    // WRFlagTemp = 0; 
    // BramWrite_U8(ChanFedFlagAddr,WRFlagTemp);

    CrcErr = BramReadDataWithCRC(Outbuf);
    /*CrcErr = 2,mean Crc error*/
    if(CrcErr == -1)
    {
        s_bram_ReadCRCErrNum_U8++;          
    }
    else
    {
        s_bram_ReadCRCErrNum_U8 = 0;
    }
    if(BRAM_DEBUG == g_DebugType_EU)
    {
        printf("Read from Bram\n");
        for(i = 0; i < RdLength; i++)
        {
            printf("Bram Byte %d data %08x\n",i, *Outbuf);  
            Outbuf++; 
        } 
    } 
    return 0;
}
/**********************************************************************
*Name           :   int8_t BramBlockWRFlagWait
*Function       :   Wait  the A9 Write Flag  is clear
*Para           :   uint32_t mapbase     The  mmap Block Addres,
*               :   uint32_t BlockFlagAddre The Block Flag Addres,
*               :   uint16_t ChanNum        THe Channel number of Bram Block
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author         :   feng
*History:
*REV1.0.0     feng    2018/03/12  Create
*********************************************************************/
int8_t BramBlockWRFlagWait(BRAM_ADDRS *BramAddrs_p)
{
    uint32_t ChanAddress = 0,ChanFlagAddr = 0,ChanFedFlagAddr = 0;
    uint8_t WRFlagInt = 0,WRFlagBit = 0;
    uint8_t WRFlagTemp = 0,WRFlagValue = 0,i;
    uint16_t WaitFlagClerNum = 0;
    struct timeval TimeStar,TimeEnd;
    uint8_t ChanNum = 0;
    ChanNum = BramAddrs_p -> ChanNum_U8;
    WRFlagInt = ChanNum / 8;
    WRFlagBit = ChanNum % 8;
    
    ChanFlagAddr = BramAddrs_p -> BramBlckFlgAddr + WRFlagInt;
     /*Wait the 570 write flag clear*/ 
    WaitFlagClerNum = 500;

    for(i = 0;i < WaitFlagClerNum;i++)
    {           
        WRFlagValue = BramReadU8(ChanFlagAddr,BramAddrs_p -> MapBlckFlgAddr_p);
        WRFlagTemp = WRFlagValue >> WRFlagBit;
        WRFlagTemp = WRFlagTemp & 1; 
        if(TIME_DEBUG == g_DebugType_EU)
        {
            printf("i %d,Read WRFlag:%02x\n",i,WRFlagValue);    
        }   
        if(WRFlagTemp == 0)
        {
            break;
        }
        if(i == 450)
        {
            return -1;             
        }
      
    }
    return 0;
}
/**********************************************************************
*Name           :   int8_t BramBlockFlagCrc
*Function       :   1,Set the A9 write Flag
*               :   2,wait A9 write flag clear
*               :   3.Read the FPGA Crc Flag 
*Para           :   uint32_t mapbase     The mmap Block Addres,
*               :   uint32_t BlockFlagAddre The Block Flag Addres,
*               :   uint16_t ChanNum        THe Channel number of Bram Block
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author         :   feng
*History:
*REV1.0.0     feng    2018/03/12  Create
*********************************************************************/
int8_t BramBlockFlagCrc(BRAM_ADDRS *BramAddrs_p)
{
    uint32_t ChanFlagAddr = 0,ChanFedFlagAddr = 0;
    uint8_t WRFlagInt = 0,WRFlagBit = 0;
    uint8_t WRFlagTemp = 0,WRFlagValue,i;
    struct timeval TimeStar,TimeEnd;
    uint16_t WaitFlagClerNum = 0;
    uint8_t FPGACrcErr = 0,FPGACrcErTemp = 0;
    int8_t ErrorCode = 0;
    uint8_t ChanNum = 0;
    ChanNum = BramAddrs_p -> ChanNum_U8;

    WRFlagInt = ChanNum / 8;
    WRFlagBit = ChanNum % 8;
    ChanFlagAddr = BramAddrs_p -> BramBlckFlgAddr + WRFlagInt;
    ChanFedFlagAddr = ChanFlagAddr + BRAM_PCKT_LNGTH_U8;
     /*Set the A9 write flag */ 
    WRFlagTemp = (1 << WRFlagBit);
    BramWriteU8(ChanFlagAddr,BramAddrs_p -> MapBlckFlgAddr_p,WRFlagTemp);

    /*Wait the A9 write flag clear*/ 
    WaitFlagClerNum = 500;

    for(i = 0;i < WaitFlagClerNum;i++)
    {           
        WRFlagValue = BramReadU8(ChanFlagAddr,BramAddrs_p -> MapBlckFlgAddr_p);
        WRFlagTemp = WRFlagValue >> WRFlagBit;
        WRFlagTemp = WRFlagTemp & 1; 
        // if(TIME_DEBUG == g_DebugType_EU)
        // {
        //     printf("i %d,Read WRFlag:%02x\n",i,WRFlagValue);    
        // }   
        if(WRFlagTemp == 0)
        {
 
            break;
        }
        if(i == 450)
        {
            return -1;             
        }
      
    }

     /*Read the  FPGA crc error */ 
    FPGACrcErr = BramReadU8(ChanFedFlagAddr,BramAddrs_p -> MapBlckFlgAddr_p);
    if(BRAM_DEBUG == g_DebugType_EU)
    {
        printf("the FPGACrcErr :%d\n",FPGACrcErr);
    }
    
    FPGACrcErTemp = FPGACrcErr >> WRFlagBit;
    FPGACrcErTemp = FPGACrcErTemp & 1; 
    /*CRC error*/
    if(FPGACrcErTemp != 0)
    {
        s_bram_WriteCRCErrNum_U8 ++;
        ErrorCode = -1;
    }
    /*CRC Right*/
    else
    {
        s_bram_WriteCRCErrNum_U8 = 0;
        ErrorCode = 0;
    }   
    return ErrorCode;
}
/**********************************************************************
*Name           :   int8_t BramBlockWriteOpti
*Function       :   Write one  Channel  data to Bram, The channel address is BlockAddre + 256 * ChanNum
*Para           :   uint32_t BaseAddr     The mmap Block Addres,
*               :   uint32_t BlockAddre   The Block Addres,
*               :   uint16_t ChanNum      THe Channel number of Bram Block
*               :   uint32_t *Outbuf      Point to write data
*               :   uint16_t Length       lenth of the read data,
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author         :   feng
*History:
*REV1.0.0     feng    2018/03/12  Create
*********************************************************************/
int8_t BramBlockWriteOpti(BRAM_ADDRS *BramAddrs_p,uint32_t *Inbuf)
{
    uint32_t ChanAddress = 0;
    uint8_t i;
    int8_t CrcErr = 0;
    uint16_t WrLength;
    struct timeval TimeStar,TimeEnd;
    uint32_t RdBuf[BRAM_PCKT_LNGTH_U32] = {0};
    uint8_t ChanNum = 0;

    ChanNum = BramAddrs_p -> ChanNum_U8;
    ChanAddress = BramAddrs_p -> BramBlckAddr + ChanNum * BRAM_PCKT_LNGTH_U8;
    if(BRAM_DEBUG == g_DebugType_EU)
    {
        printf("Write ChanAddress %x BaseAddr %x\n",ChanAddress, BramAddrs_p -> MapBlckAddr_p);
    }

    // /*read the 570 feed back data 16Byte*/
   // WrLength = BramAddrs_p -> DataU32Length + BRAM_PCKT_TOP_LNGTH_U32 + BRAM_PCKT_CRC_LNGTH_U32;
   /*20200117 delete the  BRAM_PCKT_TOP_LNGTH_U32 */
	 WrLength = BramAddrs_p -> DataU32Length + BRAM_PCKT_CRC_LNGTH_U32;
    BramPackWriteU32(ChanAddress,BramAddrs_p -> MapBlckAddr_p,Inbuf,WrLength);
    if(BRAM_DEBUG == g_DebugType_EU)
    {    
        BramPackReadU32(ChanAddress,BramAddrs_p -> MapBlckAddr_p,RdBuf,WrLength);
        printf("Write ChanAddress %x BaseAddr %x\n",ChanAddress, BramAddrs_p -> MapBlckAddr_p);
        printf("write bram readback\n");
        for( i = 0; i < WrLength; i++)
        {
            printf("Write Byte %d data %08x\n",i, RdBuf[i]);   
        }  

    }  
    return 0;
}

/**********************************************************************
*Name           :   int8_t BramBlockWrite
*Function       :   Write one  Channel  data to Bram, The channel address is BlockAddre + 256 * ChanNum
*Para           :   uint32_t BlockAddre     The Block Addres,
*               :   uint32_t BlockFlagAddre The Block Flag Addres,
*               :   uint16_t ChanNum        THe Channel number of Bram Block
*               :   uint32_t *Outbuf        Point to data save array 
*               :   uint16_t Length         lenth of the read data,
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author         :   feng
*History:
*REV1.0.0     feng    2018/03/12  Create
*********************************************************************/
int8_t BramBlockWrite(BRAM_ADDRS *BramAddrs_p,uint32_t *Inbuf)
{
    int8_t ErrorRet = 0; 
    uint32_t BackData[64];
    // if(BRAM_DEBUG == g_DebugType_EU)
    // {
    //     printf("BramBlockWrite  BlockAddre %x BlockFlagAddre %x\n",BlockAddre, BlockFlagAddre);
    //      printf("BramBlockWrite  PhyBlockAddr %x PhyFlagAddr %x\n", BramBlockAddre, BramBlockFlagAddre);
    // }    
    ErrorRet = BramBlockWRFlagWait(BramAddrs_p);
    if( ErrorRet == -1)
    {
        printf("wait error\n");
        return ErrorRet; 
    }
    BramBlockWriteOpti(BramAddrs_p,Inbuf);

    BramBlockFlagCrc(BramAddrs_p);
    return 0;
}

/**********************************************************************
*Name           :   int8_t BramMAX10BlockRead
*Function       :   Read one  Channel  data of MAX10 Bram, The channel address is BlockAddre + 256 * ChanNum
*Para           :   uint32_t BlockAddre     The Block Addres,
*               :   uint32_t BlockFlagAddre The Block Flag Addres,
*               :   uint16_t ChanNum        THe Channel number of Bram Block
*               :   uint32_t *Outbuf        Point to data save array 
*               :   uint16_t Length         lenth of the read data,
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author         :   feng
*History:
*REV1.0.0     feng    2018/03/12  Create
*********************************************************************/
int8_t BramBlockRead(BRAM_ADDRS *BramAddrs_p,uint32_t *Outbuf)
{
    int8_t ErrorRet = 0; 
    if(BRAM_DEBUG == g_DebugType_EU)
    {
        printf("BramBLVdsRead MapBlckFlagAddre %x MapBlockAddr %x\n",BramAddrs_p -> MapBlckFlgAddr_p,BramAddrs_p -> MapBlckAddr_p);
    }  
    ErrorRet = BramBlockRDFlagWait(BramAddrs_p);
    if( ErrorRet == -1)
    {
        printf("wait Flag time out error\n");
        return ErrorRet; 
    }
    BramBlockReadOpti(BramAddrs_p,Outbuf);
    BramBlockFlagClear(BramAddrs_p);
    return 0;
}
/**********************************************************************
*Name           :   int8_t BramReadWithChek2
*Function       :   Read max10 Bram data with proce the read data crc error.When crc errror,it will repeat read 3 times
*Para           :   uint32_t BlockAddre     The Block Addres,
*               :   uint32_t BlockFlagAddre The Block Flag Addres,
*               :   uint16_t ChanNum        THe Channel number of Bram Block
*               :   uint32_t *Outbuf        Point to data save array 
*               :   uint16_t Length         lenth of the read data,
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/4/1  Create
*********************************************************************/
int8_t BramReadWithChek(BRAM_ADDRS *BramAddrs_p,uint32_t *Outbuf)
{
    
    int8_t ErrorCode = 0;
    ErrorCode = BramBlockRead(BramAddrs_p,Outbuf);
    if (ErrorCode == -1 )
    {
        return ErrorCode;
    }
    if(s_bram_ReadCRCErrNum_U8 > 0)
    {
        /*FPGA Crc error reach 3times,return*/
        while((s_bram_ReadCRCErrNum_U8 > 0) && (s_bram_ReadCRCErrNum_U8 < RD_CRCERR_NUM))
        {
            /*Repeat Write Data to Bram*/
            /*Now often happen crc err ,and repeate 1st read is OK.should know the detail of mmap*/
            if(BRAM_DEBUG == g_DebugType_EU)
            {
               printf("repeate read %d for crc error\n",s_bram_ReadCRCErrNum_U8);                
            }

            ErrorCode = BramBlockRead(BramAddrs_p,Outbuf);
            if (ErrorCode == -1 )
            {
                return ErrorCode;
            }
        }  
        if(RD_CRCERR_NUM == s_bram_ReadCRCErrNum_U8)/*here ==,s_bram_ReadCRCErrNum_U8 < RD_CRCERR_NUM*/
        {
            s_bram_ReadCRCErrNum_U8 = 0;
            printf("Read Bram Failure for crc error\n");
            ErrorCode = -1;

        }
    }
    else
    {

        ErrorCode = 0;
    }
    return ErrorCode;
}
/**********************************************************************
*Name           :  int8_t BramWriteWithChek(uint32_t BlockAddre,uint32_t BlockFlagAddre,
*                  uint16_t ChanNum,uint32_t *Outbuf ,uint16_t Length)
*Function       :   Write a packet data to Bram,if The FPGA feed back the crc is error,it will repeat write 3 times
*Para           :   uint32_t BlockAddre     The Block Addres,
*               :   uint32_t BlockFlagAddre The Block Flag Addres,
*               :   uint16_t ChanNum        THe Channel number of Bram Block
*               :   uint32_t *Outbuf       Point to data save array 
*               :   uint16_t Length         lenth of the read data,
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/3/29  Create
*
*********************************************************************/
int8_t BramWriteWithChek(BRAM_ADDRS *BramAddrs_p,uint32_t *Inbuf)
{
    int8_t ErrorCode = 0;

    BramBlockWrite(BramAddrs_p,Inbuf);
    if(s_bram_WriteCRCErrNum_U8 > 0)
    {
        /*FPGA Crc error reach 3times,return error*/
        /*if write success ,the  s_bram_WriteCRCErrNum_U8 will be clear*/
        while((0 < s_bram_WriteCRCErrNum_U8) && (s_bram_WriteCRCErrNum_U8 < WR_CRCERR_NUM))
        {
            /*Repeat Write Data to Bram*/
            printf("The %d  times Repeat Write Bram\n",s_bram_WriteCRCErrNum_U8);
            BramBlockWrite(BramAddrs_p,Inbuf);
        }
        /*write failure*/
        if(WR_CRCERR_NUM == s_bram_WriteCRCErrNum_U8)
        {
            s_bram_WriteCRCErrNum_U8 = 0;
            printf("Write Bram Failure\n");
            ErrorCode = -1;
        }

    }
    else
    {
        //printf("Write Bram Right");
        ErrorCode = 0;
    }
    return ErrorCode;
}


