/**********************************************************************
*File name 		:   Xml.c
*Copyright      :   Zhuzhou CRRC Times Electric Co. Ltd ALL Rights Reserved
*Creat Date     :   2018/8/16
*Description    :   extract the parameter of xml 
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/08/16  Create
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
#include "xml.h"
/**********************************************************************
*
*Global Variable Declare Section
*
*********************************************************************/
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
*Name           :   int8_t xml_to_mem
*Function       :   extract the information of  the file,
*Para           :   const char * xml_file, the xml file path
*                   int type  : 0 is SYS_CFG,else  is RECORD_CFG
*                                             
*Return         :   the address of the xml array
*Version        :   REV1.0.0       
*Author:        :   feng
*
*History:
*REV1.0.0     feng    2018/1/29  Create
*REV1.0.0     feng    2018/6/19  Add the offset value
*********************************************************************/
char * xml_to_mem(const char * xml_file, int type)
{
	char line[100];
	char * p;
	char ch;
	char jug;
	int k = 0;
	char * xmlarray;
	int cnt;
	int i=0, j= 0;
	if (SYS_CFG == type)
	{
		cnt = SYS_CNT;
	}
	else
	{
		cnt = RECORD_CNT;
	}
    
	FILE* file = fopen(xml_file, "r" );
	if(file == NULL)
    {
        perror("open xmlfile failed");

    }
	xmlarray = malloc(sizeof(char)*ZONE*cnt);

	while (!feof(file)) 
	{
        fgets(line, sizeof(line), file);

        if (line)
        {
        	j = 0;
        	for ( i = 0; i < strlen(line); i++)
        	{
        		/* code */
        		if (line[i] == '<')
        		{
        			j++;
        		}
        	}

      		if (j == 2)
      		{
		        strtok(line,">");
		        p=strtok(NULL,"<");
				if (p)
				{
					switch(type)
					{
						case SYS_CFG:
						{	
							if (k <=SYS_CNT)
							{
								memcpy(xmlarray+k*sizeof(char)*ZONE,p,sizeof(char)*ZONE);
								
							}
							k++;
							break;
						}

						case RECORD_CFG:
						{
							if (k <=RECORD_CNT)
							{
								memcpy(xmlarray+k*sizeof(char)*ZONE,p,sizeof(char)*ZONE);
							}
							k++;
							break;
						}
					}
					
				}
			}
		}
    }
    fclose(file);
	return xmlarray;
}
