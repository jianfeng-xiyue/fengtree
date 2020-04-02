/**********************************************************************
*File name      :   GPIOControl.c
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/8/1
*Description    :   Include file read and write the GPIO of PS
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/8/1 Create
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
#include "GPIOControl.h" 
 
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/
extern DEBUG_TYPE_ENUM g_DebugType_EU;
extern int8_t  g_gpio_PowDown;
extern int8_t  g_gpio_Rs485;

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


// The specific GPIO being used must be setup and replaced thru
// this code.  The GPIO of 916 is in the path of most the sys dirs
// and in the export write.
//
// Figuring out the exact GPIO was not totally obvious when there
// were multiple GPIOs in the system. One way to do is to go into
// the gpiochips in /sys/class/gpio and view the label as it should
// reflect the address of the GPIO in the system. The name of the
// the chip appears to be the 1st GPIO of the controller.
//
// The export causes the gpio916 dir to appear in /sys/class/gpio.
// Then the direction and value can be changed by writing to them.
 
// The performance of this is pretty good, using a nfs mount,
// running on open source linux, on the ML507 reference system,
// the GPIO can be toggled about every 4 usec.
 
// The following commands from the console setup the GPIO to be
// exported, set the direction of it to an output and write a 1
// to the GPIO.
//
// bash> echo 916 > /sys/class/gpio/export
// bash> echo out > /sys/class/gpio/gpio916/direction
// bash> echo 1 > /sys/class/gpio/gpio916/value
 
// if sysfs is not mounted on your system, the you need to mount it
// bash> mount -t sysfs sysfs /sys
 
// the following bash script to toggle the gpio is also handy for
// testing
//
// while [ 1 ]; do
//  echo 1 > /sys/class/gpio/gpio916/value
//  echo 0 > /sys/class/gpio/gpio916/value
// done
/**********************************************************************
*Name           :   int8_t GPIO_SysFlOpen(void)          
*Function       :   Open the Io sysfs               
*Para           :   
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/8/1  Create
*********************************************************************/
int8_t GPIO_SysFlOpen(void)
{
    int16_t exportfd, directionfd;
    int8_t WrRdErr = 0;
 
    // The GPIO has to be exported to be able to see it
    // in sysfs
    exportfd = open("/sys/class/gpio/export", O_WRONLY);
    if (exportfd < 0)
    {
        printf("Cannot open GPIO to export it\n");
        return -1;
    }//if have operate once  /sys/class/gpio/export,then write exportfd will write error
    WrRdErr = write(exportfd, GPIO_POWDOW, 4); //4 mean the write length
    if(WrRdErr == -1)
    {
        printf("write exportfd error\n");
        return -1;
    }
    close(exportfd);
    if(g_DebugType_EU == GPIO_DEBUG)
    {
        printf("GPIO exported successfully\n");
    }

    // Update the direction of the GPIO to be an output
    directionfd = open(GPIO_POWDOW_DIREC, O_RDWR);
    if (directionfd < 0)
    {
        printf("Cannot open GPIO direction it\n");
        return -1;
    }
    write(directionfd, "in", 3);
    if(WrRdErr == -1)
    {
        printf("write directionfd error\n");
        return -1;
    }
    close(directionfd);
    if(g_DebugType_EU == GPIO_DEBUG)
    {
        printf("GPIO direction set as output successfully\n");       
    }

}
/**********************************************************************
*Name           :   int8_t GPIO_SysFlOpen(void)          
*Function       :   Open the Io sysfs               
*Para           :   
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/8/1  Create
*********************************************************************/
int8_t GPIO_RS485Open(void)
{
    int16_t exportfd, directionfd;
    int8_t WrRdErr = 0;
 
    // The GPIO has to be exported to be able to see it
    // in sysfs
    exportfd = open("/sys/class/gpio/export", O_WRONLY);
    if (exportfd < 0)
    {
        printf("Cannot open GPIO to export it\n");
        return -1;
    }//if have operate once  /sys/class/gpio/export,then write exportfd will write error
    WrRdErr = write(exportfd, GPIO_RS485EN, 4); //4 mean the write length
    if(WrRdErr == -1)
    {
        printf("write exportfd error\n");
        return -1;
    }
    close(exportfd);
    if(g_DebugType_EU == GPIO_DEBUG)
    {
        printf("GPIO exported successfully\n");
    }

    // Update the direction of the GPIO to be an output
    directionfd = open(GPIO_RS485EN_DIREC, O_RDWR);
    if (directionfd < 0)
    {
        printf("Cannot open GPIO direction it\n");
        return -1;
    }
    write(directionfd, "out", 4);
    if(WrRdErr == -1)
    {
        printf("write directionfd error\n");
        return -1;
    }
    close(directionfd);
    if(g_DebugType_EU == GPIO_DEBUG)
    {
        printf("GPIO direction set as output successfully\n");       
    }

}
/**********************************************************************
*Name           :   int8_t GPIO_PowDowOpen(uint8_t value)            
*Function       :   Open the Io sysfs,and open the gpio power down IO value                
*Para           :   
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/8/1  Create
*********************************************************************/
int8_t GPIO_PowDowOpen(void)
{

    g_gpio_PowDown = open(GPIO_POWDOW_VALUE, O_RDWR);
    if (g_gpio_PowDown < 0)
    {
        printf("Cannot open GPIO POWDOW value\n");
        return -1;
    }
    return 0;

}
/**********************************************************************
*Name           :   int8_t GPIO_PowDowOpen(uint8_t value)            
*Function       :   Open the Io sysfs,and open the gpio power down IO value                
*Para           :   
*Return         :   int8_t 0,success;-1 false.
*Version        :   REV1.0.0       
*Author:        :   feng
*History:
*REV1.0.0     feng    2018/8/1  Create
*********************************************************************/
int8_t GPIO_RS485EnOpen(void)
{

    g_gpio_Rs485 = open(GPIO_RS485EN_VALUE, O_RDWR);
    if (g_gpio_Rs485 < 0)
    {
        printf("Cannot open GPIO RS485EN value\n");
        return -1;
    }
    return 0;

}
/**********************************************************************
*Name           :   int8_t SetOneCMDPackto570(uint8_t ChanNum,uint8_t ChanCMD,uint16_t ChanPackNum)             
*Function       :   Write the CMD packet to 570                 
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
int8_t GPIO_RS484EnWrite(uint8_t value)
{
    
    int8_t WrRdErr = 0;
    char GPIOValueChar[10] = {0};
    sprintf(GPIOValueChar,"%d",value); 
    lseek(g_gpio_Rs485,0,SEEK_SET);//set the file start site
    WrRdErr = write(g_gpio_Rs485,GPIOValueChar,2);
    if(WrRdErr == -1)
    {
        printf("Write error\n");
        return -1;
    }
    if(g_DebugType_EU == GPIO_DEBUG)
    {
        printf("GPIO GPIOValue %s\n",GPIOValueChar);
    }
    return 0;

}

/**********************************************************************
*Name           :   int8_t SetOneCMDPackto570(uint8_t ChanNum,uint8_t ChanCMD,uint16_t ChanPackNum)             
*Function       :   Write the CMD packet to 570                 
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
int8_t GPIO_PowDowRead(uint8_t *value)
{
    
    int8_t WrRdErr = 0;
    char GPIOValueChar[2] = {0}; 
    lseek(g_gpio_PowDown,0,SEEK_SET);//set the file start site
    WrRdErr = read(g_gpio_PowDown,GPIOValueChar,2);
    if(WrRdErr == -1)
    {
        printf("Read error\n");
        return -1;
    }
    if(g_DebugType_EU == GPIO_DEBUG)
    {
        printf("GPIO GPIOValue %s\n",GPIOValueChar);
    }
    *value = (GPIOValueChar[0] - 48);
    return 0;

}
/**********************************************************************
*Name           :   int8_t SetOneCMDPackto570(uint8_t ChanNum,uint8_t ChanCMD,uint16_t ChanPackNum)             
*Function       :   Write the CMD packet to 570                 
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

int8_t GPIO_SysFlWrLow(void)
{
    int16_t exportfd, directionfd;
    int8_t WrRdErr = 0;
 
    // The GPIO has to be exported to be able to see it
    // in sysfs
    exportfd = open("/sys/class/gpio/export", O_WRONLY);
    if (exportfd < 0)
    {
        printf("Cannot open GPIO to export it\n");
        //return -1;
    }//if have operate once  /sys/class/gpio/export,then write exportfd will write error
    WrRdErr = write(exportfd, GPIO_POWDOW, 4); //4 mean the write length
    if(WrRdErr == -1)
    {
        printf("write exportfd error\n");
        //return -1;
    }
    close(exportfd);
    if(g_DebugType_EU == GPIO_DEBUG)
    {
        printf("GPIO exported successfully\n");
    }

    // Update the direction of the GPIO to be an output
    directionfd = open(GPIO_POWDOW_DIREC, O_RDWR);
    if (directionfd < 0)
    {
        printf("Cannot open GPIO direction it\n");
        //return -1;
    }
    /*set the gpio is low*/
    write(directionfd, "out", 4);
    if(WrRdErr == -1)
    {
        printf("write directionfd error\n");
        //return -1;
    }
    close(directionfd);
    g_gpio_PowDown = open(GPIO_POWDOW_VALUE, O_RDWR);
    if (g_gpio_PowDown  < 0)
    {
        printf("Cannot open GPIO POWDOW value\n");
        //return -1;
    }
    write(g_gpio_PowDown,"0", 2);
    close(g_gpio_PowDown);

    if(g_DebugType_EU == GPIO_DEBUG)
    {
        printf("GPIO direction set as output successfully\n");       
    }

}
/**********************************************************************
*Name           :   int *LEDBilnk(void *dummy)             
*Function       :   Write the CMD packet to 570                 
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


