#include "common.h"
 
int DirectCfgFlag = 0;

unsigned char wifi_sta_connect_ap_flag = 0;

extern Agent_config_t Agent_config_data;
extern struct sys_status_t sys_status;			/* system status */

VOID AppEvtCallBack(SYS_EVT_ID event)
{
	switch (event)
	{
		case SYS_EVT_LINK_UP:
			log_info("SYS_EVT_LINK_UP\n");
       		DirectCfgFlag = 1;
			wifi_sta_connect_ap_flag = 1;
			break;
		
		case SYS_EVT_LINK_DOWN:
			log_info("SYS_EVT_LINK_DOWN\n");
			break;

		case SYS_EVT_JOIN_FAIL:
			wifi_sta_connect_ap_flag = 0;
			if (sys_status.status == SYS_STATUS_WIFI_SOFTAP) {
			   log_info("SoftAP create fail\n");
			   BSP_ChipReset();
			}

			log_info("SYS_EVT_JOIN_FAIL\n");
			break;

		case SYS_EVT_DHCP_FAIL:
			log_info("SYS_EVT_DHCP_FAIL\n");
			break;

		case SYS_EVT_SCAN_DONE:
			log_info("SYS_EVT_SCAN_DONE\n");
			break;

		case SYS_EVT_DIRECT_CFG_DONE:
			DirectCfgFlag = 1;
			log_info("SYS_EVT_DIRECT_CFG_DONE\n");
			break;

		default:
			break;
	}
}


/* 
 * SoftAP Mode
 * Set WIFI to SoftAP mode */
void DRV_WiFi_SoftAPModeStart(void)
{
    /* 
     * Create AP network.
     * */ 
    InfWiFiStop();                                                                               
    OSTimeDly(10);                                                                               

    InfLoadDefaultParam();                                                                       
    InfNetModeSet(PARAM_NET_MODE_SOFTAP);                                                        

    InfWiFiStart();
    return;
}


/* 
 * Station Mode
 * Set WIFI to station mode, link up with StaSsid and StaPass parameters.
 * flag: 0:used SysParam parameters, 1:used staSsid and StaPass parameters.
 * */
int DRV_WiFi_StaModeConnStart(char *StaSsid, char *StaPass, int flag)
{
    IP_CFG IpParam;

    /* Start Wifi STA mode and connect to the AP with StaSsid and StaPass.
     * */
	InfWiFiStop();
	OSTimeDly(10);
	if (flag == 1) {	    	
	    InfLoadDefaultParam();                                                                       
	    
	    memset(&IpParam, 0, sizeof(IpParam));                                                        
	    IpParam.DhcpTryTimes = 0;                                                                    
	    IpParam.bDhcp = 1;                                                                           
	    InfIpSet(&IpParam);                                                                          
	
		InfConTryTimesSet(5);
	    InfNetModeSet(PARAM_NET_MODE_STA_BSS);
		InfEncModeSet(PARAM_ENC_MODE_AUTO);	
	    InfSsidSet((UINT8 *)StaSsid, strlen(StaSsid));    /* set ssid */                             
	    InfKeySet(0, (UINT8 *)StaPass, strlen(StaPass));  /* set pw */                               
	
	    log_info("Connecting to AP(%s,%s)...\n\n", StaSsid, StaPass);                                                             
 
	} else {
	    InfNetModeSet(PARAM_NET_MODE_STA_BSS);
		InfConTryTimesSet(5);
	    log_info("Start wifi connection.\n");	
	}
	InfWiFiStart();	
	OSTimeDly(10);

	wifi_sta_connect_ap_flag = 2; /* poll to get wifi status change */
	while (wifi_sta_connect_ap_flag == 2) {
		OSTimeDly(10);
	}

	if (wifi_sta_connect_ap_flag == 1) {
		return 0;
	} else {
		return -1;
	}
}

void RespondConnBroadcast(void)
{
    int ret = 0;
    int count = 0;
	int trycount = 0;
	int conncount = 0;
    
    int udp_sock;
    const int opt = 1;    
    struct sockaddr_in addrto;
    int nlen = sizeof(addrto);
    char sendbuf[24] = {0}; 

    /* fill with response data */
    sendbuf[0] = 0x18;
    sendbuf[8] = 0x06;
    sendbuf[10] = 0x01;
 
AGAIN:    
    if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        log_info("UDP Broadcast Socket error\n");
        return;
    }
    
    if (setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt)) == -1) 
    {  
        log_info("set UDP Broadcast socket error...\n");  
		lwip_close(udp_sock);  
        return;  
    }  
    
    memset(&addrto, 0, sizeof(struct sockaddr_in));  
    addrto.sin_family = AF_INET;  
    addrto.sin_addr.s_addr = htonl(INADDR_BROADCAST);  
    addrto.sin_port=htons(60002);  
    
    log_info("Respond WI-FI connection info.\n");
	count = 0;
    trycount = 0;

    while (1)
    {
        ret = sendto(udp_sock, sendbuf, 24, 0, (struct sockaddr *)&addrto, nlen);
        if (ret < 0) 
        {
			trycount++;
			log_info("Send UDP Broadcast data error(ret:%d)\n", ret);
			OSTimeDly(30);
			if (trycount > 5) {
				log_info("Respond UDP Broadcast Failed.\n");
				lwip_close(udp_sock);
				conncount++;
				if (conncount > 3) {
					return;
				}
				goto AGAIN;
			} else {
            	continue;
			}
        } 
        
        /* print debug message to console */
        if (count < 20)  // broadcast 6s
        {
            if (count % 3 == 0) 
                printf("\b..");
    
            if (count % 3 == 0) 
                printf("\b\\");
            else if (count % 3 == 1) 
                printf("\b-");
            else if (count % 3 == 2) 
                printf("\b/");
                
            OSTimeDly(30);
        } 
        else
        {
            printf("\n");
            break;
        }
        count++;
    }
	lwip_close(udp_sock);

    log_info("Respond finished.\n");
}

