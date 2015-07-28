/*
 * =====================================================================================
 *
 *       Filename:  simu_uart.c
 *
 *    Description:  GPIO simulator UART
 *
 *        Version:  0.0.1
 *        Created:  2015/7/16 10:18:05
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date          Purpose
 *        0.0.1      Lin Hui    2015/07/16     Create and Initialize
 * =====================================================================================
 */
#include "common.h"
#include "simu_uart.h"

#define UART_REAL_PIN    	0

#define IRDA_DATA_MAX_LEN		(116)

#define	SIMU_UART_TX_DATA_H()		BSP_GPIOSetValue(SIMU_UART_TX_PIN, 1)
#define	SIMU_UART_TX_DATA_L()		BSP_GPIOSetValue(SIMU_UART_TX_PIN, 0)
#define SIMU_UART_RX_INPUT()		{BSP_GPIOPinMux(SIMU_UART_RX_PIN); BSP_GPIOSetDir(SIMU_UART_RX_PIN, 0);}


int data_cnt = 1;
unsigned char byte = 0x0;
unsigned char recv_stop_flag = 1;
unsigned int bytes_cnt = 0;


/* initialize Uart transmit gpio */
void simu_uart_tx_init(void) 
{
	BSP_GPIOPinMux(SIMU_UART_TX_PIN);
	BSP_GPIOSetDir(SIMU_UART_TX_PIN, 1);		/* output */
	BSP_GPIOSetValue(SIMU_UART_TX_PIN, 1);		/* high level */
}

/* initialize Uart receive gpio */
void simu_uart_rx_init(void) {

    BSP_GPIOPinMux(SIMU_UART_RX_PIN);
	BSP_GPIOSetDir(SIMU_UART_RX_PIN, 0);
                                                        
	BSP_GPIOIntMask(SIMU_UART_RX_PIN, 0);  /* unmask gpio interrupt. */
    BSP_GPIOIntEn(SIMU_UART_RX_PIN, 0, 0); /* level low trigger */

	gpio_int_enable(SIMU_UART_RX_PIN);
}

/* Send one byte by simulator uart data */
void SimuSendOneByte(unsigned char Byte)
{
    unsigned int i = 8, tmp;
   
    SIMU_UART_TX_DATA_L(); /* start bit */
    NSTusecDelay(104);

    /* 8 data bit */
    for ( i = 0; i < 8; i++)
    {
        tmp	= (Byte >> i) & 0x01;  /* low front */
        if (tmp == 0)
        {
            SIMU_UART_TX_DATA_L();
            NSTusecDelay(104);	//0
        }
        else
        {
            SIMU_UART_TX_DATA_H();
            NSTusecDelay(104);	//1
        }
    }
    SIMU_UART_TX_DATA_H();
    NSTusecDelay(104);
}

int simu_uart_timer_task(void)
{
	int gpio_value;

	if (recv_stop_flag == 0) {
		SIMU_UART_RX_INPUT();
		gpio_value = BSP_GPIOGetValue(SIMU_UART_RX_PIN);
		if (data_cnt == 0) {	/* start bit */
			if (gpio_value != 0) {	/* false */
				/* set recv data stop flag */
				recv_stop_flag = 1;
				/* enable gpio interrupt */
				gpio_int_enable(SIMU_UART_RX_PIN);
				return -1;		
			}
			byte = 0x0;
			
		} else if ((data_cnt >= 1) && (data_cnt <= 8)) {	/* data */
		    byte >>= 1;
			if (gpio_value == 1) { 	    
		    	byte |= 0x80;     /* recv low bit firstly */			
			}

		} else if (data_cnt >= 9) {
			if (gpio_value == 1) {	/* true */
				/* Save one byte */
				uart_data_recv(byte);			
			}

			data_cnt = 0;
			bytes_cnt++;

			/* 每8个字节重新启动GPIO中断读取数据。 */
			if (bytes_cnt > 8) {
				recv_stop_flag = 1;
				/* enable gpio interrupt */
				gpio_int_enable(SIMU_UART_RX_PIN); 
			}			
			return 0;	
		}
		data_cnt++;
		BSP_Timer1Init(104);
	}
	return 0;
}


void gpio_int_func(int portNum)
{
	int gpio_value;
    
    /* disable gpio interrupt */
	gpio_int_disable(SIMU_UART_RX_PIN);

//	SIMU_UART_RX_INPUT();
	gpio_value = BSP_GPIOGetValue(SIMU_UART_RX_PIN);
	if (gpio_value == 0) {
		/* Start timer1 */
		bytes_cnt = 0;
		data_cnt = 0;
		recv_stop_flag = 0;
		BSP_Timer1Init(40);
		return;
	}
	gpio_int_enable(SIMU_UART_RX_PIN);
}


void simu_uart_init(void)
{
	simu_uart_rx_init();
	simu_uart_tx_init();
}

