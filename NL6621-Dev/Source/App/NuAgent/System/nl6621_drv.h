#ifndef __NL6621_DRV_H__
#define __NL6621_DRV_H__

typedef struct {
	 unsigned int boot_addr;		
	 unsigned short boot_flag;			
	 unsigned short boot_status;
	 unsigned int fw1_times;
	 unsigned int fw2_times;
}__attribute__((packed))BOOT_PARAM;

VOID ReadFlash(UINT8* pBuf, UINT32 DataLen, UINT32 ReadStartPos);
VOID WriteFlash(UINT8* pData, UINT32 DataLen, UINT32 BurnStartPos);

int nl6621_GetConfigData(Agent_config_t *pConfig);
int nl6621_SaveConfigData(Agent_config_t *pConfig);
void nl6621_GetWiFiMacAddress(unsigned char *pMacAddress);

void Agent_SaveSSIDAndKey(char *pSSID, char *pKey);

unsigned int nl6621_GetTime_MS(void);
unsigned int nl6621_GetTime_S(void);
void nl6621_Reset(void);

#endif  /* __nl6621_DRV_H__ */
