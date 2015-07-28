/*
 * =====================================================================================
 *
 *       Filename:  network_adapter.h
 *
 *    Description:  Head file of NuAgent core
 *
 *        Version:  0.0.1
 *        Created:  2015/6/26 14:12:55
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date          Purpose
 *        0.0.1      Lin Hui    2015/6/26      
 *
 * =====================================================================================
 */
#ifndef __NETWORK_ADAPTER_H__
#define __NETWORK_ADAPTER_H__

/* DNS client */
void InitDomainSem(void);
int GAgent_GetHostByName(char *domain, char *IPAddress);

/* TCP client of cloud */
int Socket_CreateTCPClient(void);

/* TCP server */
int Socket_CreateTCPServer(void);
int Socket_TCPServerRecvData(void);

/* UDP server */
int Socket_CreateUDPServer(void);
int Socket_UDPServerRecvData(void);
int Socket_UDPServerSendData(char *sendbuf, 
				unsigned int len, 
				void *addr, 
				int addr_size);


/* network adapter initialize */
void network_adapter_init(void);

#endif

