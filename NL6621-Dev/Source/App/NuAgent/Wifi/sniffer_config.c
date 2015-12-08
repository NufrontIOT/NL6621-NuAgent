/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  sniffer_config.c
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
 *
 * ====================================================================
 */
#include "common.h"
#include "sniffer_config.h"




//超时时间，参数如下
#define LINK_RSSI_MIN		    	-70
#define LINK_LOCK_CH_TIMEOUT	    30  // 50 ticks
#define LINK_PROVISION_TIMEOUT      600  // 600 ticks


snifferconfig_status snifferconfig_flag = Not_Start;
extern  unsigned char link_status;



/*
* 配置初始化,在配置前必须先调用
*/
void snifferconfig_init(void)
{
  //添加自定义代码，自主配置协议
}

/*
* 放入目标的mac和包长度进行处理
* 参数: 
*	mac: 固定6个byte的目标mac地址
*	len: 数据包长度
* 返回:
* 	skyconfig_status	
*/
snifferconfig_status snifferconfig_process(unsigned char* mac,int len)
{
    //添加自定义代码，自主配置协议 
    snifferconfig_status  status = Not_Start;

	mac = mac;
	len = len;

	return   status;
}

/*
* 获取wifi info包含ssid和密码
*/
sniffer_wifi_info* get_sniffer_wifi_info(void)
{   
   //添加自定义代码,自主配置协议
   char *info = (sniffer_wifi_info *)NULL;

   return  info; 
}
   

/*
* 分类802.11协议数据
*/
void Config802_11FrameProcess(unsigned char* frame,int len){
	struct probe_request_80211 *probe_buf = (struct probe_request_80211*)frame;
 	unsigned char  *mac = 0;

	if(probe_buf->framectrl.Type == FRAME_TYPE_DATA){
		// if(probe_buf->framectrl.Subtype & FRAME_SUBTYPE_MASK_QOS){
		// 	len -= 2;
		// }
	}else{
		return;
	}

	if(probe_buf->framectrl.ToDS == 0){
		mac = probe_buf->addr1;
	}else{
		mac = probe_buf->addr3;
	}
	snifferconfig_flag = snifferconfig_process(mac,len);
}
/*
* 嗅探功能，回调函数，主要监听AP的数据，来多少，入多少。
* 参数: 
*   无
* 返回:
* 	无	
*/
void SnifferConfig_RecvCb(UINT8 *pRecvBuf, UINT32 pRecvLen)
{   
    Config802_11FrameProcess(pRecvBuf, pRecvLen);
}
/*
* Skyworth一键配置处理函数
* 参数: 
*   无
* 返回:
* 	无	
*/
void Sniffer_config(void)
{
    sniffer_wifi_info  *wifi_info;

    UINT8  j,indexcount = 0;
	int err;
    SCAN_INFO_TABLE * pScanTable = NULL; 
	
	 
    log_info("Sniffer Config start！\n");   
    memset(SysParam.WiFiCfg.Ssid,0,32);
    memset(SysParam.WiFiCfg.PSK,0,64);

    // scan all channel to get ap list
    pScanTable = OSMMalloc(sizeof(SCAN_INFO_TABLE) + sizeof(SCAN_INFO) * (32 -1));
    if (!pScanTable)
    {
        log_err("Sniffer Config out of memory\n");
        return;
    }
 
SCAN_LOOP:
    OSTimeDly(1);
    // init Sniffer config
    snifferconfig_init();

    link_status = 0;
    snifferconfig_flag = Not_Start;

    InfSsidSet(0, 0);
    InfNetModeSet(PARAM_NET_MODE_STA_BSS);
    InfWiFiStart();
    pScanTable->InfoCount = 32;   
    InfScanStart(pScanTable, 0, 0);

    // wait scan done
    while (link_status != SYS_EVT_SCAN_DONE)
        OSTimeDly(10);
    InfWiFiStop();
    OSTimeDly(2);

    // sniff packets of every ap in the scan info list
    for (indexcount = 0; indexcount < pScanTable->InfoCount; indexcount++)
    {
        log_info("InfSnifferStart\n");
        InfSnifferStart(SnifferConfig_RecvCb, pScanTable->InfoList[indexcount].Bssid, pScanTable->InfoList[indexcount].Channel);
        
//		DBGPRINT(DEBUG_TRACE, "Sniff ssid: %s  bssid = %02X-%02X-%02X-%02X-%02X-%02X channel=%d\n",
//                       pScanTable->InfoList[indexcount].Ssid,pScanTable->InfoList[indexcount].Bssid[0], 
//					   pScanTable->InfoList[indexcount].Bssid[1], pScanTable->InfoList[indexcount].Bssid[2],
//                       pScanTable->InfoList[indexcount].Bssid[3], pScanTable->InfoList[indexcount].Bssid[4], 
//					   pScanTable->InfoList[indexcount].Bssid[5], pScanTable->InfoList[indexcount].Channel);
        for (j = 0; j < (LINK_LOCK_CH_TIMEOUT-2); j++)
        {
            // wait channel lock
            if (snifferconfig_flag >= Running_Lock) break;
            OSTimeDly(1);
        }

        if (snifferconfig_flag >= Running_Lock)  break;

        InfWiFiStop();
		OSTimeDly(2);
    }

    if (snifferconfig_flag >= Running_Lock)
    {
        for (j = 0; j < LINK_PROVISION_TIMEOUT; j++)// wait 60s
        {
            if (snifferconfig_flag >= Finish_OK)
                break;
            OSTimeDly(1);
        }
    } else {
	    InfWiFiStop();
		OSTimeDly(1);
        goto SCAN_LOOP;
	}

    if (snifferconfig_flag != Finish_OK){
        InfWiFiStop();
        goto SCAN_LOOP;
    }

    log_info("Skyworth config ok\n");
    /* stop direct config */
    InfWiFiStop();

	//得到SSID，KEY.
	wifi_info = get_sniffer_wifi_info();

	wifi_info->ssid[wifi_info->ssid_len] = 0;
	wifi_info->passwd[wifi_info->passwd_len] = 0;
    /* print the received config parameters (ssid and key) */
    if (wifi_info->ssid_len)
    	log_info("Ssid:%s(len=%d)\n", wifi_info->ssid, wifi_info->ssid_len);
    if (wifi_info->passwd_len)
    	log_info("Key:%s(len=%d)\n", wifi_info->passwd, wifi_info->passwd_len);

    /* use the received parameters to start wifi connection */
    err = DRV_WiFi_StaModeConnStart(wifi_info->ssid, wifi_info->passwd,1);
	if (err != 0) {
        InfWiFiStop();
	    OSTimeDly(2);
        goto SCAN_LOOP;
	}  

	/* Save SSID and password to nor flash */
	Agent_SaveSSIDAndKey((char*)SysParam.WiFiCfg.Ssid, (char*)SysParam.WiFiCfg.PSK);

    /* Send Broadcast message to inform direct config peer*/
    //SendDevicesInformation();

    OSMFree(pScanTable);
  
    log_info("Sniffer Config done\n");
}


