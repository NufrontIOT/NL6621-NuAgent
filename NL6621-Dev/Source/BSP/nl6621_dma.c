/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  nl6621_dma.c
 *
 *    Description:  This file provides all the DMA firmware functions.
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
#include "nl6621_dma.h"
#include "ucos_ii.h"


typedef OS_EVENT    NST_LOCK;

#ifdef DMA_MOVE_MEM
	OS_EVENT *pDmaSem = NULL;
	PUINT8 pDmaSrc = NULL;
	PUINT8 pDmaDst = NULL;
	UINT16 DmaSize = 0;
#endif // DMA_MOVE_MEM //

/**
 * Lookup Table of DMA Channel Number matched with
 * DMA channel pointer
 */
uint32_t DMAC_CH[4] = {
		DMAC_CH0_START_ADDR,	// DMA Channel 0
		DMAC_CH1_START_ADDR,	// DMA Channel 1
		DMAC_CH2_START_ADDR,	// DMA Channel 2
		DMAC_CH3_START_ADDR,	// DMA Channel 3
};


//dma initialization
void  BSP_DmaInit (uint8_t channel_num)
{
    int reg_val;
    int CTLl_value, CTLh_value;

   //NL6621 supply four dma channels,channel_num must be 0-3
   if(channel_num >3)
   	return;
   
   /////////////////////////////////
   // config dma reg
   // mem --> io tx
    reg_val  = 0;
    reg_val |= (0x0 <<  5);        // channel priority of channel 0        (default value)
    reg_val |= (0x0 <<  8);        // no channel suspend                   (default value)
    reg_val |= (0x0 << 10);        // dst H/W handshaking
    reg_val |= (0x0 << 11);        // src H/W handshaking
    reg_val |= (0x0 << 12);        // channel lock level                   (default value)
    reg_val |= (0x0 << 14);        // bus lock level                       (default value)
    reg_val |= (0x0 << 16);        // channel lock bit                     (default value)
    reg_val |= (0x0 << 17);        // bus lock bit                         (default value)
    reg_val |= (0x0 << 18);        // dst_hs_pol                           (default value)
    reg_val |= (0x0 << 19);        // src_hs_pol                           (default value)
    reg_val |= (0x0 << 20);        // max amba burst length, no limitation (default value)
    reg_val |= (0x0 << 30);        // src automatic reload disable         (default value)
    reg_val |= (0x0 << 31);        // dst automatic reload disable         (default value)
    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CFG_OFFSET,reg_val);

    reg_val |= (0x0 << (32-32));   // FCMODE                               (default value)
    reg_val |= (0x0 << (33-32));   // FIFO_MODE                            (default value)?
    reg_val |= (0x1 << (34-32));   // PROTCTL                              (default value)
    reg_val |= (0x0 << (37-32));   // DS_UPD_EN                            (default value)
    reg_val |= (0x0 << (38-32));   // SS_UPD_EN                            (default value)
    reg_val |= (0x6 << (39-32));   // SRC_PER
    reg_val |= (0x7 << (43-32));   // DES_PER
    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CFG_OFFSET + 4,reg_val);

 #ifdef USE_DMA_INT
    // set dma irq mask
    reg_val = 0;
    reg_val |= (0x1 <<channel_num);
    reg_val |= (0x1 <<(channel_num + 8));
    NST_WR_DMA_REG(DMAC_MASKTFR, reg_val); 
    NST_WR_DMA_REG(DMAC_MASKBLOCK, 0x0); 
    NST_WR_DMA_REG(DMAC_MASKSRCTRAN, 0x0); 
    NST_WR_DMA_REG(DMAC_MASKDSTTRAN, 0x0); 
    NST_WR_DMA_REG(DMAC_MASKERR, 0x0); 

    CTLl_value  = 0;               // must clear init value
    CTLl_value |= (0x1 << 0);      // enable interrupt
    CTLl_value |= (0x0 << 1);      // dst_tr_width is fixed to 32bit, not configrable
    CTLl_value |= (0x0 << 4);      // src_tr_width is fixed to 32bit, not configrable
    CTLl_value |= (0x2 <<  7);     // dst addr no change(2)
    CTLl_value |= (0x0 <<  9);     // src addr increment(0)
    CTLl_value |= (0x4 << 11);     // dest_msize 32 data items
    CTLl_value |= (0x4 << 14);     // src_msize 32 data items
    CTLl_value |= (0x1 << 20);     // transfer type and flow contorl: memory to per
    CTLl_value |= (0x0 << 27);     // llp_dst_en
    CTLl_value |= (0x0 << 28);     // llp_src_en

    CTLh_value  = DMA_TRANSFER_SIZE;//8;               // block transfer size: 127

    // use default cfg value
    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CTL_OFFSET,CTLl_value);
    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CTL_OFFSET + 4,CTLh_value);
 #else  
 
    CTLl_value  = 0;
    CTLl_value |= (0x0 <<  0);        // disable interrupt
    CTLl_value |= (0x2 <<  1);        // dst_tr_width 32bit
    CTLl_value |= (0x2 <<  4);        // src_tr_width 32bit
    CTLl_value |= (0x2 <<  7);        // dst addr no change(2)
    CTLl_value |= (0x0 <<  9);        // src addr increment(0)
    CTLl_value |= (0x1 << 11);        // dst_msize 4 data items
    CTLl_value |= (0x1 << 14);        // src_msize 4 data items
    CTLl_value |= (0x0 << 17);        // src gather disable
    CTLl_value |= (0x0 << 18);        // dst scatter disable
    CTLl_value |= (0x1 << 20);        // transfer type and flow contorl: memory to peripheral (FC:dmac)
    CTLl_value |= (0x0 << 23);        // dst: ahb master 1
    CTLl_value |= (0x0 << 25);        // scr: ahb master 1
    CTLl_value |= (0x0 << 27);        // llp_dst_en: disable
    CTLl_value |= (0x0 << 28);        // llp_src_en: disable

    CTLh_value  = 16;//8;             // block transfer size: 16
    
    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CTL_OFFSET,CTLl_value);    
    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CTL_OFFSET + 4,CTLh_value); // block transfer size: 16 data items, cannot be 32
 #endif //USE_DMA_INT
}

