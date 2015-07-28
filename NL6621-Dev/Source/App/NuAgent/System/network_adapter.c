/*
 * =====================================================================================
 *
 *       Filename:  network_adapter.c
 *
 *    Description:  The network adapter interface
 *
 *        Version:  0.0.1
 *        Created:  2015/6/26 10:18:05
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
 **/

#include "common.h"
#include "network_adapter.h"
#include "lan.h"
#include "cloud.h"

/*************************************************/
/*************** DNS Client resource *************/
/*************************************************/
OS_EVENT * pdomainSem;
static ip_addr_t domain_ipaddr;
static unsigned int domain_flag = 0;

/*************************************************/
/**************** Socket resource ****************/
/*************************************************/

/* 
 * Cloud TCP server 
 **/

#define CLOUD_SERVER_IP_ADDR		"192.168.1.10"
#define CLOUD_SERVER_PORT_NUM		(80)
static int TCPClientFd = -1;

/* 
 * TCP server 
 **/
#define TCP_SERVER_PORT              (12307)        /* LAN TCP server port */
#define TCP_SERVER_BUFFER_SIZE       (2048)	    	/* LAN TCP server receive size */	 
#define TCP_SERVER_CLIENT_MAX_NUM    (5)    		/* LAN TCP server client connect count */ 
typedef struct client_info_t {
  	int    client_fd;

	/* Add some other field by user */

} client_info;

static client_info lan_client[TCP_SERVER_CLIENT_MAX_NUM];

static int TCPServerFd = -1;
static int client_connect_count = 0;
char * TCPServerRecv_data = NULL;

/* 
 * UDP server 
 * */
#define UDP_SERVER_PORT              (60001)        /* LAN TCP server port */
#define UDP_SERVER_BUFFER_SIZE       (2048)	    	/* LAN TCP server receive size */
static int UDPServerFd = -1;
char * UDPServerRecv_data = NULL;

void InitDomainSem(void)
{
    pdomainSem = OSSemCreate(0);
}

void Gagent_ServerFound(const char *name, ip_addr_t *ipaddr, void *arg)
{
    if ((ipaddr) && (ipaddr->addr)) {
        memcpy(&domain_ipaddr, ipaddr, sizeof(ip_addr_t));
        domain_flag = 1;
        log_info("Get DNS server ip success.\n\r");

    } else {
        domain_flag = 0;
        log_info("Get DNS server ip error.\n\r");    
    }

    OSSemPost(pdomainSem);
}

int GAgent_GetHostByName(char *domain, char *IPAddress)
{
    unsigned char Err;
    ip_addr_t ip_addr;
    char str[32];

    memset(str, 0x0, sizeof(str));

    switch (dns_gethostbyname(domain, &ip_addr, Gagent_ServerFound, NULL)) {
        case ERR_OK:
			memcpy((unsigned char*)IPAddress, 
					(unsigned char*)inet_ntoa(domain_ipaddr.addr), 
					strlen(inet_ntoa(domain_ipaddr.addr)) + 1);
			log_info("Server name %s, first address: %s.\n", 
					domain, inet_ntoa(domain_ipaddr.addr));
			return 0;
        case ERR_INPROGRESS:
            log_info("dns_gethostbyname called back success.\n\r");
            break;

        default:
            log_info("dns_gethostbyname called back error.\n\r");
            return -1;
    }

    /* Waiting for connect success */
    OSSemPend(pdomainSem, 10, &Err);

    if (domain_flag == 1) {
        memcpy((unsigned char*)IPAddress, (unsigned char*)inet_ntoa(domain_ipaddr.addr), strlen(inet_ntoa(domain_ipaddr.addr)) + 1);
        log_info("Server name %s, first address: %s", 
                domain, inet_ntoa(domain_ipaddr.addr));

    } else {
        return -1;
    }

    return 0;
}

