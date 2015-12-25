/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  BspDemo.c
 *
 *    Description:  NL6621 BSP Demo(gpio,exti,usart,spi,wdg,spi,timer,i2c,dma,i2s,sdio)
 *
 *        Version:  0.0.1
 *        Created:  2015/12/3 15:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date        Purpose
 *        0.0.1      Hui Lou    2015/12/3    Create and initialize
 *
 * ====================================================================
 */
#include "bspdemo.h"
#include "includes.h"
#include "nuagent_uart.h"
#include "nvic.h"

extern 	void Timerx_Init(TIM_TypeDef TIMx, uint32_t timerus);

void NL6621_BSP_TEST(void)
{
#ifdef I2C_EERPOM_Demo
	unsigned char eepromw_buffer[10] = {48,49,50,51,52,53,54,55,56,57};
	unsigned char eepromr_buffer[10] = {0};
#endif

/***********************************************************************************************************
*   ����GPIO05����ߵ�ƽ�͵͵�ƽ��GPIO03���롣
***********************************************************************************************************/
#ifdef  GPIO_Demo
    printf("NL6621-NuAgent GPIO Test......\r\n");
	GPIO_InOut_Demo();     
#endif
    
/***********************************************************************************************************
*   ����GPIO10�͵�ƽ�жϣ�GPIO21�������ж�
*   BSP_GPIOIntISR_Demo(EXTI_Line10),  BSP_GPIOIntISR_Demo(EXTI_Line21)�ֱ�����жϺ������棬��cortexM3_it.c��
*   EXTI0_IRQHandler ~	 EXTI15_IRQHandler   and   EXTI16_31_IRQnHandler ����ӣ�
***********************************************************************************************************/
#ifdef  GPIO_EXTI_Demo
    printf("NL6621-NuAgent GPIO Interrupt Test......\r\n");
	GPIO_Interrupt_Demo();
	while(1);
#endif 


/**********************************************************************************************************
*   1.���ڳ�ʼ����Ĭ��115200�����ʡ�
*	2.�Ѿ���BSP�ϳ�ʼ���������Բ���Ҫ���³�ʼ����
*   3.ʹ�ô��ڽ����ж�ʱ��BSP_UartISR_Demo�����жϺ������棬��cortexM3_it.c��__irq void UART_IRQHandler(void)����ӣ�
***********************************************************************************************************/
#ifdef  USART_Demo
    #if 0
    USART_Init_Demo(DEFAULT_UART_BAUDRATE); //��ʼ�����ڣ�������115200
    printf("NL6621-NuAgent USART Test......\r\n");
    #endif
#endif

/**********************************************************************************************************
*   1.��ʱ����ʼ�������ʹ��ģ�⴮�ڵĻ�(REAL_UART_USED�궨�屻ȡ���Ļ�����ʱ��0,1����ģ�⴮��ʹ��)��
*     ��ʱ��0,1����ʹ���ˣ���ʱ��1���ڴ��ڷ��ͣ���ʱ��0���ڴ��ڽ��ա�
*	2.����õ�SPI�������ݣ�ʹ��SPI_SDIO_CMD_TEST�Ļ�����ʱ��1�Ѿ�ʹ�á�
*   3.��ʱ���жϷ�Χ�� 1us~107374182us(107s) ����
*   4.��ʱ��0 __irq void TMR0_IRQHandler(void)����
*   5.��ʱ��1 __irq void TMR0_IRQHandler(void)����   
***********************************************************************************************************/
#ifdef TIMER_Demo
    #if 0
    printf("NL6621-NuAgent Timer Test......\r\n");
    Timerx_Init(TIMER1,1000);	 //1ms�ж�
    Timerx_Init(TIMER0,1000000); //1s�ж�
	while(1);
	#endif
#endif 

/**********************************************************************************************************
*   1.���Ź����ʱ��ֻ��ȡ100ms,200ms,400ms
*   2.WATCH_Feed_Demoι����������ǰ20msι��
***********************************************************************************************************/
#ifdef WATCH_Demo
   #if 0
   printf("NL6621-NuAgent WATCH Test......\r\n");
   IWDG_Init_Demo();  //Ĭ����200ms

   while(1)
   {
       WATCH_Feed_Demo();
   	   NSTusecDelay(180000);
   }
   #endif
#endif 

/**********************************************************************************************************
*   I2C EEPROM��ʼ������������ǲ��Է�����д���ԡ�
***********************************************************************************************************/
#ifdef I2C_EERPOM_Demo

	#if 1
    printf("NL6621-NuAgent I2C EEPROM Test......\r\n");
    eeprom_init();

	while(1)
	{
	   eeprom_data_write(0x00,10,eepromw_buffer); 
	   NSTusecDelay(100);
	   eeprom_data_read(0x00,10,eepromr_buffer); 

	   if(memcmp(eepromw_buffer,eepromr_buffer,10) == 0)
	   {
	   	   printf("EEPROM Success!!\r\n");
	   }
	   else
	   {
	       printf("EEPROM Fail!!\r\n");
	   }
	   NSTusecDelay(5000000);
	}
	#endif
#endif 

/**********************************************************************************************************
*   QSPI��ʼ������������ǲ���,BSP��ʼ���Ѿ����Թ�
***********************************************************************************************************/
#ifdef QSPI_FLASH_Demo
    #if 0
    printf("NL6621-NuAgent QSPI FLASH Test......\r\n");
	BSP_QSpiInit(DEFAULT_QSPI_CLK);
    QSpiFlashRead(NV_INFO_START, (UINT8*)pNvInfo, sizeof(NV_INFO));
	#endif
#endif 

/**********************************************************************************************************
*   SPI��ʼ��������������SDIO CMD53��SPI_CS����ģ��GPIO��
***********************************************************************************************************/
#ifdef SPI_SDIO_CMD_TEST
    printf("NL6621-NuAgent SPI --> SDIO Test......\r\n");
    Timerx_Init(TIMER1,0x600000); // used for spi sdio command test
	BSP_SpiSdioCmdTest();
#endif

/**********************************************************************************************************
*   I2S��ʼ��
***********************************************************************************************************/
#ifdef HW_I2S_SUPPORT
    #if 1
    printf("NL6621-NuAgent I2S TX Test......\r\n");
   	I2S_TX_Test();
	#else 
    printf("NL6621-NuAgent I2S RX Test......\r\n");
	I2S_RX_Test();
	#endif
	while(1);
#endif //HW_I2S_SUPPORT

/**********************************************************************************************************
*   SDIO���ԣ���SPI���������໥��ϣ�SPIΪ����SDIOΪ�ӣ�ͨ��CMD53����䣩��
***********************************************************************************************************/
#ifdef SDIO_TEST	              //SDIO��ʼ��
    printf("NL6621-NuAgent SDIO Test......\r\n");
    BSP_SDIOInit();
	while(1);
#endif

/**********************************************************************************************************
*   ���ڴ���ˣ�����DMA���䣬DMA����ֻ����32bit���ӿڣ�NST_DmaMoveMem
***********************************************************************************************************/
#ifdef DMA_MOVE_MEM				  //���ڴ�ʹ��DMA����
    BSP_DmaMoveMemInit(DMA_CHANNEL_1); // init DMA CH 1 for memory move
#endif // DMA_MOVE_MEM //

}

