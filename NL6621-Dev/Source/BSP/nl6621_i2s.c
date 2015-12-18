/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  nl6621_dma.c
 *
 *    Description:  This file provides all the I2S firmware functions.
 *
 *        Version:  0.0.1
 *        Created:  2015/12/9 15:47:43
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
#include "nl6621_i2s.h"
#include "includes.h"
#include "ring_buffer.h"


#ifdef HW_I2S_SUPPORT
 

unsigned int buf0cpy[256] ={0};

//I2S 采样位数24bit 采样频率48KHZ、飞利浦格式 发送数据变量定义   
#pragma arm section rwdata="win_data"   

//should be delete after test
unsigned short buf0[0x400] = {
20,90,14,126,65523,138,65488,114,65437,45,65381,65432,65326,65231,65270,65030,
65222,64894,65170,64811,65081,64740,64988,64719,64912,64840,64885,65097,64925,65359,64974,19,
65035,209,65097,309,65177,283,65205,127,65209,65441,65136,65099,64990,64669,64819,64374,
64760,64419,64868,64867,65129,56,65473,770,379,1397,892,1904,1332,2169,1546,2039,
1403,1477,908,629,234,65314,65058,64687,64432,64341,64087,64292,64113,64513,64412,64854,
64893,65217,65406,29,363,303,732,414,886,236,828,65269,643,64626,415,64136,
114,63895,65295,63858,64987,64101,64822,64664,64842,65407,65079,609,72,1273,830,1855,
1535,2183,1922,2073,1880,1587,1435,923,756,284,35,65247,65054,64809,64769,64662,
64655,64797,64668,65123,64776,6,65080,521,43,1038,551,1346,927,1242,1118,650,
1093,65251,816,64202,288,63184,65035,62419,64099,62094,63226,62255,62600,62823,62456,63650,
62950,64650,64006,210,65334,1220,932,1917,1608,2199,1773,2119,1549,1797,1114,1263,
599,635,65470,65486,64673,64725,63932,64041,63525,63674,63745,63753,64500,64201,65510,64829,
844,65329,1349,65506,1481,65443,1344,65253,1000,64997,439,64761,65176,64584,64239,64460,
63435,64477,63046,64736,63231,65231,63867,344,64672,1014,65337,1494,274,1685,598,1593,
880,1238,1279,778,1678,418,1870,147,1688,65425,1212,65237,740,65296,391,65532,
225,321,202,675,227,859,172,756,65516,320,65280,65198,65037,64513,64712,63969,
64296,63618,63879,63486,63658,63678,63764,64238,64200,65119,64991,704,416,1846,1263,2683,
1783,3092,1983,3006,1991,2488,1987,1735,1982,1022,1782,335,1208,65202,321,64641,
64793,64148,63863,63794,63389,63771,63414,64023,63785,64382,64232,64711,64643,64919,65112,65020,
184,65084,789,65197,1094,65234,955,65178,477,65111,65449,65075,65121,65212,65260,74,
246,608,844,1081,1184,1329,1196,1317,901,1049,385,630,65200,76,64229,64952,
63088,64360,61948,63858,60992,63408,60592,63171,61079,63307,62468,63777,64356,64467,632,65330,
1973,641,2716,1286,2915,1652,2539,1651,1615,1318,393,856,64618,297,63487,65269,
62934,64914,63337,64924,64692,65228,854,153,2343,673,3363,1101,3850,1367,3869,1442,
3315,1244,2218,858,669,382,64303,65236,62380,64427,61047,63761,60946,63522,62169,63780};

int ptr = 0;
#pragma arm section

//I2S 采样位数24bit 采样频率48KHZ、飞利浦格式 接收数据变量定义 
ring_buffer_t i2srxbuf;	 //i2s rx test fifo。
volatile unsigned char rx32_flag = 0;

//I2S 采样位数24bit 采样频率48KHZ、飞利浦规定的格式.
void I2S_RX_Test(void)
{
    int i = 0;

    BSP_I2SInit();				  //I2S外设初始化
	NSTusecDelay(5000); 
	/* Init uart receive ring buffer */
	ring_buf_alloc(&i2srxbuf, 20480); //申请2K的数据给I2S RX

	BSP_I2SStart(I2S_RX_Channel0_ENABLE);

	while (1) {
        if(rx32_flag == 1 && ring_buf_cnt(&i2srxbuf) >= 1024 )
		{
			BSP_I2SHalt();
			ring_buf_read(&i2srxbuf,(char *)buf0cpy,1024);
			printf("I2S Receive 256*24bit data:\r\n");
			for(i=0;i<256;i++)
		      printf("%06X ",buf0cpy[i]);
		    printf("\r\n");
		   	NSTusecDelay(500000); 
			NSTusecDelay(500000);
        	BSP_I2SStart(I2S_RX_Channel0_ENABLE);
		}
	}
}

