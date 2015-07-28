/*
 * =====================================================================================
 *
 *       Filename:  agent_core.h
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
#ifndef __AGENT_CORE_H__
#define __AGENT_CORE_H__

/* system status interface */
void Agent_reset_init(void);
void SysResetThread(void *arg);
void SysIdxLedThread(void *arg);

/* system socket interface */
void UdpServerThread(void *arg);
void TcpServerThread(void *arg);
void TcpCloudThread(void *arg);
void OtherTaskThread(void *arg);


void Agent_Init(void);

#endif