/*******************************************************/
//NL6621 GPIO�����������Demo
/*******************************************************/
#ifdef  GPIO_Demo

	//
	/* NL6621  GPIO Demo */
	//
	void GPIO_InOut_Demo(void)
	{
	     int gpio_level = 0;
		  
		 GPIO_InitTypeDef  GPIO_InitStructure;
		 
		 /* GPIO5=��������ߵ�ƽ���͵�ƽ  */			
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		//�ܽ�PIN51
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out; 	//�������
		 GPIO_Init(&GPIO_InitStructure);
		 GPIO_SetBits(GPIO_Pin_5);						//����ߵ�ƽ
		 GPIO_ResetBits(GPIO_Pin_5);				    //����͵�ƽ	
		
		 /* GPIO3=�������룬�ڲ�û������������Ҫ�ⲿ������ */	
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	   	//�ܽ�PIN48    		
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_In; 	//��������	
		 GPIO_Init(&GPIO_InitStructure);	
		 gpio_level = GPIO_ReadInputDataBit(GPIO_Pin_3);//��ȡ״̬
		 
		 gpio_level = gpio_level; //��ֹ����
		 
		 while(1)
		 {
		     gpio_level = GPIO_ReadInputDataBit(GPIO_Pin_3);//��ȡ״̬
			 printf("GPIO_PIN_3:%d\n",gpio_level);
			 GPIO_SetBits(GPIO_Pin_5);						//����ߵ�ƽ
		   	 NSTusecDelay(1000000);
		     GPIO_ResetBits(GPIO_Pin_5);				    //����͵�ƽ
			 NSTusecDelay(1000000);	
		 }					
	}
	//
	/* NL6621  GPIO Demo */
	//

#endif

/*******************************************************/
//NL6621 GPIO�����ж�Demo
/*******************************************************/
#ifdef  GPIO_EXTI_Demo

	//
	/* NL6621  GPIO �ж� Demo */
	//
	void GPIO_Interrupt_Demo(void)
	{
	    GPIO_InitTypeDef GPIO_InitStructure;
	 	EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

	
	    /* ��ʼ��GPIO5,GPIO7 ���� */
	  	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_7;
	  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_In;
	  	GPIO_Init(&GPIO_InitStructure);
	
	    GPIO_EXTILineConfig(GPIO_Pin_5,IRQ_ENABLE);
	    /* ��ʼ��GPIO5�͵�ƽ���� */
	   	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	  	EXTI_InitStructure.EXTI_Mode = EXTI_EDGE_SENSITIVE;	
	  	EXTI_InitStructure.EXTI_Trigger = EXTI_ACTIVE_LOW;
	  	EXTI_InitStructure.EXTI_LineCmd = IRQ_ENABLE;
	  	EXTI_Init(&EXTI_InitStructure);	 

	    GPIO_EXTILineConfig(GPIO_Pin_7,IRQ_ENABLE);
	    /* ��ʼ��GPIO7�����ش��� */
	  	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	  	EXTI_InitStructure.EXTI_Mode = EXTI_EDGE_SENSITIVE;	
	  	EXTI_InitStructure.EXTI_Trigger = EXTI_ACTIVE_HIGH;
	  	EXTI_InitStructure.EXTI_LineCmd = IRQ_ENABLE;
	  	EXTI_Init(&EXTI_InitStructure);	  	

	    /* Configure GPIO5 interrupt */
	    NVIC_InitStructure.NVIC_IRQChannel = EXTI5_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = GPIO_IRQn_PRIO+3;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
	    NVIC_Init(&NVIC_InitStructure);

		/* Configure GPIO7 interrupt */
	    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = GPIO_IRQn_PRIO+2;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
	}
	
	//GPIO�жϣ�EXTI0_IRQHandler ~	 EXTI15_IRQHandler   and   EXTI16_31_IRQnHandler����
	void BSP_GPIOIntISR_Demo(uint32_t EXTI_Line)
	{
	    //NL6621 gpio irq index
	    //GPIO: 0 1 2 3 4 5 6 7  8  9 10 11 12 13 14 15 16~31
	    //IRQ:  8 7 6 5 4 3 2 1 25 24 23 22 21 20 19 18  29
	    //disable interrupt
		EXTI_GPIO_Cmd(EXTI_Line,IRQ_DISABLE);
	
	
		if(EXTI_GetITStatus(EXTI_Line) != RESET)	
		{		
		  //add user code	
		}
	    
	    EXTI_ClearITPendingBit(EXTI_Line);  
		
	    //enable interrupt
		EXTI_GPIO_Cmd(EXTI_Line,IRQ_ENABLE);
	}
	//
	/* NL6621  GPIO �ж� Demo */
	//

#endif

/*******************************************************/
//NL6621 ����Demo
/*******************************************************/

//
/* NL6621  USART �ж� Demo */
//
//��ʼ������
void USART_Init_Demo(uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;
   
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_FIFOMode = FCR_FIFO_OFF;
	//fifo RX 14byte
	USART_InitStructure.USART_FIFOMode = FCR_RCVR_TRIG_14 | FCR_FIFO_ON;

    USART_Init(&USART_InitStructure);
   
    USART_ITConfig(IER_ERBFI_MASK, IRQ_ENABLE);
   
    USART_Cmd(IRQ_ENABLE);                   
}

