/*
 * =====================================================================================
 *
 *       Filename:  agent_core.c
 *
 *    Description:  Core code of Agent
 *
 *        Version:  0.0.1
 *        Created:  2015/6/26 10:38:05
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date          Purpose
 *        0.0.1      Lin Hui    2015/06/26     Create and Initialize
 * =====================================================================================
 */
#include "common.h"

Agent_config_t Agent_config_data;
extern struct sys_status_t sys_status;

/*********************************************/

/****************************************************************/
/****************** NuAgent LED indicater interface *************/
/****************************************************************/
void LED_GPIO_Init(void)
{   
	GPIO_InitTypeDef  GPIO_InitStructure;

	//��ʼ��LEDָʾ�ƣ�GPIO9(PIN7)		
	GPIO_InitStructure.GPIO_Pin = USER_GPIO_IDX_LED;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out; 	//�������
	GPIO_Init(&GPIO_InitStructure);
	GPIO_ResetBits(USER_GPIO_IDX_LED);			    //����͵�ƽ	
}

void update_led_indicator(unsigned int low_time, unsigned int high_time)
{
	GPIO_ResetBits(USER_GPIO_IDX_LED);				        //����͵�ƽ	
	OSTimeDly(low_time);	
	GPIO_SetBits(USER_GPIO_IDX_LED);				        //����ߵ�ƽ	
	OSTimeDly(high_time);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SysIdxLedThread
 *  Description:  Output indicator LED based on system status. 
 *         Note:
 * =====================================================================================
 */
void SysIdxLedThread(void *arg)
{
	log_notice("Create system LED indicator task success.\r\n");
	LED_GPIO_Init();

	while (1) {
		switch (sys_status.status) {
			case SYS_STATUS_WIFI_STOP:
				while (sys_status.status == SYS_STATUS_WIFI_STOP) {
                	GPIO_ResetBits(USER_GPIO_IDX_LED);				        //����͵�ƽ	
					OSTimeDly(200);
				}
				break;

			case SYS_STATUS_WIFI_SOFTAP:
				update_led_indicator(300, 100);
				break;

			case SYS_STATUS_WIFI_DIRECTCONFING:
				update_led_indicator(20, 20);
				break;

			case SYS_STATUS_WIFI_STA_CONNECTED:
				update_led_indicator(100, 100);
				break;

			case SYS_STATUS_WIFI_STA_LOGIN:
				while (sys_status.status == SYS_STATUS_WIFI_STA_LOGIN) {
					GPIO_SetBits(USER_GPIO_IDX_LED);				        //����͵�ƽ	
					OSTimeDly(200);
				}
				break;

			default:
				break;
		}		
	}
}


/****************************************************************/
/******************  Agent reset system interface  **************/
/****************************************************************/
#define USER_RESET_BUTTON_PRESS				(0)		/* 1:for qfn88; 0:for NF-210S */
#define USER_RESET_BUTTON_UP				(0)
#define USER_RESET_PRESS_TIMEOUT			(7)		/* when press 4s, reset system */

static unsigned int system_reset_flag = 0;

void Agent_reset_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Reset gpio is valied when set to low level */		 
	/* GPIO10=�������룬�ڲ�û������������Ҫ�ⲿ������   */			
	GPIO_InitStructure.GPIO_Pin = USER_GPIO_RESET_BUTTON;   //�ܽ�PIN51
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_In;        	//�������
	GPIO_Init(&GPIO_InitStructure);

	/* Set GAgent reset flag */
	system_reset_flag = 0x12345678;
}

