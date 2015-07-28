/*
 * =====================================================================================
 *
 *       Filename:  wifi_conn.h
 *
 *    Description:  Head file for WI-FI connnection
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
#ifndef __WIFI_CONN_H__
#define __WIFI_CONN_H__

int DRV_WiFi_StaModeConnStart(char *StaSsid, char *StaPass, int flag);

void RespondConnBroadcast(void);

void Agent_network_init(void);
void Agent_wifi_init(void);

#endif

