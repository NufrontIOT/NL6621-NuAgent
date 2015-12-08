/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  sniffer_config.h
 *
 *    Description:  Simulation of a key configuration, using sniffer model.
 *
 *        Version:  0.0.1
 *        Created:  2015/11/25 15:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date        Purpose
 *        0.0.1      Hui Lou    2015/11/25    Create and initialize
 *        0.0.2      Hui Lou    2015/11/25    Modify webserver framework to fix serialnet
 *
 * ====================================================================
 */
#ifndef __SKYCONFIG_H__
#define __SKYCONFIG_H__



#define MAX_SSID_LEN          32
#define MAX_PASSWD_LEN        64
#define MAC_DATA_MASK         0xf0
#define MAC_DATA_IDENTIFY     0x10

#define MAC_DATA_INDEX_MASK   0x0f

#define MAC_LOCK_IDENTIFY_ONE 0x60
#define MAC_LOCK_IDENTIFY_TWO 13

typedef struct{
	char ssid[MAX_SSID_LEN+1];//+1为了打印方便
	char passwd[MAX_PASSWD_LEN+1];
	unsigned char ssid_len;
	unsigned char passwd_len;
}sniffer_wifi_info;

typedef enum{
	Not_Start = -1, //还没有开始
	Running_Unlock, //没有锁定信道
	Running_Lock,   //信道已锁定
	Finish_OK,      //数据接收完成
}snifferconfig_status;

/*
* 配置初始化,在配置前必须先调用
*/
void snifferconfig_init(void);

/*
* 放入目标的mac和包长度进行处理
* 参数: 
*	mac: 固定6个byte的目标mac地址
*	len: 数据包长度
* 返回:
* 	skyconfig_status	
*/
snifferconfig_status snifferconfig_process(unsigned char* mac,int len);

/*
* 获取wifi info包含ssid和密码
*/
sniffer_wifi_info* get_sniffer_wifi_info(void);






//ieee_802.11 数据格式定义如下
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;



#define FRAME_TYPE_MANAGEMENT 0
#define FRAME_TYPE_CONTROL    1
#define FRAME_TYPE_DATA       2

#define FRAME_SUBTYPE_MASK_QOS 0x08


typedef struct framectrl_80211
{
    //buf[0]
    uint8_t Protocol:2;
    uint8_t Type:2;
    uint8_t Subtype:4;
    //buf[1]
    uint8_t ToDS:1;
    uint8_t FromDS:1;
    uint8_t MoreFlag:1;
    uint8_t Retry:1;
    uint8_t PwrMgmt:1;
    uint8_t MoreData:1;
    uint8_t Protectedframe:1;
    uint8_t Order:1;
} framectrl_80211,*lpframectrl_80211;

typedef struct probe_request_80211
{
	struct framectrl_80211 framectrl;
	uint16_t duration;
	uint8_t addr1[6];
	uint8_t addr2[6];
	uint8_t addr3[6];
	uint16_t seq;
} probe_request, *pprobe_request;


//typedef struct tagged_parameter
//{
//	/* SSID parameter */
//	uint8_t tag_number;
//	uint8_t tag_length;
//} tagged_parameter, *ptagged_parameter;



//struct sniffer_buf{
//	struct RxControl rx_ctrl;
//	uint8_t buf[48];//包含ieee80211包头
//	u16 cnt;//包的个数
//	u16 len[1];//包的长度
//};


#endif


