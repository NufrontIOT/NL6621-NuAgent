/*
 * =====================================================================================
 *
 *       Filename:  common.h
 *
 *    Description:  Common head file for Gizwits    
 *
 *        Version:  0.0.1
 *        Created:  12/25/2014 09:35:55 AM
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Guangdong Nufront CSC Co., Ltd
 *
 *--------------------------------------------------------------------------------------          
 * ChangLog:
 *  version    Author      Date         Purpose
 *  0.0.1      Lin Hui    12/25/2014    create and initialize  
 *
 * =====================================================================================
 */

#ifndef __COMMON_H__
#define __COMMON_H__


#include "includes.h"

#include "lwip/memp.h"
#include "lwIP.h"
#include "lwIP/tcp.h"
#include "lwIP/udp.h"
#include "lwIP/tcpip.h"
#include "netif/etharp.h"
#include "lwIP/dhcp.h"
#include "arch/sys_arch.h"
#include "lwIP/sockets.h"
#include "lwIP/netdb.h"
#include "lwIP/dns.h"

#include "lwIP/snmp.h"
#include "lwIP/stats.h"
#include "lwip/inet_chksum.h"  /* add for inet_chksum interface */

#include "errno.h"

#include "log.h"
#include "nu_agent.h"

#include "ntp_client.h"
#include "ring_buffer.h"
#include "str_lib.h"

#include "simu_uart.h"

#include "wifi_conn.h"
#include "wifi_opt.h"

#include "agent_core.h"
#include "network_adapter.h"
#include "nuagent_drv.h"
#include "nuagent_uart.h"
#include "nuagent_ota.h"

#include "cloud.h"
#include "lan.h"


#include "bsp_conf.h"



#define malloc 		OSMMalloc
#define free 		OSMFree



#define GAGENT_FEATURE_OTA 					(0)

#define USER_GPIO_IDX_LED   				GPIO_Pin_9	       /* System led indicator. 4:for develop board, 9:for NF-210S */
#define USER_GPIO_DIRECTCONFIG              GPIO_Pin_10        /* start DirectConfig mode. 3:for develop board, 10:for NF-210S */
#define USER_GPIO_RESET_BUTTON				USER_GPIO_DIRECTCONFIG	/* System reset gpio */

#define SETBIT(x, y) (x |= (1 << y))		/* set bit y to 1 on x */
#define CLRBIT(x, y) (x &= !(1 << y)) 		/* set bit y to 0 on x */


/* structure for system status */
struct sys_status_t {

#define SYS_STATUS_WIFI_STOP				(0)		/* Not Start WI-FI */
#define SYS_STATUS_WIFI_SOFTAP				(1)		/* WI-FI SOFTAP success */
#define SYS_STATUS_WIFI_DIRECTCONFING		(2)		/* WI-FI Direct configing */
#define SYS_STATUS_WIFI_STA_CONNECTED		(3)		/* WI-FI Connecting AP success */
#define SYS_STATUS_WIFI_STA_LOGIN			(4)

	unsigned char status;

#define SYS_STATUS_ONBOARDING				(1)
#define SYS_STATUS_ONBOARDING_SUCCESS		(2)
#define SYS_STATUS_ONBOARDING_FAILED		(3)
	unsigned char onboarding;
};

#endif


