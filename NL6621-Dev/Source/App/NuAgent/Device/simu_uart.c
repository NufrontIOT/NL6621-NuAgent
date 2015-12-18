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
 *        0.0.2      Hui Lou    2015/12/07     Analog serial port to send data using timer1 and  Analog serial receive data using timer 1.
 * =====================================================================================
 */
#include "common.h"
#include "simu_uart.h"


//SIMU TX
#define SIMU_UART_SEND_BUF_SIZE	   (1024)

struct send_one_byte
{
	unsigned char byte;
	unsigned char bit;
	unsigned char over_flag; 
};

static struct send_one_byte byte_status;
ring_buffer_t SimuUartTxBuf;

int data_cnt = 1;
unsigned char byte = 0x0;
unsigned char recv_stop_flag = 1;
unsigned int bytes_cnt = 0;


//SIMU RX 
/* initialize Uart transmit gpio */
void simu_uart_tx_init(void) 
{		  
    GPIO_InitTypeDef  GPIO_InitStructure;
	 
	/* GPIO5=设置输出高电平，低电平  */			
    GPIO_InitStructure.GPIO_Pin = SIMU_UART_TX_PIN;		//管脚PIN51
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out; 	//设置输出
	GPIO_Init(&GPIO_InitStructure);
	SIMU_UART_TX_HIGH();					//输出高电平

	/* alloc ring buffer */
	ring_buf_alloc(&SimuUartTxBuf, SIMU_UART_SEND_BUF_SIZE);

	byte_status.byte = 0x0;
	byte_status.bit = 0x0;
	byte_status.over_flag = 0x0;
}

void SimuUartOutPut(void)
{
	/* Check and continue send one byte data, or get a new one bytes */
	if (byte_status.over_flag != 0)
	{
		if (byte_status.bit == 0)
			SIMU_UART_TX_LOW();	
		else if (byte_status.bit < 9)
		{
			if((byte_status.byte >> (byte_status.bit-1)) & 0x01)
			   	SIMU_UART_TX_HIGH();
			else
			    SIMU_UART_TX_LOW();
		}
		else
		{
			SIMU_UART_TX_HIGH();
			byte_status.over_flag = 0x0;
		}
		byte_status.bit++;	
	}
	else
	{
		/* Check tx ring buffer empty */
		if (!ring_buf_empty(&SimuUartTxBuf))
		{		
			/* Get one byte data from ring buffer*/
			ring_buf_read(&SimuUartTxBuf, (char *)&byte_status.byte, 1);
			byte_status.bit = 0;
			byte_status.over_flag = 0x1;
		}
		else
		{
			SIMU_UART_TX_HIGH();
 	        *Tmr1Ctl = TMR_INT_MASK;   //关闭中断
		}
	}
}

void SimuSendOneByte(unsigned char ch)
{
	ring_buf_write_char(&SimuUartTxBuf, (char)ch);
}


/* initialize Uart receive gpio */
void simu_uart_rx_init(void) {

    GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;

    /* 初始化SIMU_UART_RX_PIN输入 */
  	GPIO_InitStructure.GPIO_Pin  = SIMU_UART_RX_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_In;
  	GPIO_Init(&GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_Pin_22,IRQ_ENABLE);
    /* 初始化SIMU_UART_RX_PIN下降沿触发 */
   	EXTI_InitStructure.EXTI_Line = SIMU_UART_RX_PIN;
  	EXTI_InitStructure.EXTI_Mode = EXTI_EDGE_SENSITIVE;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_ACTIVE_LOW;
  	EXTI_InitStructure.EXTI_LineCmd = IRQ_ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 

	EXTI_GPIO_Cmd(SIMU_UART_RX_PIN,IRQ_ENABLE);
}

int simu_uart_timer_task(void)
{
	int gpio_value = 0;

	if (recv_stop_flag == 0) {
	    gpio_value = SIMU_UART_GET();		    
		
		if (data_cnt == 0) {	/* start bit */
			if (gpio_value != 0) {	/* false */
				/* set recv data stop flag */
				recv_stop_flag = 1;
				/* enable gpio interrupt */
			    EXTI_GPIO_Cmd(SIMU_UART_RX_PIN,IRQ_ENABLE);
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
				recv_stop_flag = 0;
				bytes_cnt = 0;
		        data_cnt = 0;
				/* enable gpio interrupt */
            	EXTI_GPIO_Cmd(SIMU_UART_RX_PIN,IRQ_ENABLE);
				*Tmr0Ctl = TMR_INT_MASK;
			}			
			return 0;	
		}
		data_cnt++;
	    *Tmr0Load = SIMU_UART_Baudrate;
		*Tmr0Ctl = (~TMR_INT_MASK) |TMR_ENA | TMR_USER_DEFINE_MODE;
	}
	return 0;
}

void Simu_UartIRQ_Func(void)
{
	int gpio_value;

	EXTI_ClearITPendingBit(SIMU_UART_RX_PIN);      
    /* disable gpio interrupt */
	EXTI_GPIO_Cmd(SIMU_UART_RX_PIN,IRQ_DISABLE);

	if(EXTI_GetITStatus(SIMU_UART_RX_PIN) != RESET)	
	{		
	  //add user code
		gpio_value = SIMU_UART_GET();
		if (gpio_value == 0) {  //判断是否串口起始位
			/* Start timer1 */
			bytes_cnt = 0;
			data_cnt = 0;
			recv_stop_flag = 0;
	
			//开启中断
		    gpio_value = *Tmr0Eoi;
		    gpio_value = *TmrsEoi;
			*Tmr0Load = SIMU_UART_Baudrate/2;
			*Tmr0Ctl = (~TMR_INT_MASK) |TMR_ENA | TMR_USER_DEFINE_MODE;
			return;
		}	  	
	}

	EXTI_GPIO_Cmd(SIMU_UART_RX_PIN,IRQ_ENABLE);
	*Tmr0Ctl = TMR_INT_MASK;
}

void simu_uart_init(void)
{
	simu_uart_rx_init();
	simu_uart_tx_init();
}