//I2S 采样位数24bit 采样频率48KHZ、飞利浦格式.
void I2S_TX_Test(void)
{
    BSP_I2SInit();				  //I2S外设初始化
    BSP_DmaInit(DMA_CHANNEL_0);   //init dma channel 0
	BSP_I2SStart(I2S_TX_Channel0_ENABLE);
	while(1);
}


//I2S init
void BSP_I2SInit(void)  
{
    int reg_val;

	//初始化寄存器
    reg_val  = 0;		
    NST_WR_I2S_REG(reg_val, I2S_CTRL);

	//I2S TX配置采样频率、采样位数
    reg_val |= (39 <<  0);          // tx sample rate: 48kHz; 120MHz/48kHz/32bit/2 = 39; Algorithm 1 on page 78
    reg_val |= (23 << 11);          // tx resolution: 24bit

 	//I2S RX配置采样频率、采样位数
    reg_val |= (39 << 16);       	// rx sample rate: 48kHz; 120MHz/48kHz/32bit/2 = 39; Algorithm 1 on page 78
    reg_val |=  0xB1000000; //        // rx resolution   24bit
    NST_WR_I2S_REG(reg_val, I2S_SRR);
  
    reg_val  = 0;
    reg_val |= (0x18 <<  0);       // <24bits = almost empty
    reg_val |= (0x1B << 16);       // >27bits = almost full

    NST_WR_I2S_REG(reg_val,I2S_TFIFO_CTRL);
    NST_WR_I2S_REG(reg_val,I2S_RFIFO_CTRL);
}


//I2S Start
void BSP_I2SStart(unsigned char channel)  
{
    int reg_val;

    /////////////////////////////////
    // config i2s reg halt i2s
    reg_val  = 0;
    reg_val |= (0x00 <<  0);              // set all clk strobe bits to 0 (enable all clk)
    reg_val |= INTR_EN|CHN0_INTR_EN;     // enable interrupt request generation after underrun / overrun condition occurrence in I2S channel 0.
    NST_WR_I2S_REG(reg_val,I2S_CID_CTRL);
  
#ifdef USE_PWM
    reg_val  = 0;
    reg_val |= ((0x1 << 0) <<  0); // enable channel 0
    reg_val |= ((0x1 << 1) <<  0); // enable channel 1
    reg_val |= ((0x1 << 0) <<  8); // set channel 0 tx
    reg_val |= ((0x0 << 1) <<  8); // set channel 1 rx
    reg_val |= (0x00 << 16);       // no channel loop back (in this case, loop back outside the chip)
    reg_val |= (0x1  << 20);       // clear sfr rst
    reg_val |= (0x0  << 21);       // set tx to slave (although transmit data, do not gen tsck, loop back from rsck!)
    reg_val |= (0x0  << 22);       // set rx to master (not receive data, only gen clk for transmit data!)
    reg_val |= (0x0f << 23);       // clear rst of tfifo/rfifo/tsync/rsync
    reg_val |= (0x0  << 27);       // tsync loop back
    reg_val |= (0x1  << 28);       // no rsync loop back
    NST_WR_I2S_REG(reg_val,I2S_CTRL);
    regdat = PWM_EN|PWM_SAMPLE_RATE_441000|PWM_CHN0;
    NST_WR_I2S_REG(regdat,I2S_PWM_CONF);  
#else
    //Channel 0 发送数据，Channel 1 接收数据
    reg_val  = 0;
    reg_val |= ((0x1 << 0) <<  0); // enable channel 0
   // reg_val |= ((0x1 << 1) <<  0); // enable channel 1
    reg_val |= ((channel) <<  8); // set channel 0 tx
    reg_val |= ((0x0 << 1) <<  8); // set channel 1 rx
    reg_val |= (0x00 << 16);       // no channel loop back (in this case, loop back outside the chip)
    reg_val |= (0x1  << 20);       // clear sfr rst
    reg_val |= (0x0  << 21);       // set tx to slave (although transmit data, do not gen tsck, loop back from rsck!)
    reg_val |= (0x1  << 22);       // set rx to master (not receive data, only gen clk for transmit data!)
    reg_val |= (0x0f << 23);       // clear rst of tfifo/rfifo/tsync/rsync
    reg_val |= (0x1  << 27);       // tsync loop back
    reg_val |= (0x0  << 28);       // no rsync loop back
    NST_WR_I2S_REG(reg_val,I2S_CTRL);
    NST_WR_I2S_REG(TRAN_REC_I2S_MODE,I2S_DEV_CONF);  //TRAN_REC_I2S_RIGHT_MODE
#endif  

#ifdef USE_I2S_DMA
    BSP_DmaStart(buf0cpy,(UINT32 *)(_I2S_BASE_ADR+NI2S_FIFO_BASE), DMA_CHANNEL_0);
#endif //USE_I2S_DMA

}

