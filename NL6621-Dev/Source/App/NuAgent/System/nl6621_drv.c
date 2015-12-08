/*
 * =====================================================================================
 *
 *       Filename:  nl6621_drv.c
 *
 *    Description:  nl6621 base driver file
 *
 *        Version:  0.0.1
 *        Created:  2015/7/1 9:32:44
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date          Purpose
 *        0.0.1      Lin Hui    2015/7/1      
 *
 * =====================================================================================
 */
#include "common.h"

extern Agent_config_t Agent_config_data;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ReadFlash
 *  Description:  Read data from nor flash. 
 * =====================================================================================
 */
VOID ReadFlash(UINT8* pBuf, UINT32 DataLen, UINT32 ReadStartPos)
{
    QSpiFlashRead(ReadStartPos, pBuf, DataLen);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  writeFlash
 *  Description:  write data to nor flash 
 * =====================================================================================
 */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  writeFlash
 *  Description:  write data to nor flash 
 * =====================================================================================
 */
static VOID WriteFlash(UINT8* pData, UINT32 DataLen, UINT32 BurnStartPos)
{
	UINT8  sector_num;	//扇区地址   GD25q41 0~127
	UINT16 sector_off;	//在扇区内的偏移
	UINT16 secre_main;	//扇区剩余空间大小   
 	UINT16 i;    
	UINT32 WriteAddr = BurnStartPos;	
    UINT8  *flash_buf = NULL; //flash缓冲区
	UINT32 Temporary_var = 0;//临时变量，防止优化


	sector_num = WriteAddr/FLASH_SECTOR_SIZE;
	sector_off = WriteAddr%FLASH_SECTOR_SIZE;
	secre_main = FLASH_SECTOR_SIZE-sector_off;  
	
	flash_buf = OSMMalloc(FLASH_SECTOR_SIZE); 

	if(flash_buf == NULL)
	{
	   printf("flash_buf malloc fail!\r\n");
	}

	if(DataLen <= secre_main){
		  secre_main = DataLen;//不大于一个扇区字节数
	}

	do
	{	
		Temporary_var =  sector_num*FLASH_SECTOR_SIZE;
		QSpiFlashRead(Temporary_var, flash_buf, FLASH_SECTOR_SIZE);//读出整个扇区的内容
		for(i=0; i<secre_main; i++){
		    Temporary_var = sector_off+i;
			if(flash_buf[Temporary_var] != 0XFF)  break;//需要擦除  	  
		}
		if(i < secre_main){
			Temporary_var = sector_num*FLASH_SECTOR_SIZE;
			QSpiFlashEraseSector(Temporary_var);//擦除这个扇区
			//OSTimeDly(2);
		    NSTusecDelay(100000); //延时

			for(i=0; i<secre_main; i++){
			    Temporary_var = sector_off+i;
				flash_buf[Temporary_var] = pData[i];	  
			}
		    Temporary_var = sector_num*FLASH_SECTOR_SIZE;
			if(QSpiWriteAny(Temporary_var, flash_buf, FLASH_SECTOR_SIZE) != 0)
			   break;//写入整个扇区  
		}
		else{
	    	if(QSpiWriteAny(WriteAddr, pData, secre_main) != 0)//写已经擦除了的,直接写入扇区剩余区间. 
	           break;	
		} 
					   
		if(DataLen == secre_main){
		   break;//写入结束了
		}
		else{//写入未结束
			sector_num++;//扇区地址增1
			sector_off = 0;//偏移位置为0 	 

		   	pData +=   secre_main;  //指针偏移
			WriteAddr += secre_main;  //写地址偏移	   	   
		   	DataLen -=   secre_main;  //字节数递减
			if(DataLen > FLASH_SECTOR_SIZE) 
			    secre_main = FLASH_SECTOR_SIZE;	//下一个扇区还是写不完
			else 
			    secre_main = DataLen;			//下一个扇区可以写完了
		}	 
	}while(1); 
	
	OSMFree(flash_buf); 	
}


#define AGENT_CONFIG_DATA_ADDR      (0x64000)  /* about 400k Bytes offset */
//#define AGENT_CONFIG_DATA_ADDR      (0x68000)  /* about 416k Bytes offset */

/* 
 * |-- flag --|-- Agent_config_t --|
 * Read gagent config data, when flag set to 0x87654321, it means the Config data had set, 
 * otherwise means there is no gagent config data.
 * */
int nl6621_GetConfigData(Agent_config_t *pConfig)
{
	ReadFlash((UINT8 *)pConfig, sizeof(Agent_config_t), AGENT_CONFIG_DATA_ADDR);
	return 0;
}


int nl6621_SaveConfigData(Agent_config_t *pConfig)
{
	WriteFlash((UINT8*)pConfig, sizeof(Agent_config_t), AGENT_CONFIG_DATA_ADDR);
    return 0;
}

void debug_gagent_config_data(void)
{
	Agent_config_t temp;
	memset(&temp, 0, sizeof(Agent_config_t));
	OSTimeDly(10);
	nl6621_GetConfigData(&temp);
	log_info("Get config data: MAGIC Number:%d, flag:0x%x, SSID:%s, password:%s.\n\n", 
			temp.magicNumber, temp.flag, temp.wifi_ssid, temp.wifi_key);
}


void nl6621_GetWiFiMacAddress(unsigned char *pMacAddress)
{
//#define USE_REAL_MAC_ADDR
#ifdef USE_REAL_MAC_ADDR
    memcpy((unsigned char *)pMacAddress, PermanentAddress, MAC_ADDR_LEN);
#else
	unsigned char testAddr[6] = {0x2c, 0x8a, 0x72, 0xc9, 0x11, 0x5a};
	memcpy((unsigned char *)pMacAddress, testAddr, MAC_ADDR_LEN);
#endif
}

void Agent_SaveSSIDAndKey(char *pSSID, char *pKey)
{
    memset(Agent_config_data.wifi_ssid,0,32);
    memset(Agent_config_data.wifi_key,0,32);
    
    memcpy((unsigned char*)Agent_config_data.wifi_ssid, (unsigned char*)pSSID, 32);
    memcpy((unsigned char*)Agent_config_data.wifi_key, (unsigned char*)pKey, 32);
	
	Agent_config_data.flag = XPG_CFG_FLAG_CONNECTED;
    nl6621_SaveConfigData(&Agent_config_data);	
      	
    log_info("Save agent config data successful.\n");
    return;
}

unsigned int nl6621_GetTime_MS(void)
{     
	return (OSTimeGet() % (100)) * 10;
}

unsigned int nl6621_GetTime_S(void)
{
    return (OSTimeGet() / (100));
}

void nl6621_Reset(void)
{
    BSP_ChipReset();
    return;
}
