/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  i2c.c
 *
 *    Description:  This file provides all the I2C firmware functions.
 *
 *        Version:  0.0.1
 *        Created:  2015/12/1 15:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date        Purpose
 *        0.0.1      Hui Lou    2015/12/1    Create and initialize
 *
 * ====================================================================
 */
#include "i2c.h"	
#include "includes.h"



void I2C_Init(I2C_InitTypeDef* I2C_InitStruct)
{
     NST_WR_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL, NST_RD_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL) & (~ENA_I2C_GPIO));


     NST_WR_I2C_REG(0x0, IC_ENABLE);

     NST_WR_I2C_REG(I2C_InitStruct->I2C_Mode | SLV_DIS | RESTART | MASTER_EN, IC_CON); // TODO standard mode
    //NST_WR_I2C_REG(M_STOP_DET, IC_INTR_MASK);

    // I2C 初始化后配一次IC_TAR即可
	// I2C 频率  =  40000/(IC_SS_SCL_LCNT+IC_SS_SCL_HCNT) Khz
	 if(I2C_InitStruct->I2C_Mode == STANDARD_MODE) {   //1: standard mode    (0 kbit/s to 100 kbit/s)  
	     NST_WR_I2C_REG(I2C_InitStruct->I2C_ClockLow,  IC_SS_SCL_LCNT);
	     NST_WR_I2C_REG(I2C_InitStruct->I2C_ClockHihg, IC_SS_SCL_HCNT);	 
	 } else if(I2C_InitStruct->I2C_Mode == FAST_MODE) { //2: fast mode       (0 kbit/s ≤ 400 kbit/s)
         NST_WR_I2C_REG(I2C_InitStruct->I2C_ClockLow,  IC_FS_SCL_LCNT);
         NST_WR_I2C_REG(I2C_InitStruct->I2C_ClockHihg, IC_FS_SCL_HCNT);	 
	 } else if(I2C_InitStruct->I2C_Mode == HIGH_MODE) { //3: high speed mode  ( ≤ 3.4 Mbit/s)
         NST_WR_I2C_REG(I2C_InitStruct->I2C_ClockLow,  IC_HS_SCL_LCNT);
         NST_WR_I2C_REG(I2C_InitStruct->I2C_ClockHihg, IC_HS_SCL_HCNT);	 	 
	 }

     NST_WR_I2C_REG(I2C_InitStruct->I2C_OwnAddress1, IC_TAR);  //设备地址
     NST_WR_I2C_REG(0x82, IC_SDA_SETUP);
     NST_WR_I2C_REG(0x01, IC_ENABLE);
     NST_RD_I2C_REG(IC_CLR_INTR);
}

void I2C_Cmd(FunctionalState NewState)
{
 	if (NewState != IRQ_DISABLE){
	    NST_WR_I2C_REG(1, IC_ENABLE);
	} else {
	    NST_WR_I2C_REG(0, IC_ENABLE);
	}   
}

void I2C_DMACmd(uint16_t DMA_TYPE,uint16_t Length)
{
	 NST_SPI->DMACR = DMA_TYPE;
	
	 if(DMA_TYPE == (IC_TDMAE | IC_RDMAE)) {
         NST_WR_I2C_REG((IC_TDMAE|IC_RDMAE), IC_ENABLE);
	 }
	 else if(DMA_TYPE == IC_TDMAE)  {
	     NST_WR_I2C_REG(IC_TDMAE, IC_DMA_TDLR);
	 }
	 else if(DMA_TYPE == IC_RDMAE) {
	     NST_WR_I2C_REG(IC_RDMAE, IC_DMA_RDLR);
	 }
}

void I2C_WaitForBus(void)
{
    while(NST_RD_I2C_REG(IC_STATUS) & STAT_MST_ACTIVITY 
        ||!(NST_RD_I2C_REG(IC_STATUS) & STAT_TFE)); 
}

void I2C_XferFinish(void)
{
    uint8_t DataByte;
    
    while(1) {
        if(((NST_RD_I2C_REG(IC_RAW_INTR_STAT) & STOP_DET) == STOP_DET)) {
            NST_RD_I2C_REG(IC_CLR_STOP_DET);
            break;
        }
    }
    
    I2C_WaitForBus();
    
    DataByte = DataByte;
    while(NST_RD_I2C_REG(IC_STATUS) & STAT_RFNE) // rx fifo not empty
    {
        DataByte = NST_RD_I2C_REG(IC_DATA_CMD);
    }
}