//���ڽ����жϣ�__irq void UART_IRQHandler(void)����	
void BSP_UartISR_Demo(void)
{
    uint8_t   LSRValue,IIRValue;
    uint8_t   Dummy = 0,i;
    BOOLEAN   bFlag = TRUE;

    while (bFlag)
    {
        IIRValue = NST_RD_UART_REG(IIR_OFFSET);         //read interrupt register  
        IIRValue = IIRValue&0x0f;                         //get  interrupt id 
        switch (IIRValue)
        {
            case  UART_IIR_RLS :                            // --receiver line status
                LSRValue = NST_RD_UART_REG(LSR_OFFSET);      // read line status register & clear interrupt 

                // check overrun error,parity error, framing error,receiver fifo error bits
                if ( LSR_OE_GET(LSRValue) &&  LSR_PE_GET(LSRValue) &&  LSR_FE_GET(LSRValue) && LSR_FERR_GET(LSRValue))                       // -- ��״̬ �����й��ж� ֻ��������ĳɶԽ���RSR���ַ�
                {                            
                    break;                                 
                }

                if ( LSR_DR_GET(LSRValue) )               // -- indicate receiver data ready
                {          
                    Dummy = NST_RD_UART_REG(RBR_OFFSET);
                }
                break;
                
            case UART_IIR_RDA :                             // -- received data avaliable
		   	    //Dummy = USART_ReceiveData();
		
				//USART_SendData(Dummy);//debug use
				 //fifo RX 14byte
		         for(i=0;i<14;i++){
		             Dummy = USART_ReceiveData();
		             uart_data_recv(Dummy);       /* SerialNet mode process interface */
		         }  

                break;
                
            case UART_IIR_CTI :                           // -- character timeout. we must read receiver buffer register to clear this inerrupt 
		   	     //Dummy = USART_ReceiveData();			
				 //USART_SendData(Dummy);//debug use
			     //fifo RX
				 do{                       
		             Dummy = USART_ReceiveData();
		             uart_data_recv(Dummy);       /* SerialNet mode process interface */      
		         }while(USART_GetFlagStatus(USART_FLAG_RXNE) != RESET);
                break;
				
            case UART_IIR_THRE :                            //--THR empty
                break;
				
            case UART_IIR_MDS :                             //--modem status 
                Dummy = NST_RD_UART_REG(MSR_OFFSET);    //clear interrupt
                break;

            case UART_IIR_BUSY :                             //--modem status 
                Dummy = NST_RD_UART_REG(USR_OFFSET);    //clear interrupt
                break;
                           
            default:
                bFlag = FALSE;                              
                break;
        }
    }		   
}
//
/* NL6621  USART �ж� Demo */
//


/*******************************************************/
//NL6621 ��ʱ��Demo
/*******************************************************/
#ifdef TIMER_Demo
	//
	/* NL6621 TIMER DEMO */
	//	
	void Timerx_Init(TIM_TypeDef TIMx, uint32_t timerus)
	{
	    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
		TIM_TimeBaseStructure.TIM_Period = timerus; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_UserDown; /* TIM_CounterMode_FreeRun��֧���жϹ��� */
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	 
		/* TIM IT enable */
		TIM_ITConfig(TIMx, IRQ_ENABLE);
	
		TIM_Cmd(TIMx, IRQ_ENABLE);  							 
	}	
	
	/* NL6621 TIMER DEMO */
	//
#endif


/*******************************************************/
//NL6621 ���Ź�Demo
/*******************************************************/
#ifdef WATCH_Demo

	#define WatchTimeout_100ms  6
	#define WatchTimeout_200ms  7
	#define WatchTimeout_400ms  8
	//
	/* NL6621 ���Ź� Demo */
	//
	void IWDG_Init_Demo(void) 
	{	
	 	IWDG_WriteAccessCmd();  
		
		IWDG_SetReload(WatchTimeout_200ms);  
		
		IWDG_Enable(); 
	}
	
	//ι����������ǰ20msι��
	void WATCH_Feed_Demo(void)
	{
		IWDG_ReloadCounter();
	}
	//
	/* NL6621  USART �ж� Demo */
	//
#endif 


/*******************************************************/
//NL6621 EEPROM Demo
/*******************************************************/
#ifdef I2C_EERPOM_Demo
	//
	/* NL6621 I2C DEMO */
	//
	/* AT24C08  EEPROM��д���� */
	
	#define EEPROM_ADDR    (0xA0 >> 1)  //�õ�ַ�ǵ�Ҫ����һλ���Ƚ�AT24C08������I2C��ַΪ0xA0,,������Ϊ0x50
	
	/* 
	 * ===  FUNCTION  ======================================================================
	 *         Name:  eeprom_data_read
	 *  Description:  Read eeprom data interface.
	 *   Parameters:  @addr: eeprom data address.
	 *                @length: eeprom data length which will be read.
	 *                @buf: buffer stored eeprom data, it must be large or equel 'length'.
	 *       Return:  0, return the buffer data read.
	 *                1, read eeprom data error.
	 *         Note:
	 * =====================================================================================
	 */
	int eeprom_data_read(unsigned int addr, unsigned int length, unsigned char * buf)
	{
	    int temp = length;
	    
	    temp = BSP_I2CSeqRead(addr, buf, length);
	
	    return temp;
	}
	
	
	/* 
	 * ===  FUNCTION  ======================================================================
	 *         Name:  eeprom_data_write
	 *  Description:  write data to eeprom interface.
	 *   Parameters:  @addr: eeprom data address.
	 *                @length: eeprom data length which will be read.
	 *                @buf: buffer stored eeprom data, it must be large or equel 'length'.
	 *       Return:  1, return data length which had write.
	 *                0, write data to eeprom error.
	 *         Note:
	 * =====================================================================================
	 */
	int eeprom_data_write(unsigned int addr, unsigned int length, unsigned char * buf)
	{
	    int temp = length;
	    
	    temp = BSP_I2CSeqWrite(addr, buf, length);
	
	    return temp;
	}		/* -----  end of function eeprom_data_write  ----- */
	
	//eeprom init
	int eeprom_init(void)
	{
	    I2C_InitTypeDef	I2C_InitStruct;
	
		I2C_InitStruct.I2C_Mode = FAST_MODE;
	
	  	I2C_InitStruct.I2C_ClockLow = 200;		// I2C Ƶ�� 40000/(200+200) Khz = 100Khz
	
		I2C_InitStruct.I2C_ClockHihg = 200;		
	
		I2C_InitStruct.I2C_OwnAddress1 = EEPROM_ADDR;
	
	    I2C_Init(&I2C_InitStruct); 
	
		return 0;
	}
	
	//
	/* NL6621 I2C DEMO */
	//
	/* AT24C08 EEPROM��д���� */
#endif


