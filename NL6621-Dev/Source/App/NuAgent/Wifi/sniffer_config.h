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
	char ssid[MAX_SSID_LEN+1];//+1Ϊ�˴�ӡ����
	char passwd[MAX_PASSWD_LEN+1];
	unsigned char ssid_len;
	unsigned char passwd_len;
}sniffer_wifi_info;

typedef enum{
	Not_Start = -1, //��û�п�ʼ
	Running_Unlock, //û�������ŵ�
	Running_Lock,   //�ŵ�������
	Finish_OK,      //���ݽ������
}snifferconfig_status;

/*
* ���ó�ʼ��,������ǰ�����ȵ���
*/
void snifferconfig_init(void);

/*
* ����Ŀ���mac�Ͱ����Ƚ��д���
* ����: 
*	mac: �̶�6��byte��Ŀ��mac��ַ
*	len: ���ݰ�����
* ����:
* 	skyconfig_status	
*/
snifferconfig_status snifferconfig_process(unsigned char* mac,int len);

/*
* ��ȡwifi info����ssid������
*/
sniffer_wifi_info* get_sniffer_wifi_info(void);






//ieee_802.11 ���ݸ�ʽ��������
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
//	uint8_t buf[48];//����ieee80211��ͷ
//	u16 cnt;//���ĸ���
//	u16 len[1];//���ĳ���
//};


#endif