void BSP_I2CSetAdr(uint32_t Adr,uint32_t time)
{
    int count = 0;

    //默认为AT24C08型号，16位寻址
    // High byte address
    NST_WR_I2C_REG((Adr >> 8) & 0xFF, IC_DATA_CMD);

    // Low byte address
    NST_WR_I2C_REG((Adr & 0xFF), IC_DATA_CMD);
    
	//超时查询I2C应答信号
    while(1) {
	     if((NST_RD_I2C_REG(IC_RAW_INTR_STAT) &  TX_ABRT))
		 {
		      NST_RD_I2C_REG(IC_CLR_TX_ABRT);
			  NST_RD_I2C_REG(IC_CLR_INTR); 
			  NSTusecDelay(count);	
			  NST_WR_I2C_REG((Adr >> 8) & 0xFF, IC_DATA_CMD);
			  NST_WR_I2C_REG((Adr & 0xFF), IC_DATA_CMD);
			  time = 0;	 
		 }
	   	 if(time++ > count ){//|| (NST_RD_I2C_REG(IC_RAW_INTR_STAT) & TX_EMPTY) == TX_EMPTY) {
			  break;
		 }	    
		 NSTusecDelay(1); // 10ms  
	} 
}

BOOL_T BSP_I2CRead(UINT8* pBuf, UINT32 Len,uint32_t time)
{
    int count = 0;
    uint32_t i = 0;
    BOOL_T Ret = NST_TRUE;

    while(i < Len) {
        NST_WR_I2C_REG(IC_RD, IC_DATA_CMD);                      // send read command
        //超时查询I2C应答信号
	    while(1) {
		     if((NST_RD_I2C_REG(IC_RAW_INTR_STAT) &  TX_ABRT))
			 {
			      NST_RD_I2C_REG(IC_CLR_TX_ABRT);
				  NST_RD_I2C_REG(IC_CLR_INTR); 
				  NSTusecDelay(count);	
                  NST_WR_I2C_REG(IC_RD, IC_DATA_CMD);  
				  time = 0;	 
			 }
		   	 if(time++ > count ){//|| (NST_RD_I2C_REG(IC_RAW_INTR_STAT) & TX_EMPTY) == TX_EMPTY) {
				  break;
			 }	    
			 NSTusecDelay(1); // 10ms  
		}	
			
		if((NST_RD_I2C_REG(IC_STATUS) & STAT_RFNE) == STAT_RFNE) { // rx fifo not empty, then read it   
            pBuf[i++] = NST_RD_I2C_REG(IC_DATA_CMD);
        }
        
        if(NST_RD_I2C_REG(IC_RAW_INTR_STAT) & TX_ABRT) {
            Ret = NST_FALSE;
            break;
        }
    }
    
    NST_WR_I2C_REG(1 << 9, IC_DATA_CMD); // stop	   
    
    I2C_XferFinish();    

    return Ret;
}

BOOL_T BSP_I2CWrite(uint8_t* pBuf, uint32_t Len)
{
    uint32_t i = 0;
    BOOL_T Ret = NST_TRUE;

    while(i < Len)
    {
        if(NST_RD_I2C_REG(IC_STATUS) & STAT_TFNF)  // TX fifo not full, then write it
        {
            NST_WR_I2C_REG(pBuf[i], IC_DATA_CMD);
            i++;
        }				  
    }	 
    /* Wait for read/write operation to complete on actual memory */
    NSTusecDelay(20000); // 10ms

    return Ret;
}

BOOL_T BSP_I2CSeqRead(uint32_t Adr, uint8_t *pBuf, uint32_t Len)
{
    BSP_I2CSetAdr(Adr,150);

    return BSP_I2CRead(pBuf, Len, 150);
}

BOOL_T BSP_I2CSeqWrite(uint32_t Adr, uint8_t *pBuf, uint32_t Len)
{
    BSP_I2CSetAdr(Adr,150);
    
    return BSP_I2CWrite(pBuf, Len);
}

/*
******************************************************************************
**                            End Of File                                    **
******************************************************************************
*/