//I2S 暂停发送和接收数据
//提供 I2S 接口的暂停操作，在使用 PWM 接口时提供 PWM mute 操作，使用 DMA 时提供 DMA suspend
//操作（可与 BSP_I2SStart 成对使用）。
void BSP_I2SHalt(void)  
{
    int reg_val;

    /////////////////////////////////
    // config i2s reg halt i2s
    reg_val  = NST_RD_I2S_REG(I2S_CID_CTRL);
    reg_val |= (0x03 <<  0);       // set channel0&channel1 clk strobe bits to 1 (block clk)
    //reg_val |= (0x1ffff << 15);    // set all irq mask bits to 1 (mask all irq)
    //reg_val &= (INTR_DIS&0xffffffff);
    NST_WR_I2S_REG(reg_val,I2S_CID_CTRL);
	
#ifdef USE_PWM
    /////////////////////////////////
    //config pwm reg mute pwm 
    reg_val  = NST_RD_I2S_REG(I2S_PWM_CONF);
    reg_val |= PWM_MUTE;     
    NST_WR_I2S_REG(regdat,I2S_PWM_CONF);
#endif

#ifdef USE_I2S_DMA
    BSP_DmaHalt(DMA_CHANNEL_0);
#endif //USE_I2S_DMA
}

//提供 I2S 接口的静默操作，在使用 PWM 接口时提供 PWM mute 操作， 多用于 I2S 接口无音频数据时
//的短时静默。（可与 BSP_I2SSound 成对使用）。
//I2S Mute,I2S disable for a while
void BSP_I2SMute(void)  
{
    int reg_val ;
	
    /////////////////////////////////
    // config i2s reg,set i2s mute
    reg_val  = NST_RD_I2S_REG(I2S_CTRL);
    reg_val &= (~(0x1 << 0) );    // disable channel 0
    reg_val &= (~(0x1 << 1) );
    NST_WR_I2S_REG(reg_val,I2S_CTRL);
	
#ifdef USE_PWM
    /////////////////////////////////
    //config pwm reg mute pwm 
    reg_val  = NST_RD_I2S_REG(I2S_PWM_CONF);
    reg_val |= PWM_MUTE;     
    NST_WR_I2S_REG(regdat,I2S_PWM_CONF);
#endif
	
}

//提供 I2S 接口在短时静默之后的恢复操作， 与 BSP_I2SMute 成对使用。
//I2S sound enable, opposite to BSP_I2SMute
void BSP_I2SSound(void)  
{
    int reg_val ;

    /////////////////////////////////
    // config i2s reg, clear i2s mute
    reg_val  = NST_RD_I2S_REG(I2S_CTRL);
    reg_val |= ((0x1 << 0) );    // enable channel 0
    reg_val |= ((0x1 << 1) );
    NST_WR_I2S_REG(reg_val,I2S_CTRL);
	
#ifdef USE_PWM
    /////////////////////////////////
    //config pwm reg clear pwm mute
    reg_val  = NST_RD_I2S_REG(I2S_PWM_CONF);
    reg_val &= (~PWM_MUTE);     
    NST_WR_I2S_REG(regdat,I2S_PWM_CONF);
#endif

}

//提供 I2S 接口的卸载操作。
//I2S Deinit
void BSP_I2SDeinit(void)  
{
    int reg_val;

    // config i2s reg
    reg_val  = 0;
    NST_WR_I2S_REG(reg_val,I2S_CTRL);
  
    reg_val  = 0;
    reg_val |= (0xFF <<  0);       // set all clk strobe bits to 1 (block all clk)
    reg_val |= (0x0 << 15);        // set all irq mask bits to 0 (disable all irq)
    NST_WR_I2S_REG(reg_val,I2S_CID_CTRL);
	
}


