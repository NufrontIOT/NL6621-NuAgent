/*
 * =====================================================================================
 *
 *       Filename:  lan.h
 *
 *    Description:  Head file of NuAgent local network
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
#ifndef __LAN_H__
#define __LAN_H__


typedef struct LAN_NETDATA_HEAD_T {
	short size;

#define LAN_NETDATA_CMD_SOFTAP_CONFIG	(0x11)
#define LAN_NETDATA_CMD_CONFIG_RESPONSE	(0x21)
#define LAN_NETDATA_CMD_DEV_DISCOVER	(0x31)
#define LAN_NETDATA_CMD_OTA_CONTROL		(0x41)

	short cmd;
	short func;
	short msg;
} LAN_NETDATA_HEAD;



void Socket_DispatchTCPData(int tcp_sock, char *recv_data, int data_len);

void Socket_DispatchUDPData(char *recv_data, 
				unsigned int recv_count, 
				struct sockaddr_in client_addr);

#endif