void SysResetThread(void *arg)
{
	UINT8 gpio_val;
	int i;
	Agent_config_t  pConfigData;

    log_notice("Create system reset task success.\r\n");
	while (1) 
	{
		if (system_reset_flag == 0x12345678) 
		{
			gpio_val = GPIO_ReadInputDataBit(USER_GPIO_RESET_BUTTON);
			if (gpio_val == USER_RESET_BUTTON_PRESS) {
				for (i = 0; i < USER_RESET_PRESS_TIMEOUT; i++) {
					OSTimeDly(50);		/* delay 500ms filter button shake */
					gpio_val = GPIO_ReadInputDataBit(USER_GPIO_RESET_BUTTON);
					if (gpio_val != USER_RESET_BUTTON_PRESS) {
						break;
					}
				}
				
				if (i == USER_RESET_PRESS_TIMEOUT) {
					
					memcpy(&pConfigData, &Agent_config_data, sizeof(Agent_config_t));
					pConfigData.flag = 0;
					memset(pConfigData.wifi_ssid, 0x0, sizeof(pConfigData.wifi_ssid));
					memset(pConfigData.wifi_key, 0x0, sizeof(pConfigData.wifi_key));
	
					nl6621_SaveConfigData(&pConfigData);
					/* reset module */
					log_warn("RESET button press more than 4 seconds, reseting System.\n");
					OSTimeDly(100);
					nl6621_Reset();
				} 
			}				
		}
		OSTimeDly(100);
	}
}

void Agent_Init(void)
{
	/* Initialize WIFI base resource */
	Agent_wifi_init();

	/* Initialize NL6621 network */
	Agent_network_init();

	/* Initialize network adapter resource */
	network_adapter_init();

	/* initialize Domain semaphare */
	InitDomainSem();
    
    return;
}


/**********************************************************************/
/************** Network task thread for local or remote ***************/
/**********************************************************************/
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  UdpServerThread
 *  Description:  Task thread for UDP server would be used to process device scan,
 *				register and bind.
 *         Note:
 * =====================================================================================
 */
void UdpServerThread(void *arg)
{
	int ret = 0;

 	log_notice("Create UDP Server task success.\r\n");

	while (1) {
	    /* Create UDP server socket */
		while (1) {
			OSTimeDly(50);
		    ret = Socket_CreateUDPServer();
			if (ret < 0) {
				log_info("Create UDP server socket error.\n");
			} else {
				log_info("Create UDP server socket success.\n");
				break;
			} 
		}    

	    Socket_UDPServerRecvData();
	}
}		/* -----  end of function UdpServerThread  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TcpServerThread
 *  Description:  Task thread for TCP server which can be used to receive MQTT and Gserver
 *              communication data.
 *         Note:
 * =====================================================================================
 */
void TcpServerThread(void *arg)
{   
	int ret = 0;

   	log_notice("Create TCP Server task success.\n");
	while (1) {

		while (sys_status.status != SYS_STATUS_WIFI_STA_CONNECTED)
			OSTimeDly(50);	

	    /* Create TCP server socket */
		while (1) {
	        ret = Socket_CreateTCPServer();
			if (ret < 0) {
				OSTimeDly(50);
				log_info("Create TCP server socket error.\n");
			} else {
				log_info("Create TCP server socket success.\n");
				break;
			} 
		}

		Socket_TCPServerRecvData();
	}
}		/* -----  end of function TcpServerThread  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TcpCloudThread
 *  Description:  Task thread for TCP login MQTT cloud server 
 *         Note:
 * =====================================================================================
 */
void TcpCloudThread(void *arg)
{
//	int ret;   
 	log_notice("Create TCP Client Cloud task success.\n");
	 
    while (1) {
#if 0
		while (sys_status.status != SYS_STATUS_WIFI_STA_CONNECTED)
#else
		while (1)			
#endif
			OSTimeDly(50);

#if 0
		while (1) {
	        ret = Socket_CreateTCPClient();
			if (ret < 0) {
				OSTimeDly(50);
				log_info("Create TCP cloud client socket error.\n");
			} else {
				log_info("Create TCP cloud client socket success.\n");
				break;
			} 
		}

		Agent_cloud_process();
		while(1) 
			OSTimeDly(10);	
#endif	 
    }
}		/* -----  end of function TcpCloudThread  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  OtherTaskThread
 *  Description:  Finish user's private task, like NTP, OTA 
 *         Note:
 * =====================================================================================
 */
void OtherTaskThread(void *arg)
{
	int count = 0; 
	int ntp_ret = 0;
	log_notice("Create Other Task thread success.\n");

	while (1) 
	{
		if ((count < 5) && (ntp_ret > 0)) 
		{
			ntp_ret = start_ntp();
			count++;
		}
		else 
		{
			/* When NTP setup finish, Add OTA functionality here. */

			OSTimeDly(1000);
		}		
	}    
}		/* -----  end of function RTCThread  ----- */