int StartDirectConfigConn(void)
{
    UINT8 SsidLen = 0;
    UINT32 count = 0;
	int ret = 0;

	memset(&SysParam.WiFiCfg.Ssid, 0x0, strlen((const char *)SysParam.WiFiCfg.Ssid));
	memset(&SysParam.WiFiCfg.PSK, 0x0, strlen((const char *)SysParam.WiFiCfg.PSK));

    /* start direct config */
    log_info("Start directconfig...\n");
	DirectCfgFlag = 0;
    InfDirectCfgStart(0, 0,NULL);

    /* print steps */
    while (1) 
    {
        if (DirectCfgFlag == 1) 
        {
            DirectCfgFlag = 0;
            log_info("\n");
            break;
        }
        
        if (count % 10 == 0) 
            log_info("\b..");		
        
        if (count % 3 == 0) 
            log_info("\b\\");
        else if (count % 3 == 1) 
            log_info("\b-");
        else if (count % 3 == 2) 
            log_info("\b/");
            
        count++;
        OSTimeDly(30);				
    }

    /* stop direct config */
    InfWiFiStop();

    /* print the received config parameters (ssid and key) */
    SsidLen = strlen((const char *)SysParam.WiFiCfg.Ssid);
    if (SsidLen)
    	log_info("Ssid:%s(len=%d)\n", SysParam.WiFiCfg.Ssid, SsidLen);
    if (SysParam.WiFiCfg.KeyLength)
    	log_info("Key:%s(len=%d)\n", SysParam.WiFiCfg.PSK, SysParam.WiFiCfg.KeyLength);

    /* use the received parameters to start wifi connection */
    ret = DRV_WiFi_StaModeConnStart(NULL, NULL, 0);
	if (ret != 0) {
		return -1;
	}  

	/* Save SSID and password to nor flash */
	Agent_SaveSSIDAndKey((char*)SysParam.WiFiCfg.Ssid, (char*)SysParam.WiFiCfg.PSK);

    /* Send Broadcast message to inform direct config peer*/
    RespondConnBroadcast();

	return 0;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Agent_network_init
 *  Description:  Initialize agent system system connection status.
 *         Note:  This interface will check the gpio 10 level, when it was press, then
 *              set to DirectConfig mode, else change gpio to reset button. 
 * =====================================================================================
 */
void Agent_network_init(void)
{
    UINT8 gpio_val;
	int ret = 0;

	/* DirectConfig gpio is valied when set to low level */
	BSP_GPIOPinMux(USER_GPIO_DIRECTCONFIG);  
	BSP_GPIOSetDir(USER_GPIO_DIRECTCONFIG, GPIO_DIRECTION_INPUT);

    /* if the Directconfig button was pressed(gpio is low level), then set to Directconfig
     * mode.
     * */
    gpio_val = BSP_GPIOGetValue(USER_GPIO_DIRECTCONFIG);
    if (gpio_val == 0) {
        OSTimeDly(20);		/* delay 200ms, filter button shake */
        gpio_val = BSP_GPIOGetValue(USER_GPIO_DIRECTCONFIG);
        if (gpio_val == 0) {
			/* Set system status */
			sys_status.status = SYS_STATUS_WIFI_DIRECTCONFING;	
            
			/* Directconfig mode */
            ret = StartDirectConfigConn();
			if (ret == 0) {			
				/* Set system to STA mode */
				sys_status.status = SYS_STATUS_WIFI_STA_CONNECTED;
	
				Agent_reset_init();
	            return;
			}
        } 
    }	
	
	/* If directconfig function is not triggered, initialize device reset function */
	Agent_reset_init();

    /* Start the Wifi connection, setup AP or Station mode according to last connection 
	 * status store in nor flash. Otherwise, switch to softap mode if connected AP
	 * failed in station mode.
	 * */	
	if (nl6621_GetConfigData(&Agent_config_data) != 0) {
		log_info("Get GAgentConfig Data error.\n");
	}

	if ((Agent_config_data.flag & XPG_CFG_FLAG_CONNECTED) == XPG_CFG_FLAG_CONNECTED) {
        /* setup station mode */
		sys_status.status = SYS_STATUS_WIFI_DIRECTCONFING;
		
		ret = DRV_WiFi_StaModeConnStart(Agent_config_data.wifi_ssid, Agent_config_data.wifi_key, 1);
		if (ret != 0) { 	/* switch to softAP mode */
			goto SOFTAP;
		}
		sys_status.status = SYS_STATUS_WIFI_STA_CONNECTED;

	} else {
		/* setup softap mode */
SOFTAP:
		log_info("Device Working at AP mode\r\n");
        DRV_WiFi_SoftAPModeStart();
		sys_status.status = SYS_STATUS_WIFI_SOFTAP;
	}

    return;	
}

/* create WIFI mode setup semphare and register event callback.
 * InfSysEvtCBSet() interface must be set befor used wifi connection.
 **/
void Agent_wifi_init(void)
{
    InfSysEvtCBSet(AppEvtCallBack);
    InfLoadDefaultParam();		
}


