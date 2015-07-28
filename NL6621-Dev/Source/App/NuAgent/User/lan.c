/*
 * =====================================================================================
 *
 *       Filename:  lan.C
 *
 *    Description:  Lan network
 *
 *        Version:  0.0.1
 *        Created:  2015/7/1 14:12:55
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
#include "lan.h"

#define UDP_SCAN_RESPONSE_TIME                   (5) 

extern Agent_config_t Agent_config_data;
extern struct sys_status_t sys_status;

/* Dispatch TCP data */
void Socket_DispatchTCPData(int tcp_sock, char *recv_data, int data_len)
{
	
}


void lan_softap_config(char *data, int data_len)
{
	int ssid_len, passwdlen, ret = 0;
	char ssid[33] = {0};
	char passwd[65] = {0};

	ssid_len = data[0];
	if (ssid_len > 32 || ssid_len <= 0) {
		log_info("SSID length(%d) invalid.\n", ssid_len);
		return;
	}
	memcpy((unsigned char*)ssid, (unsigned char*)(data + 1), ssid_len);
	
	passwdlen = data[ssid_len + 1];
	if (passwdlen > 64) {
		log_info("password length invaild\r\n");
		return;
	}
	memcpy((unsigned char*)passwd, (unsigned char*)(data + 1 + ssid_len + 1), passwdlen);
	log_info("ssid:%s, password:%s.\r\n", ssid, passwd);

	sys_status.onboarding = SYS_STATUS_ONBOARDING;	
	ret = DRV_WiFi_StaModeConnStart(ssid, passwd, 1);
	if (ret == 0) {
		sys_status.onboarding = SYS_STATUS_ONBOARDING_SUCCESS;
		/* Save SSID and password to nor flash */
		Agent_SaveSSIDAndKey(ssid, passwd);
	
	    /* Send Broadcast message to inform direct config peer*/
	    RespondConnBroadcast();

		BSP_ChipReset();
	}
}

void lan_device_discover_response(struct sockaddr_in client_addr)
{
    int count = 0;
    char *sendbuf = "ok";

    while (1) 
    {
		Socket_UDPServerSendData(sendbuf, strlen(sendbuf), &client_addr, sizeof(struct sockaddr_in));
        if (count < UDP_SCAN_RESPONSE_TIME) 
            OSTimeDly(5);
        else
            break;
        count++;
    }
}


/* Dispatch UDP data  */
void Socket_DispatchUDPData(char *recv_data, 
				unsigned int recv_count, 
				struct sockaddr_in client_addr)
{
	LAN_NETDATA_HEAD head_data;

	/* Dispatch LAN network head data(8 bytes) */
	memcpy(&head_data, (unsigned char*)recv_data, sizeof(LAN_NETDATA_HEAD));
	log_debug("size:0x%02x, cmd:0x%02x, func:0x%02x, msg:0x%02x.\n", 
			head_data.size,
			head_data.cmd,
			head_data.func,
			head_data.msg);

	switch (head_data.cmd) {
		case LAN_NETDATA_CMD_SOFTAP_CONFIG:
			lan_softap_config(recv_data + sizeof(LAN_NETDATA_HEAD), recv_count - sizeof(LAN_NETDATA_HEAD));
			break;

		case LAN_NETDATA_CMD_DEV_DISCOVER:
			log_notice("Discover device.\n");
			lan_device_discover_response(client_addr);
			break;

#if 0
		case LAN_NETDATA_CMD_OTA_CONTROL:
			log_notice("Discover device.\n");
			break;
#endif

		default:
			log_err("Lan data package can not dispatch.\n");
			break;
	}
}