/*******************************************************/
//NL6621 QSPI FLASH Demo
/*******************************************************/
#ifdef QSPI_FLASH_Demo

	//
	/* NL6621 QSPI DEMO */
	//
	//������QSPIģ��ĳ�ʼ�����룬���ó�����ģʽ������GD25Q41B						  
	//QSPI�ڳ�ʼ��
	//�������Ƕ�SPI1�ĳ�ʼ��
	void BSP_QSpiInit(uint16_t SpiClkDiv)
	{
	    QSPI_InitTypeDef QSPI_InitStructure; 
	
		NST_WR_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL, NST_RD_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL) & (~ENA_QSPIM_GPIO));
	
		QSPI_InitStructure.QSPI_Direction = QSPI_TMOD_TR;                   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
		QSPI_InitStructure.QSPI_Mode = QSPI_FRF_SPI;		                //����SPIģʽ: Motorola SPI
		QSPI_InitStructure.QSPI_DataSize = QSPI_DataSize_8;                 //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
		QSPI_InitStructure.QSPI_CPOL = QSPI_CPOL_High;		                //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
		QSPI_InitStructure.QSPI_CPHA = QSPI_CPHA_2Edge;	                    //���ݲ����ڵڶ���ʱ����
		QSPI_InitStructure.QSPI_BaudRatePrescaler = SpiClkDiv;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	
		QSPI_Init(&QSPI_InitStructure);                                     //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	 
		QSPI_Cmd(IRQ_ENABLE); //ʹ��SPI����				 
	} 

	//continous read flash into pBuf using register addressing mode
	void  QSpiFlashRead(uint32_t ReadStartPos, uint8_t* pBuf, uint32_t Len)  
	{
	    uint32_t ReadAddr = ReadStartPos;
	    uint8_t* pRxData = pBuf;
	    uint32_t DnldByteCnt = 0;
	    uint32_t FlashAdr;
	    OS_CPU_SR  cpu_sr;
	
	    //SpiFlashWaitBusy();
	    
	    while(Len > DnldByteCnt)
	    {
	        uint32_t RdCnt = (Len - DnldByteCnt) > MAX_FLASH_RD_SIZE ? MAX_FLASH_RD_SIZE : (Len - DnldByteCnt);

			FlashAdr = ReadAddr;
	        //SpiFlashWaitBusy();
	        
	        NST_QSPI->SSIENR= 0x00;
			NST_QSPI->CTRLR0=  (QSPI_TMOD_EPROMREAD) | (QSPI_FRF_SPI) | (QSPI_DataSize_8) |( QSPI_CPOL_High | QSPI_CPHA_2Edge);
	        NST_QSPI->CTRLR1= RdCnt - 1; //������д���ݸ���
	        NST_QSPI->SSIENR= 0x01;
	
	        OS_ENTER_CRITICAL();
	        
	        // send cmd
	        BSP_QSpiWriteByte(FLASH_CMD_SEQ_READ);
	        BSP_QSpiWriteByte((FlashAdr>>16) & 0xFF);
	        BSP_QSpiWriteByte((FlashAdr>>8) & 0xFF);
	        BSP_QSpiWriteByte(FlashAdr & 0xFF);
	
	        BSP_QSpiRead(RdCnt, pRxData);
	
	        OS_EXIT_CRITICAL();
	        
	        BSP_QSpiWait();
	        DnldByteCnt += RdCnt;
	        ReadAddr += RdCnt;
	        pRxData += RdCnt;
	
	    }
	}
	
	//continous read flash into pBuf using direct addressing mode
	void  QSpiFlashDirectRead(uint32_t ReadStartPos, uint8_t* pBuf, uint32_t Len)  
	{
	    uint32_t DnldByteCnt = 0;
	    uint8_t*   FlashMemAdr = (UINT8*)(_QSPI_MEM_BASE_ADR + ReadStartPos);
	    
	   // BSP_QSpiDirectReadInit(40);
	    
	    while(Len > DnldByteCnt)
	    {
	        *pBuf++ = *FlashMemAdr++;
	        DnldByteCnt++;
	    }
	}
	
	//read flash status register
	uint8_t QSpiFlashReadSR(void)   
	{  
	    uint8_t byte = 0;   
	
	    // send cmd
	    NST_QSPI->SSIENR= 0x00;
	    NST_QSPI->CTRLR0=  (QSPI_TMOD_TR) | (QSPI_FRF_SPI) | (QSPI_DataSize_8) |( QSPI_CPOL_High | QSPI_CPHA_2Edge);
	    NST_QSPI->SSIENR= 0x01;
	    NST_QSPI->DR = FLASH_CMD_RD_SR;
	
	    // send dummy byte enable read sck
	    NST_QSPI->DR = DUMMY_BYTE;
	
	    BSP_QSpiWait();
	
	    byte = NST_QSPI->DR; 		//����������������������
	    
	    byte = NST_QSPI->DR;  // SR
	    
	    return byte;
	}

	//wait flash operation done
	void QSpiFlashWaitBusy(void)   
	{   
	    uint8_t SR = 0;
	    
	    while (1)
	    {
	        SR = QSpiFlashReadSR();

	        if (!(SR & FLASH_SR_BUSY))
	            break;
	    }
	}  
	

	void QSPI_FlashWriteEnable(void) 
	{
	    NST_QSPI->SSIENR= 0x00;
	    NST_QSPI->CTRLR0=  (QSPI_TMOD_TO) | (QSPI_FRF_SPI) | (QSPI_DataSize_8) |( QSPI_CPOL_High | QSPI_CPHA_2Edge);
	    NST_QSPI->SSIENR= 0x01;
	    
	    NST_QSPI->DR = FLASH_CMD_WREN;
	
	    BSP_QSpiWait(); 
	}
	
	//erase whole chip
	void QSpiFlashEraseChip(void)   
	{  
	    QSPI_FlashWriteEnable();
	
	    NST_QSPI->SSIENR= 0x00;
	    NST_QSPI->CTRLR0=  (QSPI_TMOD_TO) | (QSPI_FRF_SPI) | (QSPI_DataSize_8) |( QSPI_CPOL_High | QSPI_CPHA_2Edge);
	    NST_QSPI->SSIENR= 0x01;
	    
	    NST_QSPI->DR = FLASH_CMD_CHIP_ERASE;

	
		//printf("BSP_QSpiWait\n");
	    BSP_QSpiWait();  // �ȴ�SPI����������ȷ��ǰһ��cmd�Ѿ�����(CS������)
	
	    QSpiFlashWaitBusy();    				   //�ȴ�оƬ��������
	}   
	

	//QSPI program one page using register addressing mode
	void QSpiFlashWriteOnePage(uint32_t Adr, uint8_t* pBuf, uint16_t DataLen) 
	{
	    uint32_t i;
	    uint32_t FlashAdr;
	
	    FlashAdr = Adr;

	    QSPI_FlashWriteEnable();
	
	    NST_QSPI->SSIENR= 0x00;
	    NST_QSPI->CTRLR0=  (QSPI_TMOD_TO) | (QSPI_FRF_SPI) | (QSPI_DataSize_8) |( QSPI_CPOL_High | QSPI_CPHA_2Edge);
	    NST_QSPI->SSIENR= 0x01;
	
	    // send cmd
	    BSP_QSpiWriteByte(FLASH_CMD_PAGE_WRITE);
	    BSP_QSpiWriteByte((FlashAdr>>16) & 0xFF);
	    BSP_QSpiWriteByte((FlashAdr>>8) & 0xFF);
	    BSP_QSpiWriteByte(FlashAdr & 0xFF);
	
	    // send data
	    for (i = 0; i <DataLen; i++)
	    {
	        while(!(NST_QSPI->SR & QSPI_SR_TF_NOT_FULL));
	        
	        NST_QSPI->DR = pBuf[i];
	    }
	
	    BSP_QSpiWait(); 
	
	    QSpiFlashWaitBusy(); 
	}
	
	void QSpiFlashEraseSector(uint32_t SectorAddr) 
	{
	    uint32_t FlashAdr = SectorAddr;
	
	    QSPI_FlashWriteEnable();
	
	    NST_QSPI->SSIENR= 0x00;
	    NST_QSPI->CTRLR0=  (QSPI_TMOD_TO) | (QSPI_FRF_SPI) | (QSPI_DataSize_8) |( QSPI_CPOL_High | QSPI_CPHA_2Edge);
	    NST_QSPI->SSIENR= 0x01;
	
	    // send cmd
	    BSP_QSpiWriteByte(FLASH_CMD_SECTOR_ERASE);
	    BSP_QSpiWriteByte((FlashAdr>>16) & 0xFF);
	    BSP_QSpiWriteByte((FlashAdr>>8) & 0xFF);
	    BSP_QSpiWriteByte(FlashAdr & 0xFF);
	
	    BSP_QSpiWait(); 
	
	    QSpiFlashWaitBusy(); 
	}
	
	uint8_t PageBuf[FLASH_PAGE_SIZE] = {0};
	
	int32_t QSpiWriteOneSector(uint32_t SectorAddr, uint8_t* pBuf)
	{
	    uint32_t i, j;
	    uint32_t WriteAddr = SectorAddr;
	    uint32_t PageCnt;
	    
	    PageCnt = FLASH_SECTOR_SIZE/FLASH_PAGE_SIZE;
	    
	    QSpiFlashEraseSector(WriteAddr);
	
	    NSTusecDelay(100000);
	
	    for (i = 0; i < PageCnt; i++)
	    {
	        j = 10;
	
	        while (j--)
	        {
	            QSpiFlashWriteOnePage(WriteAddr, pBuf,FLASH_PAGE_SIZE);
	            NSTusecDelay(1000);
	            QSpiFlashRead(WriteAddr, PageBuf, FLASH_PAGE_SIZE);
	            if (memcmp(PageBuf, pBuf, FLASH_PAGE_SIZE) == 0)
	                break;
	        }
	
	        if (j == 0)
	            return -1;
	        
	        pBuf += FLASH_PAGE_SIZE;
	        WriteAddr += FLASH_PAGE_SIZE;
	    }
	
	    return 0;
	}
		
	//ȷ��д�ĵ�ַ����֮���Ƿ�����ˡ�
	int32_t QSpiWriteAny(uint32_t WriteAddr, uint8_t* pData, uint16_t DataLen)   
	{ 			 		 
		UINT16 pageremain,j;	 
		  
	
		pageremain = FLASH_PAGE_SIZE-WriteAddr%FLASH_PAGE_SIZE; //��ҳʣ����ֽ���
				 	    
		if(DataLen <= pageremain)
		   pageremain = DataLen;  //������256���ֽ�
	
		do{	
		    j = 10;
	        while (j--)
	        {    
				QSpiFlashWriteOnePage(WriteAddr, pData, pageremain);
		        NSTusecDelay(1000);
		        QSpiFlashRead(WriteAddr, PageBuf, pageremain);
		        if (memcmp(PageBuf, pData, pageremain) == 0)
		             break;
	        }
	        if (j == 0)
	            return -1;
	
			if(DataLen == pageremain)
			    break;//д�������
		 	else {
				pData     += pageremain;
				WriteAddr += pageremain;	
	
				DataLen   -= pageremain;//��ȥ�Ѿ�д���˵��ֽ���
				if(DataLen > FLASH_PAGE_SIZE)
				   pageremain = FLASH_PAGE_SIZE; //һ�ο���д��256���ֽ�
				else 
				   pageremain = DataLen; 	  //����256���ֽ���
			}
		}while(1);	  
		return 0;  
     } 
