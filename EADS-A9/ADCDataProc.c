/**********************************************************************
*File name 		:   BramDataProc.c
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/5/16
*Description    :   Include process the ADC data of ADE7913
                    1.Trans the ADE7913 binary data to Voltage and Current
                    2.Process the Voltage using filting
                    3.Judge the Relay operate Number
                    4.Judge the L/R time
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/05/16  Create
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
#include "ADCDataProc.h"
 
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/

extern DEBUG_TYPE_ENUM g_DebugType_EU;

extern CHAN_CALIB_DATA  g_ChanCalib110VData_ST;/*read the ChanCalibData from /tffs0/ChanCalib10VValue.dat"*/

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
*Name           :   int8_t ADC7913VIDataTranInt
*Function       :   change the Binary data of Bram to Decinal data
*Para           :    uint32_t *BramInBuf,  the Binary data
*                   CHAN_DATA *g_ChanData_ST_p            the  Decinal data
*                   uint8_t TranType: 0 is Tran with Calib Value;1 is Tran without Calib Value for Calib Data Save
*                   
*Return         :   uint8_t 1,success;0 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*REV1.0.0     feng    2018/6/19  Add the offset value
*********************************************************************/
int8_t ADC7913IDataTranInt(uint32_t *BramInBuf,CHAN_DATA *OutBuf,uint16_t TranLeng,uint8_t TranType)
{
    uint16_t i = 0;
    int16_t VoltageTemBin = 0,CurrTemBin = 0; //the AD data with 16bit
    uint16_t VoltageBinAbs = 0;
    float  VoltageTem = 0,CurrTem = 0;//the AD data After Tran
    int16_t VolIntTem = 0,CurrIntTem = 0;
    float LinarFactr = 0;
    int16_t VolLinrValue = 0,CurrLinrValue = 0;
    uint32_t Inbuf;

    uint8_t VolIntFlag = 0;
    for(i = 0;i < TranLeng;i++)
    {
        Inbuf = *BramInBuf;
        VoltageTemBin = (int16_t)(Inbuf & 0xFFFF);
        CurrTemBin = (int16_t)(Inbuf >> 16) & 0xFFFF;
        /*Accord 7913 */
        if(LINECAlib_DEBUG == g_DebugType_EU)
        {
            printf(" %d VoltageTemBin %x  ",i,VoltageTemBin);
            printf("CurrTemBin %x\n", CurrTemBin);
        }
        //mean the voltage is - nagetive
         if(VoltageTemBin >= 0)
        {
            VolIntFlag = 1;
        }
                //mean the voltage is - nagetive
        else if(VoltageTemBin < 0)
        {
            VolIntFlag = 0;
        }
        VoltageBinAbs = abs(VoltageTemBin);//the Voltage only send uint value
        VoltageTem = (float)VoltageBinAbs * VOL_BINTAN_FACTOR; //24.0478uV
        CurrTem = (float)CurrTemBin * VOL_BINTAN_FACTOR ; //1.5036uV

        VoltageTem *= VOL_REALTAN_FACTOR;//VOL_REALTAN_FACTOR  550
        ////CurrTem *= VOL_REALTAN_FACTOR;//CURR_REALTAN_FACTOR  10, 
        CurrTem *= CURR_REALTAN_FACTOR; //CURR_REALTAN_FACTOR  10,
        //VolIntTem uint V
        VolIntTem = (int16_t)(VoltageTem /= 100000); // uV Trans to 100mV
        //CurrIntTem uint:mA
        ////   CurrIntTem =(int16_t)(CurrTem /= 10);//uV Trans to mV by /1000,and trans to mA by * 10,the Resister is 0.1 oumu
        CurrIntTem =(int16_t)(CurrTem /= 1000);//uA Trans to mA

        //for the voltage in circuit is  opposite the measure value
        //the circuit voltage is 110,the measure value is -110
        //the circuit voltage is -110,the measure value is 110
        OutBuf -> CurrChan_ST[i].CH_Current_I16 = CurrIntTem;
       if(LINECAlib_DEBUG == g_DebugType_EU)
        {
            //printf("VolIntTem %dV\n", VolIntTem);
            //printf("CurrIntTem %dmA\n", CurrIntTem);
            printf("CurrCH %d: %dmA\n",i, OutBuf -> CurrChan_ST[i].CH_Current_I16);
        }
        BramInBuf++;
    }
    return 0;
}
/**********************************************************************
*Name           :   int8_t ADC7913VDataTranInt
*Function       :   change the Binary data of Bram to Decinal data
*Para           :   uint32_t *BramInBuf  
*                   CHAN_DATA *OutBuf  
*                   uint16_t TranLeng         
*                   uint8_t TranType: 0 is Tran with Calib Value;1 is Tran without Calib Value for Calib Data Save
*                                     
*Return         :   uint8_t 1,success;0 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*********************************************************************/
int8_t ADC7913VDataTranInt(uint32_t *BramInBuf,CHAN_DATA *OutBuf,uint16_t TranLeng,uint8_t TranType)
{
    uint16_t i = 0;
    int16_t VoltageTemBin = 0,HVoltageTemBin = 0; //the AD data with 16bit
    uint16_t VoltageBinAbs = 0,HVoltageBinAbs = 0;
    float  VoltageTem = 0,HVoltageTem = 0;//the AD data After Tran
    int16_t VolIntTem = 0,HVolIntTem = 0;
    float LinarFactr = 0;
    int16_t VolLinrValue = 0;
    uint32_t Inbuf;
    uint8_t VolIntFlag = 0,HVolIntFlag = 0;
    //every loop ,i +=2;
    for(i = 0;i < TranLeng;i++)
    {
        Inbuf = *BramInBuf;
        VoltageTemBin =  (int16_t)(Inbuf & 0xFFFF); 
        HVoltageTemBin =  (int16_t)((Inbuf >> 16) & 0xFFFF); 
        // VoltageTemBin -= g_ChanOffsetValue_ST.ChanWithVol_Offset_I16[i];
        // HVoltageTemBin -= g_ChanOffsetValue_ST.ChanWithVol_Offset_I16[i+1];
        /*Accord 7913 */
        if(LINECAlib_DEBUG == g_DebugType_EU)
        {
            printf("V %d,VoltageTemBin %x ",i,VoltageTemBin);
            printf("HVoltageTemBin %x\n",HVoltageTemBin);
        }
        if(VoltageTemBin >= 0)
        {
            VolIntFlag = 1;
        }
        //mean the voltage is - nagetive
        else if(VoltageTemBin < 0)
        {
            VolIntFlag = 0;
        }
        if(HVoltageTemBin >= 0)
        {
            HVolIntFlag = 1;
        }
        //mean the voltage is - nagetive
        else if(HVoltageTemBin < 0)
        {
            HVolIntFlag = 0;
        } 
        VoltageBinAbs = abs(VoltageTemBin);//the Voltage only send uint value
        VoltageTem = (float)VoltageBinAbs * VOL_BINTAN_FACTOR; //24.0478uV
        HVoltageBinAbs = abs(HVoltageTemBin);//the Voltage only send uint value
        HVoltageTem = (float)HVoltageBinAbs * VOL_BINTAN_FACTOR; //24.0478uV
        VoltageTem  *= VOL_REALTAN_FACTOR;// VOL_REALTAN_FACTOR  550
        HVoltageTem *= VOL_REALTAN_FACTOR;// VOL_REALTAN_FACTOR  550
        //VolIntTem uint V
        VolIntTem = (int16_t)(VoltageTem / 100000); // uV Trans to 100mV
        HVolIntTem = (int16_t)(HVoltageTem / 100000); // uV Trans to 100mV
        if(1 == VolIntFlag)
        {
            OutBuf -> VolChan_ST[i].CH_Voltage_I16 = VolIntTem; //positive
            
        }
        // EADS   the capture voltage is same as the real circuit 
        // LMU  the capture voltage is opposite as the real circuit 
        else
        { 
            OutBuf -> VolChan_ST[i].CH_Voltage_I16 = -VolIntTem;

        }
       // OutBuf -> VolChan_ST[i].CH_Voltage_I16 = VolIntTem;
        if(LINECAlib_DEBUG == g_DebugType_EU)
        {

            printf("VolCH %d: %dV \n",i, OutBuf -> VolChan_ST[i].CH_Voltage_I16);

        }
        i++;

        if(1 == HVolIntFlag)
        {
            OutBuf -> VolChan_ST[i].CH_Voltage_I16 = HVolIntTem;           
        }
        else
        { 
            OutBuf -> VolChan_ST[i].CH_Voltage_I16 = -HVolIntTem;

        }
        if(LINECAlib_DEBUG == g_DebugType_EU)
        {

            printf("VolCH %d: %dV \n",i, OutBuf -> VolChan_ST[i].CH_Voltage_I16);

        }

        BramInBuf++;
    }
    return 0;
}
/**********************************************************************
*Name           :   uint8_t ChanLCUDataToDe(BRAM_PACKET_DATA *BramNormaSpeedPacketData_ST_p,CHAN_DATA *g_ChanData_ST_p)
*Function       :   change the Binary data of Bram to Decinal data
*Para           :   BRAM_CHAN_DATA *Bramg_ChanData_ST_p,  the Binary data
*                   CHAN_DATA *g_ChanData_ST_p            the  Decinal data
*Return         :   uint8_t 1,success;0 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*
*********************************************************************/
int8_t MAXLifeRead(uint32_t BramInBuf,CHAN_DATA *OutBuf,uint16_t TranLeng)
{
     // uint16_t TemBuf = 0;
     // uint16_t i = 0;
     // TemBuf = (BramInBuf >>16) & 0xFFFF;
     // for(i = 0;i < TranLeng;i++)
     // {
     //    OutBuf -> CurrChan_ST[i]  = TemBuf * LRTIME_FACTOR;
 
     // }
     // return 0;
}


