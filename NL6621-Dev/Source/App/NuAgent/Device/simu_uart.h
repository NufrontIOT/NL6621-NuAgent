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

#include "gpio.h"


#if 1
#define	SIMU_UART_RX_PIN				(GPIO_Pin_6)
#define	SIMU_UART_TX_PIN				(GPIO_Pin_5)
#else
#define	SIMU_UART_RX_PIN				(GPIO_Pin_29)
#define	SIMU_UART_TX_PIN				(GPIO_Pin_28)
#endif

#define SIMU_UART_TX_HIGH()               GPIO_SetBits(SIMU_UART_TX_PIN)	
#define SIMU_UART_TX_LOW()                GPIO_ResetBits(SIMU_UART_TX_PIN)	

#define SIMU_UART_GET() 				  GPIO_ReadInputDataBit(SIMU_UART_RX_PIN)


void uart_init(void);
void SimuSendOneByte(unsigned char Byte);
int simu_uart_timer_task(void);
void Simu_UartIRQ_Func(void);
void simu_uart_init(void);
void SimuUartOutPut(void);



#endif
