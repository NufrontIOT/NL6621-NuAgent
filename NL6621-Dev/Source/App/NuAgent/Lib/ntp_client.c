/*
 * =====================================================================================
 *
 *       Filename:  ntp_client.c
 *
 *    Description:  Source code for NTP client
 *
 *        Version:  0.0.1
 *        Created:  2015/3/4 9:11:57
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date          Purpose
 *        0.0.1      Lin Hui    2015/3/4      create
 *
 * =====================================================================================
 */

#include "common.h"
#include "ntp_client.h"

#define JAN_1970     	(0x83aa7e80)
#define NTPFRAC(x)      (4294 * (x) + ((1981 * (x))>>11))
#define USEC(x)         (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))

typedef struct NtpTime {
    unsigned int coarse;
    unsigned int fine;
} NTPTIME;


typedef struct ntpheader
{
    union
    {
        struct 
        {	    
            signed char local_precision;   	//��ʾ����ʱ�Ӿ���Ϊ2^local_precision�롣local_precisionͨ��Ϊ������		    		    		    
            char Poll;              		//��ʾ���Լ��Ϊ2^poll�롣
            unsigned char stratum;  		//NTP�������׼���0��ʾ��ָ����1��ʾУ׼��ԭ���ӡ�Ӧ��Ϊ0��
            unsigned char Mode : 3; 		//ͨ��ģʽ��Ӧ��Ϊ3����ʾ��client��		    	      
            unsigned char VN   : 3; 		//NTPЭ��汾�š�Ӧ��Ϊ3
            unsigned char LI   : 2; 		//����ָʾ��һ����0
        } ntptime;
        unsigned int headData;
    } ntptd;
}NTPHEADER; 

typedef struct NtpPacked
{
    NTPHEADER header;           	//���������ϼ����ֶ�	
    unsigned int root_delay;    	//�з��Ź̶�����ű�ʾ��Ҫ�ο�Դ�����ӳ٣��ܶ�ʱ���ڵ�λ15��16��ķֶε�
    unsigned int root_dispersion;   //�޷��Ź̶�����ű�ʾ�������Ҫ�ο�Դ����������ܶ�ʱ���ڵ�λ15��16��ķֶε�
    unsigned int refid;         	//ʶ������ο�Դ��
    NTPTIME  reftime;
    NTPTIME  orgtime;           	//������������������ͻ�����ʱ�䣬����64λʱ�꣨Timestamp����ʽ�� 
    NTPTIME  rectime;           	//��������������󵽴�ͻ�����ʱ�䣬����64λʱ�꣨Timestamp����ʽ�� 
    NTPTIME  trantime;          	//������ͻ����𸴷����������ʱ�䣬����64λʱ�꣨Timestamp����ʽ��
} NTPPACKED, *PNTPPACKED;


extern char *ctime( const struct timeval *timer );

int CreateNtpClientSockfd(struct sockaddr_in *addr_src)
{
    int sockfd;

    /* create socket. */
    if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0))) {		
        log_err("create socket error!\n");
        return -1;
    }
	
    addr_src->sin_family = AF_INET;
    addr_src->sin_addr.s_addr = htonl(0xCA700A24);
    addr_src->sin_port = htons(123);
    memset(&(addr_src->sin_zero), 0, sizeof(addr_src->sin_zero));

    return sockfd;
}

