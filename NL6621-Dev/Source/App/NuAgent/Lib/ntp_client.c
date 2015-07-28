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
            signed char local_precision;   	//表示本机时钟精度为2^local_precision秒。local_precision通常为负数。		    		    		    
            char Poll;              		//表示测试间隔为2^poll秒。
            unsigned char stratum;  		//NTP服务器阶级，0表示不指定，1表示校准用原子钟。应该为0。
            unsigned char Mode : 3; 		//通信模式。应该为3，表示是client。		    	      
            unsigned char VN   : 3; 		//NTP协议版本号。应该为3
            unsigned char LI   : 2; 		//闰秒指示，一般填0
        } ntptime;
        unsigned int headData;
    } ntptd;
}NTPHEADER; 

typedef struct NtpPacked
{
    NTPHEADER header;           	//集合了以上几个字段	
    unsigned int root_delay;    	//有符号固定点序号表示主要参考源的总延迟，很短时间内的位15到16间的分段点
    unsigned int root_dispersion;   //无符号固定点序号表示相对于主要参考源的正常差错，很短时间内的位15到16间的分段点
    unsigned int refid;         	//识别特殊参考源。
    NTPTIME  reftime;
    NTPTIME  orgtime;           	//这是向服务器请求分离客户机的时间，采用64位时标（Timestamp）格式。 
    NTPTIME  rectime;           	//这是向服务器请求到达客户机的时间，采用64位时标（Timestamp）格式。 
    NTPTIME  trantime;          	//这是向客户机答复分离服务器的时间，采用64位时标（Timestamp）格式。
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