#ifdef DMA_MOVE_MEM
	//dma initialization
	void  BSP_DmaMoveMemInit (uint8_t channel_num)
	{
	    int reg_val;
	    int CTLl_value, CTLh_value;
	
	    //NL6621 supply four dma channels,channel_num must be 0-3
	    if(channel_num >3)
	        return;
	    
	    pDmaSem = OSSemCreate(0);
	    if (pDmaSem == NULL)
	    {
	        printf("pDmaSem is NULL \n");
	    }
	   
	   /////////////////////////////////
	   // config dma reg
	   // mem --> io tx
	    reg_val  = 0;
	    reg_val |= (0x0 <<  5);        // channel priority of channel 0        (default value)
	    reg_val |= (0x0 <<  8);        // no channel suspend                   (default value)
	    reg_val |= (0x0 << 10);        // dst H/W handshaking
	    reg_val |= (0x0 << 11);        // src H/W handshaking
	    reg_val |= (0x0 << 12);        // channel lock level                   (default value)
	    reg_val |= (0x0 << 14);        // bus lock level                       (default value)
	    reg_val |= (0x0 << 16);        // channel lock bit                     (default value)
	    reg_val |= (0x0 << 17);        // bus lock bit                         (default value)
	    reg_val |= (0x0 << 18);        // dst_hs_pol                           (default value)
	    reg_val |= (0x0 << 19);        // src_hs_pol                           (default value)
	    reg_val |= (0x0 << 20);        // max amba burst length, no limitation (default value)
	    reg_val |= (0x0 << 30);        // src automatic reload disable         (default value)
	    reg_val |= (0x0 << 31);        // dst automatic reload disable         (default value)
	    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CFG_OFFSET,reg_val);
	
	    reg_val |= (0x0 << (32-32));   // FCMODE                               (default value)
	    reg_val |= (0x0 << (33-32));   // FIFO_MODE                            (default value)?
	    reg_val |= (0x1 << (34-32));   // PROTCTL                              (default value)
	    reg_val |= (0x0 << (37-32));   // DS_UPD_EN                            (default value)
	    reg_val |= (0x0 << (38-32));   // SS_UPD_EN                            (default value)
	    reg_val |= (0x6 << (39-32));   // SRC_PER
	    reg_val |= (0x7 << (43-32));   // DES_PER
	    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CFG_OFFSET + 4,reg_val);
	
	    /////////////////////////////////
	    // set dma irq mask
	    reg_val = 0;
	    reg_val |= (0x1 <<channel_num);
	    reg_val |= (0x1 <<(channel_num + 8));
	    NST_WR_DMA_REG(DMAC_MASKTFR, reg_val); 
	    NST_WR_DMA_REG(DMAC_MASKBLOCK, 0x0); 
	    NST_WR_DMA_REG(DMAC_MASKSRCTRAN, 0x0); 
	    NST_WR_DMA_REG(DMAC_MASKDSTTRAN, 0x0); 
	    NST_WR_DMA_REG(DMAC_MASKERR, 0x0); 
	
		   
	    CTLl_value  = 0;                  // must clear init value
	    CTLl_value |= (0x1 << 0);         // enable interrupt	  (如果希望中断使能，则在INT_EN中使能该位)
	    CTLl_value |= (0x5 << 1);         // dst_tr_width 256 bit 保留此参数，没有这功能(该field决定了目的端的端口宽度，根据具体情况来确定。)
	    CTLl_value |= (0x5 << 4);         // src_tr_width 256 bit 保留此参数，没有这功能(该field决定了源端的端口宽度，根据具体情况来确定。)
	    CTLl_value |= (0x0 <<  7);        // dst addr increment(0)(该field决定了目的端传输数据的地址是递增，还是递减，或者不变。)
	    CTLl_value |= (0x0 <<  9);        // src addr increment(0)(该field决定了源端传输数据的地址是递增，还是递减，或者不变。一般，外设的地址是不变的，memory的地址是递增，或者递减的。)
	    CTLl_value |= (0x7 << 11);        // dest_msize 256 data items
	    CTLl_value |= (0x7 << 14);        // src_msize 256 data items
	    CTLl_value |= (0x0 << 17);        // src gather disable
	    CTLl_value |= (0x0 << 18);        // dst scatter disable
	    CTLl_value |= (0x0 << 20);        // transfer type and flow contorl: memory to memory （此field决定了此次传输的类型，是memory到memory传输，还是memory到peripheral传输，或者其它情况，可以参照对应的编码表来具体选择。）
	    CTLl_value |= (0x0 << 23);        // dst: ahb master 1 保留此参数，没有这功能（该field决定了源端位于DMA的哪个master端口，对于目前的系统架构，只有DDR2是位于DMA的master1端口，其它设备位于DMA的master2端口。）
	    CTLl_value |= (0x0 << 25);        // scr: ahb master 1 保留此参数，没有这功能（该field决定了目的端口是位于DMA的哪个master端口。）
	    CTLl_value |= (0x0 << 27);        // llp_dst_en: disable  由于此处是single block传输下的配置，CTL0寄存器中的LLP_SRC_EN,LLP_DST_EN fields要置0
	    CTLl_value |= (0x0 << 28);        // llp_src_en: disable  由于此处是single block传输下的配置，CTL0寄存器中的LLP_SRC_EN,LLP_DST_EN fields要置0
	
	    CTLh_value  = DMA_TRANSFER_SIZE;    // block transfer size: 127
	
	    // use default cfg value
	    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CTL_OFFSET,CTLl_value);
	    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CTL_OFFSET + 4,CTLh_value);
	}