void SendQueryTimePacked(int sockfd, struct sockaddr_in *addr_src)
{
    NTPPACKED SynNtpPacked;
    int ticks;
    int timer;

    memset(&SynNtpPacked, 0, sizeof(SynNtpPacked));
    SynNtpPacked.header.ntptd.ntptime.local_precision = -6;
    SynNtpPacked.header.ntptd.ntptime.Poll = 4;
    SynNtpPacked.header.ntptd.ntptime.stratum = 0;
    SynNtpPacked.header.ntptd.ntptime.Mode = 3;
    SynNtpPacked.header.ntptd.ntptime.VN = 3;
    SynNtpPacked.header.ntptd.ntptime.LI = 0;
    SynNtpPacked.root_delay = 1<<16;      	/* Root Delay (seconds) */
    SynNtpPacked.root_dispersion = 1<<16; 	/* Root Dispersion (seconds) */
    SynNtpPacked.header.ntptd.headData = htonl(SynNtpPacked.header.ntptd.headData);
    SynNtpPacked.root_delay = htonl(SynNtpPacked.root_dispersion);
    SynNtpPacked.root_dispersion = htonl(SynNtpPacked.root_dispersion);
    ticks = OSTimeGet();
    timer = ticks / OS_TICKS_PER_SEC ;
    SynNtpPacked.trantime.coarse = htonl((long)timer + JAN_1970); 	/* Transmit Timestamp coarse */
    SynNtpPacked.trantime.fine = htonl(NTPFRAC(timer));  			/* Transmit Timestamp fine   */
    
	sendto(sockfd, &SynNtpPacked, sizeof(SynNtpPacked), 0, addr_src, sizeof(struct sockaddr));
}

int RecvNtpPacked(int sockfd,PNTPPACKED pSynNtpPacked,struct sockaddr_in * ServerSocket_in)
{
    int receivebytes;
    socklen_t addr_len = sizeof(struct sockaddr_in); 
   
    /* recv ntp server's response. */
    receivebytes = recvfrom(sockfd, pSynNtpPacked, sizeof(NTPPACKED), 0, (struct sockaddr *)ServerSocket_in, &addr_len);
    if(-1==receivebytes)		
    {					
        close(sockfd);			
        return -1;	
    }
    return receivebytes;
}

void GetSynTimeFromNtpPacked(PNTPPACKED pSynNtpPacked,struct timeval* new_time)
{
    NTPTIME trantime;
    trantime.coarse = ntohl(pSynNtpPacked->trantime.coarse);
    trantime.fine   = ntohl(pSynNtpPacked->trantime.fine);	
    new_time->tv_sec = trantime.coarse - JAN_1970;
    new_time->tv_usec = USEC(trantime.fine);	
}

void CloseSockfd(int sockfd)
{
    close(sockfd);
}

void GetLocalTime( struct timeval* tv)
{
    unsigned int ticks;
    ticks = OSTimeGet();
    tv->tv_sec = ticks / OS_TICKS_PER_SEC ;
    tv->tv_sec = (tv->tv_sec + 1417305600 + 28800);
    tv->tv_usec = ticks % OS_TICKS_PER_SEC / OS_TICKS_PER_SEC * 1000 ;	
}

void ModifyLocalTime(struct timeval* tv)
{
    unsigned int ticks, time;
    time = tv->tv_sec - 1417305600;
    ticks = time * OS_TICKS_PER_SEC; 
    OSTimeSet(ticks);
}

int start_ntp(void)
{
    int sockfd;
    int ret;
    fd_set fds_read;
	struct sockaddr_in addrto;
    struct timeval localtime;
    struct timeval timeout, new_time;
    NTPPACKED SynNtpPacked;

    sockfd = CreateNtpClientSockfd(&addrto);
    if ( -1 == sockfd) {
        log_err("CreateNtpClientSockfd error!\n");
        return 1;
    }
    
    /*get and set time*/
    FD_ZERO(&fds_read);
    FD_SET(sockfd, &fds_read);
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;
    ret = select(sockfd + 1, &fds_read, NULL, NULL, &timeout);
    if ((0 == ret) || !FD_ISSET(sockfd, &fds_read))	{
        /* send ntp protocol packet */
        SendQueryTimePacked(sockfd, &addrto);
		log_info("\tSend NTP package success.\n");
        return 1;
    }
    
    /* recv ntp server's response. */
    if ( -1 != RecvNtpPacked(sockfd, &SynNtpPacked, &addrto) ) {
        GetSynTimeFromNtpPacked(&SynNtpPacked, &new_time);
        ModifyLocalTime(&new_time);
        GetLocalTime(&localtime);
        log_info("\tGMT time:%s\n", (char*)ctime(&(localtime.tv_sec)));
		return 0;
    }
	return 1;
}




