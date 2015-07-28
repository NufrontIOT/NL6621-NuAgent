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
VOID WriteFlash(UINT8* pData, UINT32 DataLen, UINT32 BurnStartPos)
{
	UINT32 j;
    UINT32 WriteAddr = BurnStartPos;
    UINT32 PageCnt;
    
    PageCnt = DataLen / FLASH_SECTOR_SIZE + ((DataLen % FLASH_SECTOR_SIZE) > 0 ? 1 : 0);
    
    for (j = 0; j < PageCnt; j++) {
        QSpiFlashEraseSector(WriteAddr);
		OSTimeDly(1); 
        QSpiWriteOneSector(WriteAddr, pData);
        pData += FLASH_SECTOR_SIZE;
        WriteAddr += FLASH_SECTOR_SIZE;
    }
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
	
	Agent_config_data.flag |= XPG_CFG_FLAG_CONNECTED;
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