#endif // DMA_MOVE_MEM //


//DMA Start
void BSP_DmaStart(uint32_t* pSrc, uint32_t* pDest, uint8_t channel_num)  
{
    int reg_val;

   //NL6621 supply four dma channels,channel_num must be 0-3
    if(channel_num >3)
   	  return;
	
    /////////////////////////////////
    // config DMA reg start DMA
    
    // Clear channel suspend   
    reg_val  = NST_RD_DMA_REG(DMAC_CH[channel_num]+ DMAC_CFG_OFFSET);
    reg_val &= ~(0x1 <<  8);          
    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CFG_OFFSET,reg_val);	

    // Enable DMAC
    NST_WR_DMA_REG(DMAC_DMACFGREG,0x1); 
    // Set DMA Source addr
    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_SAR_OFFSET, pSrc);
    // Set DMA Destination addr
    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_DAR_OFFSET, pDest);
    // Enable DMA channelX
    reg_val = 0;
    reg_val |= (0x1 <<channel_num);
    reg_val |= (0x1 <<(channel_num + 8));
    NST_WR_DMA_REG(DMAC_CHENREG,reg_val);

   // Register DMA complete callback function
}

//DMA Halt
void BSP_DmaHalt(uint8_t channel_num)  
{
    int reg_val;

    //NL6621 supply four dma channels,channel_num must be 0-3
    if(channel_num >3)
        return;
   
    /////////////////////////////////
    // config DMA reg halt DMA

    // channel suspend
    reg_val  = NST_RD_DMA_REG(DMAC_CH[channel_num]+ DMAC_CFG_OFFSET);
    reg_val |= (0x1 <<  8);                    
    NST_WR_DMA_REG(DMAC_CH[channel_num]+ DMAC_CFG_OFFSET,reg_val);	
}