//I2S write in Polling mode,just an example
void BSP_I2SWrite(void * buf,  unsigned int len)
{
    unsigned int * cur = buf;		
    //int regdat = 0;
    int idx = 0;
    unsigned int temp = 0;
	static int regdat = 0;
	static int stat = 0;
	
    while(1)
    {
        stat = NST_RD_I2S_REG(I2S_TFIFO_STAT);
        //regdat = 0; 
        //NST_WR_I2S_REG(regdat, I2S_STAT);
        if(stat<20)
        {
            temp = *cur;
            temp &=0xffff;
            regdat = (unsigned int)(temp<<16);//due to the i2s config, if i2s set 16bit
            regdat |= temp;
            //regdat = flag*16383;
            NST_WR_I2S_REG(regdat,NI2S_FIFO_BASE);
            NST_WR_I2S_REG(regdat,NI2S_FIFO_BASE);
            //*((unsigned int *)NI2S_FIFO_BASE)= (unsigned int)((*cur1)<<0);//due to the i2s config
            //if(idx<2048)
            //buf1[idx] = (unsigned int )regdat;
            cur += 1;
            idx++;
            //if((unsigned int)cur0-(unsigned int)buf0>len)
            if (idx>=len)
            {
                idx = 0;
                cur=buf;
            }
        }	
    }
	
}


//I2S write in DMA mode,just an example
/*
DMA mode:
        if use DMA, you must copy audio data to DMASAR,and set DMADAR to NI2S_FIFO_BASE
        NST_WR_DMA_REG(DMAC_SAR0 ,buf0cpy);
        NST_WR_DMA_REG(DMAC_DAR0 ,_I2S_BASE_ADR+NI2S_FIFO_BASE);
*/
void BSP_I2SDMAWrite(void)
{
    int i = 0,reg_val = 0;
    unsigned short s_temp = 0;
    
#ifdef USE_I2S_DMA_INT
    //此处为循环发送8K音频数据
    ptr +=  DMA_TRANSFER_SIZE * 2;
    if(ptr >= 0x400)  ptr = 0;
    for(i=0;i< DMA_TRANSFER_SIZE * 2;i++)
    {
        //目前I2S为24bit，DMA为32bit,其中低24bit为有效数据，高8bit补0
        s_temp = buf0[i+ptr];
        buf0cpy[i] = (unsigned int)(s_temp);           
    }

	//采用DMA传输数据，使能TX中断
    reg_val  = 0;
    reg_val |= INTR_EN | CHN0_INTR_EN;
    NST_WR_I2S_REG(reg_val,I2S_CID_CTRL); 

    BSP_DmaStart(buf0cpy, (UINT32 *)(_I2S_BASE_ADR+NI2S_FIFO_BASE), DMA_CHANNEL_0);
#endif //USE_I2S_DMA_INT

}

// I2S INT ISR
void BSP_I2SIntISR(void)
{
    UINT32 i2s_tfifo_stat =0;
    UINT32 i2s_rfifo_stat =0;
    UINT32 i2s_stat =0;
    UINT32 i,rfifo_val,reg_val;

    NVIC_DisableIRQ(I2S_IRQn);
    NVIC_ClearIRQChannelPendingBit(I2S_IRQn);


    NST_WR_I2S_REG(0,I2S_CID_CTRL);
   
    i2s_tfifo_stat = NST_RD_I2S_REG(I2S_TFIFO_STAT);
    i2s_rfifo_stat = NST_RD_I2S_REG(I2S_RFIFO_STAT);
    i2s_stat = NST_RD_I2S_REG(I2S_STAT);
    DBGPRINT(DEBUG_INFO, "I2s int tsts 0x%x  rsts 0x%x  i2ssts 0x%x \n", i2s_tfifo_stat,i2s_rfifo_stat,i2s_stat);

    if (i2s_stat & 0x4000 && i2s_rfifo_stat >= 32) {	// rfifo_full
	   for(i=0; i<32; i++) {	 
		  rfifo_val = NST_RD_I2S_REG(NI2S_FIFO_BASE);
	      ring_buf_write(&i2srxbuf, (char *)&rfifo_val, 4);	
	   }
    }	 
   	rx32_flag = 1;

	if (i2s_stat & 0x0010) 		// rdata_overrerr
		NST_WR_I2S_REG(i2s_stat & ~(0x0010), I2S_STAT);
//    if(i2s_stat & 0x00000001) 
//        NST_WR_I2S_REG(i2s_stat & 0xFEEE, I2S_STAT);

    if (ring_buf_cnt(&i2srxbuf) < 1025	)
	{
	    reg_val  = 0;
	    reg_val |= INTR_EN | CHN0_INTR_EN;
	    NST_WR_I2S_REG(reg_val,I2S_CID_CTRL); 
	}
    NVIC_EnableIRQ(I2S_IRQn);
}

#endif
/*
******************************************************************************
**                            End Of File                                   **
******************************************************************************
*/



