/*
 * =====================================================================================
 *
 *       Filename:  nl6621_uart.c
 *
 *    Description:  nl6621 uart driver file
 *
 *        Version:  0.0.1
 *        Created:  2015/7/1 13:30:44
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
#include "common.h"
#include "nl6621_uart.h"

ring_buffer_t uartrxbuf;

NST_TIMER * recv_timer;	/* receive timer */
OS_EVENT * recv_sem;	/* receive sem */
NST_LOCK * recv_lock;	/* receive lock */

/* store data receive from uart, user can change the data store address.
 * */
char recv_data[1024];

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uart_data_send
 *  Description:  Interface of uart data receive.
 *	 parameters: 
 * 				@data:	data buffer will be send
 *				@len:	data len will be send
 *				@uart_type:	type of uart, real uart or simulator
 *						1: real uart; 0: simulator
 *         Note:
 * =====================================================================================
 */
int uart_data_send(unsigned char *data, unsigned short len, unsigned char uart_type)
{
	int i;

	for (i = 0; i < len; i++) {
		NSTusecDelay(5);
		if (uart_type == 1)
			BSP_UartPutcPolled(*(unsigned char *)(data + i));
		else 
			SimuSendOneByte(*(unsigned char *)(data + i));
	}
    if(uart_type == 0)   	//开启定时器1中断，发送数据
	{
	    i = *Tmr1Eoi;
	    i = *TmrsEoi;
		NVIC_EnableIRQ(TMR1_IRQn);
		*Tmr1Load = SIMU_UART_Baudrate;
		*Tmr1Ctl = (~TMR_INT_MASK) |TMR_ENA | TMR_USER_DEFINE_MODE;
	}
	return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uart_data_recv
 *  Description:  Interface of uart data receive. 
 *         Note:
 * =====================================================================================
 */
void uart_data_recv(unsigned char Dummy)
{
	ring_buf_write_char(&uartrxbuf, (char)Dummy);			

	if (ring_buf_cnt(&uartrxbuf) >= (UART_RECV_BUF_SIZE / 2)) {
		OSSemPost(recv_sem);
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  zigbee_recv_timer_handle
 *  Description:  When soft timer coming ervery 50ms, check ring buffer and send to 
 *                remote server. 
 *         Note:
 * =====================================================================================
 */
void uart_recv_timer_handle(void *ptmr, void *parg)
{
	if (ring_buf_cnt(&uartrxbuf) > 0) {
		OSSemPost(recv_sem);
	}	    
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uart_init
 *  Description:  Init uart resource, sended and received   
 *         Note:
 * =====================================================================================
 */
void uart_init(void)
{
	/* Init uart receive ring buffer */
	ring_buf_alloc(&uartrxbuf, UART_RECV_BUF_SIZE);

	memset(recv_data, 0x0, sizeof(recv_data));

	recv_sem = OSSemCreate(0);
	NST_ALLOC_LOCK(&recv_lock);

	/* register receive timer */
    NST_InitTimer(&recv_timer, uart_recv_timer_handle, NULL, NST_TRUE);

	/* init system device */	
	simu_uart_init();

}		/* -----  end of function uart_init  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  UartTaskThread
 *  Description:  Task thread for receving uart data. 
 *         Note:
 * =====================================================================================
 */
void UartTaskThread(void *arg)
{
	UINT8 Err;
	int i;
	unsigned short recv_size;

	log_notice("Create uart recv task success.\n");
	
	/* Init uart resource */
	uart_init();

    NST_SetTimer(recv_timer, UART_RECV_TIMER_NUM);

	while (1) {
	 	OSSemPend(recv_sem, 0, &Err);
		
		if ((recv_size = ring_buf_cnt(&uartrxbuf)) > 0) {
			/* get ring buffer data */
			NST_AQUIRE_LOCK(recv_lock);

			memset(recv_data, 0x0, UART_RECV_BUF_SIZE);
			ring_buf_read(&uartrxbuf, (char*)recv_data, recv_size);
				
			NST_RELEASE_LOCK(recv_lock);

			/* Print the data receive from uart */
#if 1
			log_info("Get uart data(len:0x%x):\n", recv_size);
		    for (i = 0; i < recv_size; i++) {
		        if (((i % 8) == 0) && (i != 0)) 	log_info("\r\n");
		        log_info("0x%x ", recv_data[i]);		
		    }

//			printf("Get uart data(len:0x%x):\n", recv_size);
//		    for (i = 0; i < recv_size; i++) {
//		        if (((i % 8) == 0) && (i != 0)) 	printf("\r\n");
//		        printf("0x%x ", recv_data[i]);		
//		    }
//            recv_data[recv_size] = 0;
//			log_info("%s\r\n",recv_data);
#endif
		}
	}
}		/* -----  end of function UartTaskThread  ----- */