//dma deinitialization
void  BSP_DmaDeinit (void)
{
    // Disable DMAC
    NST_WR_DMA_REG(DMAC_DMACFGREG,0x0);    
}


//DMA INT ISR(example for I2s audio test)
void BSP_DmaIntISR(void)
{    

    NVIC_DisableIRQ(DMA_IRQn);
    NVIC_ClearIRQChannelPendingBit(DMA_IRQn);
	
    // DMA complete Interrupt for channel 0
    if((NST_RD_DMA_REG(DMAC_STATUSTFR) & (0x1 << 0)))
    {
        NST_WR_DMA_REG(DMAC_CLEARTFR, (0x1 << 0)); 
		
        //Use DMA channel 0 for i2s test
#ifdef HW_I2S_SUPPORT
        BSP_I2SDMAWrite();
#endif //HW_I2S_SUPPORT

    }

  // DMA complete Interrupt for channel 1
  if((NST_RD_DMA_REG(DMAC_STATUSTFR) & (0x2 << 0)))
    {
        NST_WR_DMA_REG(DMAC_CLEARTFR, (0x2 << 0)); 

        // Use DMA CH 1 for memory move
#ifdef DMA_MOVE_MEM
        //printf("DMA INT \n");
        // Next Block
        pDmaDst += DMA_TRANSFER_SIZE_IN_BYTE;
        pDmaSrc += DMA_TRANSFER_SIZE_IN_BYTE;
        DmaSize -= DMA_TRANSFER_SIZE_IN_BYTE;
        if (DmaSize > DMA_TRANSFER_SIZE_IN_BYTE)
        {
            BSP_DmaStart(pDmaDst, pDmaSrc, DMA_CHANNEL_1);
        }
        else
        {
            //printf("pDmaSem \n");
            OSSemPost(pDmaSem);
        }
#endif // DMA_MOVE_MEM //
    }

  // DMA complete Interrupt for channel 2
  if((NST_RD_DMA_REG(DMAC_STATUSTFR) & (0x4 << 0)))
    {
        NST_WR_DMA_REG(DMAC_CLEARTFR, (0x4 << 0)); 
	//TODO:when DMA(channel 2) interrupt is complete, insert here what you want to do.
	
    }

   // DMA complete Interrupt for channel 3
   if((NST_RD_DMA_REG(DMAC_STATUSTFR) & (0x8 << 0)))
    {
        NST_WR_DMA_REG(DMAC_CLEARTFR, (0x8 << 0)); 
	//TODO:when DMA(channel 3) interrupt is complete, insert here what you want to do.
	
    }

    NVIC_EnableIRQ(DMA_IRQn);
}

/*
******************************************************************************
**                            End Of File                                    **
******************************************************************************
*/