#endif



/*******************************************************/
//NL6621 SPI FLASH Demo
/*******************************************************/
#ifdef SPI_FLASH_Demo

	//
	/* NL6621 SPI DEMO */
	//

	//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������GD25Q41B						  
	//SPI�ڳ�ʼ��
	//�������Ƕ�SPI1�ĳ�ʼ��
	
	void BSP_SpiInit(void)
	{
	    SPI_InitTypeDef  SPI_InitStructure;
	
	    NST_WR_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL, NST_RD_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL) & (~ENA_SPIM_GPIO));
	    NST_WR_PWM_REG(ADDR_SPI_PIN_MUX_CTRL, NST_RD_PWM_REG(ADDR_SPI_PIN_MUX_CTRL) & (~SPI_MUX_QSPI_PINS));
	
		SPI_InitStructure.SPI_Direction = SPI_TMOD_TR;      //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
		SPI_InitStructure.SPI_Mode = SPI_FRF_SPI;		    //����SPIģʽ: Motorola SPI
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8;      //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		    //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	    //���ݲ����ڵڶ���ʱ����
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
		SPI_InitStructure.SPI_DevNum = 1;	                    //���豸ʹ��,1�����豸��
	
		SPI_Init(&SPI_InitStructure);                                           //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	 
		SPI_Cmd(IRQ_ENABLE); //ʹ��SPI����			 
	} 

	//���ڱ�־һ����д�������
	void BSP_SpiWait(void)
	{
	    uint8_t retry=0;
	    
	    while(!(NST_SPI->SR & SR_TF_EMPT))//�ȴ���������    
	    {
	        retry++;    
	    }
	
	    retry=0;
	    while(NST_SPI->SR & SR_BUSY) //�ȴ�������һ��byte  
	    {
	        retry++;
	    }
	
	}
	
	//spi write one byte
	void  BSP_SpiWriteByte(uint8_t Byte)   
	{
	    
	    while(!(NST_SPI->SR & SR_TF_NOT_FULL));
	    
	    NST_SPI->DR = Byte;    
	
	}
	
	//spi continous read
	void BSP_SpiRead(uint32_t RdCnt, uint8_t* pBuf)   
	{
	    uint8_t RxByte;
	    
	    while(RdCnt > 0) {
	       
		    if(NST_SPI->SR & SR_RF_NOT_EMPT) {
	            *pBuf++ =  NST_SPI->DR;
	            RdCnt--;
	        }
	    }
	
	    // throw the redundant bytes
	    if(NST_SPI->SR & SR_RF_NOT_EMPT) {
	        RxByte =  NST_SPI->DR;
	        RxByte =RxByte; // remove warning
	    }
	}
	
	/*  nl6621 spi�ж����� */
	//	NVIC_InitTypeDef NVIC_InitStructure;
		/* Enable the SPI global Interrupt */
	//	NVIC_InitStructure.NVIC_IRQChannel = SPI_IRQn; 
	//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	//	NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE; 
	//	NVIC_Init(&NVIC_InitStructure);
	//	  
	//    SPI_ITConfig(IMR_TXEIM|IMR_RXOIM,IRQ_ENABLE);
	//void SPI_IRQHandler(void)
	//{
	//    NVIC_DisableIRQ(SPI_IRQn);
	//  
	//	if (SPI_GetITFlagStatus(RISR_RXOIR|RISR_TXEIR) != RESET)
	//    {
	//		SPI_Clear_Interrupt(ICR_FLAG); 
	//		//ADD UERS CODE...
	//		//...  
	//	}
	//
	//    NVIC_EnableIRQ(SPI_IRQn);  
	//} 
		
	uint8_t SpiFlashReadSR(void)   
	{  
	    uint8_t byte = 0;   
	
	    // send cmd
	    NST_SPI->SSIENR= 0x00;
	    NST_SPI->CTRLR0=  (SPI_TMOD_TR) | (SPI_FRF_SPI) | (SPI_DataSize_8) |( SPI_CPOL_High | SPI_CPHA_2Edge);
	    NST_SPI->SSIENR= 0x01;
	    NST_SPI->DR = FLASH_CMD_RD_SR;
	
	    // send dummy byte enable read sck
	    NST_SPI->DR = DUMMY_BYTE;
	
	    BSP_SpiWait();
	
	    byte = NST_SPI->DR;  //����������������������
	    
	    byte = NST_SPI->DR;  // SR
	    
	    return byte;
	}

	//wait flash operation done
	void SpiFlashWaitBusy(void)   
	{   
	    uint8_t SR = 0;
	    
	    while (1)
	    {
	        SR = SpiFlashReadSR();
	        if (!(SR & FLASH_SR_BUSY))
	            break;
	    }
	}  
	
	//continous read flash into pBuf
	void  SpiFlashRead(uint32_t ReadStartPos, uint8_t* pBuf, uint32_t Len)  
	{
	    uint32_t ReadAddr = ReadStartPos;
	    uint8_t* pRxData = pBuf;
	    uint32_t DnldByteCnt = 0;
	    uint32_t FlashAdr;
	    
	    //SpiFlashWaitBusy();
	    
	    while(Len > DnldByteCnt)
	    {
	        uint32_t RdCnt = (Len - DnldByteCnt) > MAX_FLASH_RD_SIZE ? MAX_FLASH_RD_SIZE : (Len - DnldByteCnt);
	
	        FlashAdr = ReadAddr;
	        //SpiFlashWaitBusy();
	        
	        NST_SPI->SSIENR= 0x00;
	        NST_SPI->CTRLR0=  (SPI_TMOD_EPROMREAD) | (SPI_FRF_SPI) | (SPI_DataSize_8) |( SPI_CPOL_High | SPI_CPHA_2Edge);
	        NST_SPI->CTRLR1= RdCnt - 1; //������д���ݸ���
	        NST_SPI->SSIENR= 0x01;
	        
	        // send cmd
	        BSP_SpiWriteByte(FLASH_CMD_SEQ_READ);
	        BSP_SpiWriteByte((FlashAdr>>16) & 0xFF);
	        BSP_SpiWriteByte((FlashAdr>>8) & 0xFF);
	        BSP_SpiWriteByte(FlashAdr & 0xFF);
	
	        BSP_SpiRead(RdCnt, pRxData);
	        BSP_SpiWait();
	        DnldByteCnt += RdCnt;
	        ReadAddr += RdCnt;
	        pRxData += RdCnt;
	
	    }
	
	}

	//enable flash write
	void SPI_FlashWriteEnable(void) 
	{
	    NST_SPI->SSIENR= 0x00;
	    NST_SPI->CTRLR0=  (SPI_TMOD_TO) | (SPI_FRF_SPI) | (SPI_DataSize_8) |( SPI_CPOL_High | SPI_CPHA_2Edge);
	    NST_SPI->SSIENR= 0x01;
	    
	    NST_SPI->DR = FLASH_CMD_WREN;
	
	    BSP_SpiWait(); 
	}
	
	// erase whole chip
	void SpiFlashEraseChip(void)   
	{  

	    SPI_FlashWriteEnable();
	
	    NST_SPI->SSIENR= 0x00;
	    NST_SPI->CTRLR0=  (SPI_TMOD_TO) | (SPI_FRF_SPI) | (SPI_DataSize_8) |( SPI_CPOL_High | SPI_CPHA_2Edge);
	    NST_SPI->SSIENR= 0x01;
	    
	    NST_SPI->DR = FLASH_CMD_CHIP_ERASE;
	
	    BSP_SpiWait();  // �ȴ�SPI����������ȷ��ǰһ��cmd�Ѿ�����(CS������)
	
	    SpiFlashWaitBusy();                       //�ȴ�оƬ��������
	}   
	
	//program one page
	void SpiFlashWriteOnePage(uint32_t Adr, uint8_t* pBuf,  uint16_t DataLen) 
	{
	    uint32_t  i;
	    uint32_t  FlashAdr;
	    OS_CPU_SR cpu_sr;
	
	    FlashAdr = Adr;
	

	    SPI_FlashWriteEnable();
	
	
	    NST_SPI->SSIENR= 0x00;
	    NST_SPI->CTRLR0=  (SPI_TMOD_TO) | (SPI_FRF_SPI) | (SPI_DataSize_8) |( SPI_CPOL_High | SPI_CPHA_2Edge);
	    NST_SPI->SSIENR= 0x01;
	
	    OS_ENTER_CRITICAL();
	
	    // send cmd
	    BSP_SpiWriteByte(FLASH_CMD_PAGE_WRITE);
	    BSP_SpiWriteByte((FlashAdr>>16) & 0xFF);
	    BSP_SpiWriteByte((FlashAdr>>8) & 0xFF);
	    BSP_SpiWriteByte(FlashAdr & 0xFF);
	
	    // send data
	    for (i = 0; i <DataLen; i++)
	    {
	        while(!(NST_SPI->SR & SR_TF_NOT_FULL));
	        
	        NST_SPI->DR = pBuf[i];
	    }
	
	    OS_EXIT_CRITICAL();
	
	    BSP_SpiWait(); 
	
	    SpiFlashWaitBusy(); 
	        
	}
	
	void SpiFlashEraseSector(uint32_t SectorAddr) 
	{
	    uint32_t FlashAdr = SectorAddr;
	
	    SPI_FlashWriteEnable();
	
	    NST_SPI->SSIENR= 0x00;
	    NST_SPI->CTRLR0=  (SPI_TMOD_TO) | (SPI_FRF_SPI) | (SPI_DataSize_8) |( SPI_CPOL_High | SPI_CPHA_2Edge);
	    NST_SPI->SSIENR= 0x01;
	
	    // send cmd
	    BSP_SpiWriteByte(FLASH_CMD_SECTOR_ERASE);
	    BSP_SpiWriteByte((FlashAdr>>16) & 0xFF);
	    BSP_SpiWriteByte((FlashAdr>>8) & 0xFF);
	    BSP_SpiWriteByte(FlashAdr & 0xFF);
	
	    BSP_SpiWait(); 
	
	    SpiFlashWaitBusy(); 
	}

 	uint8_t SpiPageBuf[FLASH_PAGE_SIZE] = {0};
	//ȷ��д�ĵ�ַ����֮���Ƿ�����ˡ�
	INT32 SpiWriteAny(UINT32 WriteAddr, UINT8* pData, UINT16 DataLen)   
	{ 			 		 
		UINT16 pageremain,j;	 
		  
	
		pageremain = FLASH_PAGE_SIZE-WriteAddr%FLASH_PAGE_SIZE; //��ҳʣ����ֽ���
				 	    
		if(DataLen <= pageremain)
		   pageremain = DataLen;  //������256���ֽ�
	
		do{	
		    j = 10;
	        while (j--)
	        {    
				SpiFlashWriteOnePage(WriteAddr, pData, pageremain);
		        NSTusecDelay(1000);
		        SpiFlashRead(WriteAddr, SpiPageBuf, pageremain);
		        if (memcmp(SpiPageBuf, pData, pageremain) == 0)
		             break;
	        }
	        if (j == 0)
	            return -1;
	
			if(DataLen == pageremain)
			    break;//д�������
		 	else {
				pData     += pageremain;
				WriteAddr += pageremain;	
	
				DataLen   -= pageremain;//��ȥ�Ѿ�д���˵��ֽ���
				if(DataLen > FLASH_PAGE_SIZE)
				   pageremain = FLASH_PAGE_SIZE; //һ�ο���д��256���ֽ�
				else 
				   pageremain = DataLen; 	  //����256���ֽ���
			}
		}while(1);	  
		return 0;  
	}

	#ifdef SPI_SDIO_CMD_TEST
	/*SPI Master Tx Cmd for EUMU SPI slave*/
	uint8_t EUM_CMD[13][6] ={
	/*CMD0    */     0x40,	0x00,	0x00,	0x00,	0x00,	0x95,
	/*CMD5_1  */   0x45,	0x00,	0x00,	0x00,	0x00,	0x5B,
	/*CMD5_2  */   0x45,	0x00,	0x30,	0x00,	0x00,	0x87,
	/*CMD52_1 */  0x74,	0x88,	0x00,	0x26,	0x03,	0x71,
	/*CMD52_2 */  0x74,	0x88,	0x00,	0x26,	0x03,	0x71,
	/*CMD52_3 */  0x74,	0x88,	0x00,	0x08,	0x07,	0x19,
	/*CMD52_4 */  0x74,	0x88,	0x00,	0x04,	0x07,	0xf1,
	/*CMD52_5 */  0x74,	0x88,	0x00,	0x06,	0x07,	0xdd,
	/*CMD52_6 */  0x74,	0x88,	0x00,	0x10,	0xff,	0x57,
	/*CMD52_7 */  0x74,	0x88,	0x00,	0x08,	0x00,	0x67,
	/*CMD52_8 */  0x74,	0x88,	0x00,	0x0e,	0x20,	0x77,
	/*CMD52_9 */  0x74,	0x88,	0x00,	0x08,	0x07,	0x19,
	/*CMD52_10*/ 0x74,	0x88,	0x00,	0x2c,	0x03,	0xed	
		};
	
	uint8_t SPIEUM[13] = {
	    8,
		12,
		12,
		9,
		9,
		9,
		9,
		9,
		9,
		9,
		9,
		9,
		9
		};
	
	uint8_t CMD53R[5] = {
		0x75,
		0x10,
		0x00,
		0x00,
		0x00
		};	
	
	uint8_t CMD53W[5] = {
		0x75,
		0x90,
		0x00,
		0x00,
		0x00
		};
	
	uint8_t TestSdioWriteBuf[4096] = {0};
	uint8_t TestSdioReadBuf[4096] = {0};
	
	void BSP_SpiEumCmdWrite(uint8_t index)
	{
	    UINT32 i;
	    
	    NST_WR_GPIO_REG(SWPORTA_DR, 0x00); 
	    
	    for (i=0; i<SPIEUM[index]; i++)
	    {	    
	        if (i<6) 
	            BSP_SpiWriteByte(EUM_CMD[index][i]); // write cmd bytes
	        else    
	            BSP_SpiWriteByte(0xFF); // send clk for response token
	    }
	    
	    BSP_SpiWait();
	    
	    NST_WR_GPIO_REG(SWPORTA_DR, 0xFF);
	}
	
	void  BSP_SpiEumSdioSlave(void)   
	{
	    uint8_t i;
	    
	    // use spi tx only mode
	    NST_SPI->SSIENR = 0x00;
	    NST_SPI->CTRLR0=  (SPI_TMOD_TO) | (SPI_FRF_SPI) | (SPI_DataSize_8) |( SPI_CPOL_High | SPI_CPHA_2Edge);
	    NST_SPI->SSIENR = 0x01;
	    
	    // sync clk domain
	    NST_WR_GPIO_REG(SWPORTA_DR, 0xFF);
	    BSP_SpiWriteByte(0xFF);
	    BSP_SpiWait();
	
	    // send command for sdio slave enumeration
	    for(i=0; i< 13;i++)
	    { 
	        BSP_SpiEumCmdWrite(i);
	    }
	}
	
	void BSP_SpiGenCmd53Crc(uint16_t bytecnt, uint8_t FlagRW)
	{
	    UINT32 i;
	    UINT32 j;
	    UINT8  crc7=0;
	    UINT8  cmd;
	    UINT8 * pCmd53 = FlagRW ? CMD53W : CMD53R;
	    
	    NST_WR_GPIO_REG(SWPORTA_DR, 0x00); 
	            
	    for (i=0;i<5;i++)
	    {
	        if (i==3) 
	            cmd=((bytecnt>>8)&0x1) | (pCmd53[i]&0xFE);
	        else if (i==4) 
	            cmd = bytecnt & 0xFF;
	        else 
	            cmd=pCmd53[i];
	        
	        BSP_SpiWriteByte(cmd);
	        
	        for(j=0;j<8;j++)
	        { 	//crc <= {crc[5:3], crc[2]^crc_in^crc[6], crc[1:0], crc_in^crc[6]};	
	            crc7=( ((crc7<<1)&0x76)  | (((crc7<<1) ^ (crc7>>3) ^ (cmd>>4))&0x8) | (((cmd>>7) ^ (crc7>>6))&0x1));
	            cmd = cmd<<1;					
	        }	
	    }
	            
	    crc7=(crc7<<1) | 0x1; 
	            
	    BSP_SpiWriteByte(crc7);	
	}
	
	void BSP_SpiReadSingleBlock(uint32_t RdCnt, uint8_t* pBuf)   
	{      
	    // start clk
	    while(!(NST_SPI->SR & SR_TF_NOT_FULL));    
	    NST_SPI->DR = 0xFF;  
	
	    // wait for start token(1 byte)    
	    while (1)
	    {
	        if (NST_SPI->SR & SR_RF_NOT_EMPT)
	        {
	             if (NST_SPI->DR == 0xFE)
	                 break;
	        }
	    }
	    
	    // read data
	    while(RdCnt)
	    {
	        if(NST_SPI->SR & SR_RF_NOT_EMPT)
	        {
	            *pBuf++ =  NST_SPI->DR;
	            RdCnt--;
	        }
	    }
	
	    // ignore data crc16
	
	    ///TODO: wait for slave to complete TRANS_DONE_INT, no need to send sclk
	}
	
	void BSP_SpiCmd53Read(uint16_t bytecnt,uint8_t* pBuf)
	{
	    uint32_t i;
	    uint16_t bytecnt_i;
	    OS_CPU_SR  cpu_sr;
	 
	    while (bytecnt > 0)
	    {
	        if(bytecnt>511) 
	            bytecnt_i=512;
	        else 
	            bytecnt_i = bytecnt;
	
	        // use spi tx only mode to send cmd53
	        NST_SPI->SSIENR = 0x00;
	        NST_SPI->CTRLR0=  (SPI_TMOD_TO) | (SPI_FRF_SPI) | (SPI_DataSize_8) |( SPI_CPOL_High | SPI_CPHA_2Edge);
	        NST_SPI->SSIENR = 0x01;
	        
	        BSP_SpiGenCmd53Crc(bytecnt_i, 0);  //sens cmd53 and crc (6 bytes total)
	        
	        // wait cmd53 response  (1 byte for start token, 2 bytes for response)
	        for (i = 0; i < 3; i++)
	            BSP_SpiWriteByte(0xFF);
	
	        BSP_SpiWait();
	
	        ///TODO: wait for slave to complete RD_START_INT, need to send sclk
	
	        NST_WR_GPIO_REG(SWPORTA_DR, 0xFF);
	        // switch to spi read only mode to read data response
	        NST_SPI->SSIENR = 0x00;
	        NST_SPI->CTRLR0=  (SPI_TMOD_RO) | (SPI_FRF_SPI) | (SPI_DataSize_8) |( SPI_CPOL_High | SPI_CPHA_2Edge);
	        NST_SPI->CTRLR1= bytecnt_i+10; //  delay byte(1~7) + 1start byte + bytecnt + 2byte crc + 1end byte - 1
	        NST_SPI->SSIENR = 0x01;
	
	        OS_ENTER_CRITICAL();
	        
	        NST_WR_GPIO_REG(SWPORTA_DR, 0x00);
	        
	        BSP_SpiReadSingleBlock(bytecnt_i, pBuf) ; 
	
	        BSP_SpiWait();
	       
	        NST_WR_GPIO_REG(SWPORTA_DR, 0xFF);
	        
	        OS_EXIT_CRITICAL();
		
	        bytecnt -= bytecnt_i;
	        pBuf += bytecnt_i;
	    }
	}
	
	void BSP_SpiCmd53Write(uint16_t bytecnt,uint8_t* pBuf)
	{
	    uint32_t i;// j;
	    uint16_t bytecnt_i;
	    uint16_t crc16;
	   // UINT16 Data;
	
	    // use spi tx only mode to send cmd53 and write data
	    NST_SPI->SSIENR = 0x00;
	    NST_SPI->CTRLR0=  (SPI_TMOD_TO) | (SPI_FRF_SPI) | (SPI_DataSize_8) |( SPI_CPOL_High | SPI_CPHA_2Edge);
	    NST_SPI->SSIENR = 0x01;
	
	    while (bytecnt > 0)
	    {
	        if(bytecnt>511) 
	            bytecnt_i=512;
	        else 
	            bytecnt_i = bytecnt;
	    
	        BSP_SpiGenCmd53Crc(bytecnt_i, 1);  //send cmd53 and crc (6 bytes total)
	        
	        // wait cmd53 response  (1 byte for start token, 2 bytes for response)
	        for (i = 0; i < 3; i++)
	            BSP_SpiWriteByte(0xFF);
	
	        ///TODO: wait for slave firmware to complete WR_START_INT, need to send sclk
	    
	        // send start token
	        BSP_SpiWriteByte(0xFE);
	    
	        // send tx data
	        for (i = 0; i < bytecnt_i; i++)
	        {
	            //Data = (UINT16)pBuf[i];
	            BSP_SpiWriteByte(pBuf[i]);
	    
	            #if 0 // slave not care data crc, so set to zero
	    		for(j=8;j>0;j--)
	    		{ 					
	    			// crc = {crc[14:12],crc[15]^crc[11]^bit[j-1],crc[10:5],crc[15]^crc[4]^bit[j-1],crc[3:0],crc[15]^bit[j-1]
	                crc16 = ((crc16<<1)&0xE000) | 
	                        (((crc16>>3)^(crc16<<1)^ (Data<<(13-j)))&0x1000)| 
	                        ((crc16<<1)&0x0FC0) | 
	                        (((crc16>>10)^(crc16<<1)^ ((j>6)?(Data>>(j-6)):(Data<<(6-j))))&0x0020)| 
	                        ((crc16<<1)&0x001E) | 
	                        (((crc16>>15) ^ (Data>>(j-1))) & 0x0001);
	    		}
	    		#endif
	        }
	
	        // send data crc
	        crc16 = 0;
	        BSP_SpiWriteByte((UINT8)((crc16 & 0xFF00)>>8));
	        BSP_SpiWriteByte((UINT8)(crc16 & 0xFF));
	
	        ///TODO: wait for slave firmware to complete TRANS_DONE_INT, no need to send sclk
	    
	        // wait data response token and busy token
	        for (i = 0; i < 8; i++)
	            BSP_SpiWriteByte(0xFF);    
	
	        BSP_SpiWait();
	    
	        NST_WR_GPIO_REG(SWPORTA_DR, 0xFF);
	        bytecnt -= bytecnt_i;
	        pBuf += bytecnt_i;
	    }
	}
	
	//SPIʹ��53�����/����SDIO����
	void BSP_SpiSdioCmdTest(void)
	{
	    uint32_t i;
	    uint32_t WriteStartTime = 0;
	    uint32_t WriteDoneTime = 0;
	    uint32_t ReadDoneTime = 0;
	    
	    for (i = 0; i < 4096; i++)
	    {
	        TestSdioWriteBuf[i] = 255 - (UINT8)i;
	        TestSdioReadBuf[i] = 0xFF;
	    }
	
	    BSP_SpiInit();
	
	    BSP_SpiEumSdioSlave();
	
	    WriteStartTime = *Tmr1Val;
	    BSP_SpiCmd53Write(4096, TestSdioWriteBuf);
	    WriteDoneTime = *Tmr1Val;
	
	    BSP_SpiCmd53Read(4096, TestSdioReadBuf);
	    ReadDoneTime = *Tmr1Val;
	
	    if (memcmp(TestSdioWriteBuf, TestSdioReadBuf, 4096) == 0)
	    {
	        DBGPRINT(DEBUG_TRACE, "test sdio succeed, WriteStartTime = %d, WriteDoneTime = %d, ReadDoneTime = %d\n",
	        						WriteStartTime, WriteDoneTime, ReadDoneTime);
	    }
	    else
	    {
	        DBGPRINT(DEBUG_TRACE, "test sdio failed\n");
	        hex_dump("TestSdioReadBuf", TestSdioReadBuf, 4096);
	    }
	}
    #endif //SPI_SDIO_CMD_TEST

#endif  //#ifdef SPI_FLASH_Demo



/*
******************************************************************************
**                            End Of File                                    **
******************************************************************************
*/