int Socket_CreateTCPClient(void)
{
    int ret = 0;
    struct sockaddr_in server_addr;
       
    if ((TCPClientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        log_err("Create cloud client socket failed.\n");
        return -1;
    }     

    memset(&server_addr, 0x0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CLOUD_SERVER_PORT_NUM);
    server_addr.sin_addr.s_addr = inet_addr(CLOUD_SERVER_IP_ADDR);
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero)); 

	log_info("Cloud server:%s(port:%d, socketfd:%d).\n\n", 
			CLOUD_SERVER_IP_ADDR, CLOUD_SERVER_PORT_NUM, TCPClientFd);

    ret = connect(TCPClientFd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 
    if (ret < 0) {
        log_err("Cloud server connected failed(ret:%d)\n", ret);
        close(TCPClientFd);
        return -1;
    }
		
	return 0;
}

int Socket_TCPClientSendData(char *sendbuf,	unsigned int len)
{
	return send(TCPClientFd, sendbuf, len, 0);
}

int Socket_TCPClientRecvData(char *recvbuf, int len)
{
    fd_set readfds;
    int recv_count = 0;
    struct timeval tmv;

    tmv.tv_sec = 1;	/* user can change this value */
	tmv.tv_usec = 0;    
    memset(recvbuf, 0, len);
    
	FD_ZERO (&readfds);
	FD_SET (TCPClientFd, &readfds);
    
	if (select((TCPClientFd + 1), &readfds, NULL, NULL, &tmv) == -1)
    {      
	    return -1;
    }
    
    if (FD_ISSET(TCPClientFd, &readfds))    
    {
        recv_count = recv(TCPClientFd, recvbuf, len, 0);        
		if((recv_count) <= 0)
		{
			log_info("TCP client(%d) receive data failed.\n", TCPClientFd);
    		close(TCPClientFd);
			return -2;
		}         
    }
    return recv_count;
}

int Socket_CreateTCPServer(void)
{
    struct sockaddr_in addr;

    if (TCPServerFd == -1)
    {
        TCPServerFd = socket(AF_INET, SOCK_STREAM, 0);
        if (TCPServerFd < 0)
        {
            log_err("Create TCP server socket failed, errno:%d\n", errno);
            TCPServerFd = -1;
            return -1;
        }

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(TCP_SERVER_PORT);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(TCPServerFd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
        {
            log_err("Bind TCP server failed, errno:%d\n", errno);
            close(TCPServerFd);
            TCPServerFd = -1;
            return -1;
        }

        if (listen(TCPServerFd, 0) != 0)
        {
            log_err("Listen TCP server failed, errno:%d.\n", errno);
            close(TCPServerFd);
            TCPServerFd = -1;
            return -1;
        } 
	    log_debug("TCP Server socketid:%d(port:%d).\n", TCPServerFd, TCP_SERVER_PORT);
	
	    return TCPServerFd;
    } else {
        //log_debug("TCP server socket exist, create UDP server failed.\n");
        return -1;
    }
}


int Socket_TCPServerRecvData(void)
{
	int ret, i;
	int new_fd;
	int max_sock;
	int recv_count;
    fd_set fdset;
    struct sockaddr_in client_addr;
	socklen_t sin_size = sizeof(struct sockaddr_in);

	max_sock = TCPServerFd;
	
	/* init phone client storage table */
	for (i = 0; i < TCP_SERVER_CLIENT_MAX_NUM; i++) {
		lan_client[i].client_fd = -1;			
	} 

    while (1) {	
        FD_ZERO(&fdset); 
        FD_SET(TCPServerFd, &fdset);      
        for (i = 0; i < TCP_SERVER_CLIENT_MAX_NUM; i++) {
            if (lan_client[i].client_fd >= 0) {
                FD_SET(lan_client[i].client_fd , &fdset);
            }
        }
		      
        ret = select(max_sock + 1, &fdset, NULL, NULL, NULL);
        if (ret < 0) {
            log_err("TCP server select error!\n");
            continue;
        } else if (ret == 0) {		
            continue;
        }

		/* judge if not the client in the client table. */
		for (i = 0; i < TCP_SERVER_CLIENT_MAX_NUM; i++) {
            if (FD_ISSET(lan_client[i].client_fd, &fdset)) {
                recv_count = recv(lan_client[i].client_fd, TCPServerRecv_data, TCP_SERVER_BUFFER_SIZE, 0);
                if (recv_count <= 0) {
					client_connect_count--;       
                    FD_CLR(lan_client[i].client_fd, &fdset);

                    log_debug("TCP server close client socket(%d), connect num(%d)\n",
                            lan_client[i].client_fd, client_connect_count);              
					lan_client[i].client_fd = -1;

                } else {  
                    log_debug("TCP server recv client(%d) data(len:%d) success.\n", 
						lan_client[i].client_fd, recv_count);

					/* parse TCP data come from TCP client. */
					Socket_DispatchTCPData(lan_client[i].client_fd, TCPServerRecv_data, recv_count);						                      
                }
            }
        }

		if (FD_ISSET(TCPServerFd, &fdset)) {  
			/* 
			 * the phone client is not login, need to record data. and return the login
			 * status to phone client.
			 */
            new_fd = accept(TCPServerFd, (struct sockaddr *)&client_addr, &sin_size);
            if (new_fd < 0) {
                log_err("accept socket error!");
                continue;
            }

			if (client_connect_count < TCP_SERVER_CLIENT_MAX_NUM) {
				/* get empty client in phone client table */
				for (i = 0; i < TCP_SERVER_CLIENT_MAX_NUM; i++) {
					if (lan_client[i].client_fd == -1) {
						break;
					}				
				}

				max_sock = (max_sock < (new_fd + 1)) ? (new_fd + 1) : max_sock;
				lan_client[i].client_fd = new_fd;
				ret = recv(new_fd, TCPServerRecv_data, TCP_SERVER_BUFFER_SIZE, 0);
	
	            log_debug("TCP connect client:%s:%d, receive data length:%d\n", 
	                    inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), ret);
				
				/* parse TCP data come from TCP client and return. */
				Socket_DispatchTCPData(new_fd, TCPServerRecv_data, ret);
			}			
        }
	}
}

