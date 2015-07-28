/*
 * =====================================================================================
 *
 *       Filename:  nl6621_uart.h
 *
 *    Description:  Head file for nl6621 receive and send data
 *
 *        Version:  0.0.1
 *        Created:  2015/7/1 13:12:55
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
#ifndef __NL6621_UART_H__
#define __NL6621_UART_H__

#define UART_RECV_BUF_SIZE		(512)
#define UART_RECV_TIMER_NUM		(200)		/* Set receive timer 100ms */

void uart_data_recv(unsigned char Dummy);

int uart_data_send(unsigned char *data, unsigned short len, unsigned char uart_type);
void UartTaskThread(void *arg);


#endif
