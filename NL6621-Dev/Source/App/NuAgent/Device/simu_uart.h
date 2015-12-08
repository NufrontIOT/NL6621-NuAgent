/*
 * =====================================================================================
 *
 *       Filename:  simu_uart.h
 *
 *    Description:  Head file for gpio simulator UART
 *
 *        Version:  0.0.1
 *        Created:  2015/7/6 0:08:05
 *       Revision:  none
 *
 *         Author:  linhui (link), 
 *   Organization:  nufront
 *
 *--------------------------------------------------------------------------------------          
 * ChangLog:
 *  version    Author      Date           Purpose
 *  0.0.1      linhui      2015/7/6    
 *  0.0.2      Hui Lou     2015/12/07     Analog serial port to send data using timer1 and  Analog serial receive data using timer 1.  
 *
 * =====================================================================================
 */
#ifndef __SIMU_UART_H__
#define __SIMU_UART_H__




#if 1
#define	SIMU_UART_RX_PIN				(6)
#define	SIMU_UART_TX_PIN				(5)
#else
#define	SIMU_UART_RX_PIN				(29)
#define	SIMU_UART_TX_PIN				(28)
#endif


void uart_init(void);
void SimuSendOneByte(unsigned char Byte);
int simu_uart_timer_task(void);
void gpio_int_func(int portNum);
void simu_uart_init(void);
void SimuUartOutPut(void);



#endif