int Socket_TCPServerSendData(int socketfd,
				char *sendbuf, 
				unsigned int len)
{
	return send(socketfd, sendbuf, len, 0);
}

int Socket_CreateUDPServer(void)
{
    struct sockaddr_in addr;

    if (UDPServerFd == -1) 
    {
        UDPServerFd = socket(AF_INET, SOCK_DGRAM, 0);
        if (UDPServerFd < 1)
        {
            log_err("Create UDP server failed, errno:%d.\n", errno);
            UDPServerFd = -1;
            return -1;
        }

        memset(&addr, 0x0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(UDP_SERVER_PORT);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(UDPServerFd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
        {
            log_err("Bind UDP server failed, errno:%d\n", errno);
            close(UDPServerFd);
            UDPServerFd = -1;
            return -1;
        }
		log_debug("UDP Server socketid:%d(port:%d)\n", UDPServerFd, UDP_SERVER_PORT);

		return UDPServerFd;
    } else {
		//log_debug("UDP server socket exist, create UDP server failed.\n");
        return -1;
    }
}

int Socket_UDPServerRecvData(void)
{
	int recv_count;
    struct sockaddr_in client_addr;
    socklen_t addrLen = sizeof(struct sockaddr_in);

	if (UDPServerFd <= 0) return -1;

	while (1) 
	{
        recv_count = recvfrom(UDPServerFd, 
						UDPServerRecv_data, 
						UDP_SERVER_BUFFER_SIZE, 0,
						(struct sockaddr *)&client_addr, 
						&addrLen);
		log_info("Recv UDP client(%s) data(len:%d).\n", inet_ntoa(client_addr.sin_addr), recv_count);
		if (recv_count <= 0) {
			log_info("Receive UDP server data error.\n");
			continue;	
		} else {
			Socket_DispatchUDPData(UDPServerRecv_data, recv_count, client_addr);
		}			
	}	
}

/* The addr parameter is UDP client sockaddr, the same as client_addr parameter of 
 * Socket_DispatchUDPData interface.
 * */
int Socket_UDPServerSendData(char *sendbuf, 
				unsigned int len, 
				void *addr, 
				int addr_size)
{
	return sendto(UDPServerFd, sendbuf, len, 0, (const struct sockaddr*)addr, addr_size);
}

void network_adapter_init(void)
{
	TCPServerFd = -1;
	UDPServerFd = -1;

	/* Alloc TCP server recv buffer */
    TCPServerRecv_data = malloc(TCP_SERVER_BUFFER_SIZE);
	UDPServerRecv_data = malloc(UDP_SERVER_BUFFER_SIZE);
}

