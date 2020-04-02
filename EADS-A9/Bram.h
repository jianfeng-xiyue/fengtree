/**********************************************************************
*File name 		:   Bram.h
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/1/29
*Description    :   Include the bram data read and write.
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*
*
*
*********************************************************************/

/**********************************************************************
*
*Multi-Include-Prevent Sart Section
*
*********************************************************************/

#ifndef _BRAM_H
#define _BRAM_H
/**********************************************************************
*
*Include File Section
*
*********************************************************************/  
#include "BaseDefine.h"
/**********************************************************************
*
*Global Macro Define Section
*
*********************************************************************/
/*getpagesize get the pagesize of linux to make the right set,it is 1024*/
#define PAGE_SIZE  ((size_t)getpagesize())//4096,0x1000
/*(PAGE_SIZE - 1) FFF */
#define PAGE_MASK ((uint32_t)~(PAGE_SIZE - 1))//FFFF F000 
 

/*the A9 Read block data section*/
#define BRAM_A9_RD_A_TMS_BLOCK0    	  0x40000000
#define BRAM_A9_RD_A_TMS_BLOCK1  	  0x40000400    /*the block for A TMSTMS communicate with 4~7 board */
#define BRAM_A9_RD_A_TMS_BLOCK2  	  0x40000800    /*the block for A TMSTMS  communicate with 8~11 board */
#define BRAM_A9_RD_A_TMS_BLOCK3  	  0x40000C00    /*the block for A TMSTMS  communicate with 12~15 board */
#define BRAM_A9_RD_A_TMS_BLOCK4  	  0x40001000    /*the block for A TMSTMS  communicate with 16~19 board */

#define BRAM_A9_RD_A_BLVDS_BLOCK0  	  0x40002000    /*the block for A blvds communicate with 0~3 board */
#define BRAM_A9_RD_A_BLVDS_BLOCK1  	  0x40002400    /*the block for A blvds communicate with 4~7 board */
#define BRAM_A9_RD_A_BLVDS_BLOCK2  	  0x40002800    /*the block for A blvds communicate with 8~11 board */
#define BRAM_A9_RD_A_BLVDS_BLOCK3  	  0x40002C00    /*the block for A blvds communicate with 12~15 board */
#define BRAM_A9_RD_A_BLVDS_BLOCK4  	  0x40003000    /*the block for A blvds communicate with 16~19 board */


#define BRAM_A9_RD_B_TMS_BLOCK0       0x40004000

#define BRAM_A9_RD_B_BLVDS_BLOCK0  	  0x40006000    /*the block for B blvds communicate with 0~3 board */
#define BRAM_A9_RD_B_BLVDS_BLOCK1  	  0x40006400    /*the block for B blvds communicate with 4~7 board */
#define BRAM_A9_RD_B_BLVDS_BLOCK2  	  0x40006800    /*the block for B blvds communicate with 8~11 board */
#define BRAM_A9_RD_B_BLVDS_BLOCK3  	  0x40006C00    /*the block for B blvds communicate with 12~15 board */
#define BRAM_A9_RD_B_BLVDS_BLOCK4  	  0x40007000    /*the block for B blvds communicate with 16~19 board */

#define BRAM_A9_RD_A_TMS_SPCBLOCK0    0x40008000   /*the block for A TMSTMS write,for example feed back the update status*/
#define BRAM_A9_RD_A_BLVDS_SPCBLOCK0  0x4000A000 /*the block for receive the  feed back update status,When update the DI DO board programe*/

/*the A9 Read block flag section*/
#define BRAM_A9_RD_A_TMS_FLAG          0x4000E000
#define BRAM_A9_RD_A_BLVD_FLAG         0x4000E004
#define BRAM_A9_RD_B_TMS_FLAG          0x4000E008
#define BRAM_A9_RD_B_BLVD_FLAG         0x4000E00C
#define BRAM_A9_RD_A_TMS_SPCFLAG       0x4000E010
#define BRAM_A9_RD_A_BLVDS_SPCFLAG     0x4000E014

/*the A9 Read block Feed back flag section*/
#define BRAM_A9_RD_FED_A_TMS_FLAG       0x4000E100
#define BRAM_A9_RD_FED_A_BLVD_FLAG      0x4000E104
#define BRAM_A9_RD_FED_B_TMS_FLAG       0x4000E108
#define BRAM_A9_RD_FED_B_BLVD_FLAG      0x4000E10C
#define BRAM_A9_RD_FED_A_TMS_SPCFLAG    0x4000E110
#define BRAM_A9_RD_FED_A_BLVDS_SPCFLAG  0x4000E114

/*the A9 Write block data section*/
#define BRAM_A9_WR_A_TMS_SPCBLOCK0      0x80008000
#define BRAM_A9_WR_A_BLVDS_SPCBLOCK0    0x8000A000

/*the A9 Write block flag section*/
#define BRAM_A9_WR_A_TMS_FLAG           0x8000E000
//#define BRAM_A9_WR_A_BLVD_FLAG          0x8000E004
#define BRAM_A9_WR_A_TMS_SPCFLAG        0x8000E010
#define BRAM_A9_WR_A_BLVDS_SPCFLAG      0x8000E014

/*the A9 Write block Feed back flag section*/
//#define BRAM_A9_WR_ERR_A_TMS_FLAG        0x8000E100
//#define BRAM_A9_WR_ERR_A_BLVD_FLAG       0x8000E104
#define BRAM_A9_WR_ERR_A_TMS_SPCFLAG     0x8000E110
#define BRAM_A9_WR_ERR_A_BLVDS_SPCFLAG   0x8000E114

#define BRAM_PCKT_LNGTH_U8 256
#define BRAM_4BLCK_LNGTH_U8 4096
#define BRAM_PCKT_LNGTH_U32 64

#define BRAM_PCKT_TOP_LNGTH_U32 3 /*for CRC calculate */
#define BRAM_PCKT_CRC_LNGTH_U32 1



/**********************************************************************
*
*Global Structure Define Section
*
******************************************************************/
typedef struct 
{
   uint32_t Board0_Data_U32[5][50]; 
   uint32_t Board1_Data_U32[5][50]; 

}BLVDS_BRD_DATA;


/**********************************************************************
*
*Global Prototype Declare Section
*
*********************************************************************/
int8_t  BramOpen(void);
int8_t CloseBram(uint8_t *mapAdd,uint16_t size);
uint8_t *BramMap(int8_t Fd,uint32_t phyaddr,uint16_t PageSize);

uint8_t BramReadU8(uint32_t phyaddr,uint8_t *Mapbase);
int8_t BramWriteU8(uint32_t phyaddr,uint8_t *Mapbase,uint8_t val);
int8_t BramPackReadU32(uint32_t phyaddr,uint8_t *Mapbase,uint32_t *inbuf, uint16_t length);
int8_t BramPackWriteU32(uint32_t phyaddr,uint8_t *Mapbase,uint32_t *outbuf, uint16_t length);
/**********************************************************************
*
*Multi-Include-Prevent End Section
*
*********************************************************************/
#endif /*_BRAM_H*/
