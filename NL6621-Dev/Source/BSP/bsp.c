/*
******************************************************************************
**
** Project     : 
** Description :    BSP
** Author      :    pchn                             
** Date        : 
**    
** UpdatedBy   : 
** UpdatedDate : 
**
******************************************************************************
*/
//------------------------------------------------------------------------------------------------------
#include "includes.h"
#include "bsp.h"
#include "mpu.h"

uint32_t CpuClkFreq = CPU_CLK_FREQ_160M;
uint32_t ApbClkFreq = APB_CLK_FREQ_40M;

NV_INFO EeBuffer;

extern void BSP_RFInit(void);
#ifdef ADD_IQ_CALIBRATION
extern void IQCalibration(void);
#endif
extern void BSP_SmidLowMacIntEnable(void);
extern void BSP_SetPwmRegDefaultVal(void);
extern void BSP_SetMacRegDefaultVal(void);
extern int32_t  BSP_NvInfoInit(void);

/*
******************************************************************************
**                        VOID  BSP_GlobalIrqIntEnable (VOID)
**
** Description  : enable IRQ global int
** Arguments    : 
                  
                  
** Returns      : 无
** Author       :                                   
** Date         : 
**
******************************************************************************
*/
/*CPSR/SPSR:SPSR 只有Arm模式才能识别
  b31
  N(neg) |Z(Zero) |C(Carry)| V(Over) |Q(饱和) | J |  Rsv(b25:b8)| GE[3:0](>=) | E(Endian)| A(禁止abort) | I(禁止IRQ) | F(禁止FIQ) | T(thumb/arm)| mode(usr/FIQ/IRQ/SVC/ABT/UNDEFINE/SYSTEM)
*/
void  BSP_GlobalIrqIntEnable (void)
{
// 清IRQbit使能IRQ全局中断      
 //OS_CPU_ARM_CONTROL_INT_DIS      EQU  0xC0                     ; Disable both FIQ and IRQ. 
 #ifdef USE_CORTEX_M3
    CPU_IntEn();
 #else
 __asm
   {   
      MRS     R0, CPSR
      BIC     R0, R0, #0x80
      MSR     CPSR_c, R0           
   }                
 #endif
}

/*
******************************************************************************
**                        VOID  BSP_GlobalIrqIntDisable (VOID)
**
** Description  : disable global IRQ int
** Arguments    : 
                  
                  
** Returns      : 无
** Author       :                                   
** Date         : 
**
******************************************************************************
*/
void  BSP_GlobalIrqIntDisable (void)
{
     //置IRQbit禁止IRQ中断  
     
 #ifdef USE_CORTEX_M3
     CPU_IntDis();
 #else
     __asm
    {   
       MRS     R0, CPSR 
       ORR     R1, R0, #0x80
       MSR     CPSR_c, R1           
    }
#endif
}


/*
******************************************************************************
**                        VOID  BSP_IntCtrlInit (BOOL_T bSdioInitDone)
**
** Description  : int controller init
** Arguments    : 
                  
                  
** Returns      : 无
** Author       :                                   
** Date         : 
**
******************************************************************************
*/

// USE nufront link board
static  void  BSP_IntCtrlInit (BOOL_T bSdioInitDone)
{
    uint32_t   AIRCR;
    
    // TODO中断控制器寄存器参数初始化
    NVIC_InitTypeDef NVIC_InitStructure;

    //  关全局中断
    if(!bSdioInitDone) // SDIO不掉电，SRAM掉电后加载不能关中断
        BSP_GlobalIrqIntDisable();

     // SET priority group [10:8] = 4   
    AIRCR = ((NST_SCB->AIRCR & (~0x700)) | (AIRCR_PRI_GROUP << 8)); // [7:5] preemption prio  [4:0] sub priority
    // 写AIRCR需要有key，只有高16bit为0x05FA时才能写进去
    NST_SCB->AIRCR = (AIRCR & 0x0000FFFF) | AIRCR_VECT_KEY;; 
    
    /* Configure and enable WAKE UP CPU interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = WAKEUP_CPU_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = WAKEUP_CPU_IRQn_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure and enable SMID interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = SMID_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SMID_IRQn_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure and enable LOW MAC interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = LOW_MAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_MAC_IRQn_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure and enable timer0 interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TMR0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TMR0_IRQn_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure and enable timer1 interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TMR1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TMR1_IRQn_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure and enable WAKE UP CPU interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = WWDG0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = WWDG0_IRQn_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Configure DMA interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DMA_IRQn_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
    NVIC_Init(&NVIC_InitStructure);

#ifdef HW_I2S_SUPPORT
    /* Configure I2s interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = I2S_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = I2S_IRQn_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif //HW_I2S_SUPPORT

    /* Configure UART interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = UART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART_IRQn_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE;
    NVIC_Init(&NVIC_InitStructure);


   NST_SCB->SHCSR = (NST_SCB->SHCSR |(SCB_SHCSR_MEMFAULTENA | SCB_SHCSR_BUSFAULTENA | SCB_SHCSR_USGFAULTENA)); 

// cpu stop if fault occurs
    NST_SCB->DEMCR = (NST_SCB->DEMCR |SCB_DEMCR_HARDERR|SCB_DEMCR_BUSERR
                                 |SCB_DEMCR_CHKERR|SCB_DEMCR_STATERR
                                 |SCB_DEMCR_NOCPERR|SCB_DEMCR_MMERR);

    BSP_SmidLowMacIntEnable();
    
    BSP_GlobalIrqIntEnable();

}

/*
******************************************************************************
**                  VOID  BSP_ClkInit ()
**
** Description  : BSP CLK mode
** Arguments    : 
                  
                  
** Returns      : 无
** Author       :                                   
** Date         : 
**
******************************************************************************
*/
void  BSP_ClkInit (void)
{
	// clk init
	/*CHIP MODE
	      31:4    |        5          |       4:3   |           2           |    1:0           
	       Rsv    | CPU Sel ena  |   CPU Sel |   Bandwith Ena  |  Bandwith
	
	CLK_CTRL
	      31:7     |             4         |            3         |            2         |     1:0 
	      Rsv      | APB2_GATE_E    | APB1_GATE_E  | WLAN_GATE_E  |  APB_SEL
	*/

    if(CpuClkFreq == CPU_CLK_FREQ_160M)
    {
        NST_WR_PWM_REG(ADDR_CHIP_MODE, CHIP_BW_MODE_20M|CHIP_BW_ENA|(CPU_CLK_MODE_160M<<CPU_CLK_SEL)|CPU_SEL_ENA|CHIP_SMID_RST_DONE);
        NST_WR_PWM_REG(ADDR_CLK_CTRL, APB_CLK_MODE_40M|WLAN_GATE_ENA|APB1_GATE_ENA|APB2_GATE_ENA);
        ApbClkFreq = APB_CLK_FREQ_40M;
    }
    else if(CpuClkFreq == CPU_CLK_FREQ_120M)
    {
        NST_WR_PWM_REG(ADDR_CHIP_MODE, CHIP_BW_MODE_20M|CHIP_BW_ENA|(CPU_CLK_MODE_120M<<CPU_CLK_SEL)|CPU_SEL_ENA|CHIP_SMID_RST_DONE);
        NST_WR_PWM_REG(ADDR_CLK_CTRL, APB_CLK_MODE_40M|WLAN_GATE_ENA|APB1_GATE_ENA|APB2_GATE_ENA);
        ApbClkFreq = APB_CLK_FREQ_40M;
    }
    else if(CpuClkFreq == CPU_CLK_FREQ_80M)
    {
        NST_WR_PWM_REG(ADDR_CHIP_MODE, CHIP_BW_MODE_20M|CHIP_BW_ENA|(CPU_CLK_MODE_80M<<CPU_CLK_SEL)|CPU_SEL_ENA|CHIP_SMID_RST_DONE);
        NST_WR_PWM_REG(ADDR_CLK_CTRL, APB_CLK_MODE_20M|WLAN_GATE_ENA|APB1_GATE_ENA|APB2_GATE_ENA);
        ApbClkFreq = APB_CLK_FREQ_20M;
    }
    else if(CpuClkFreq == CPU_CLK_FREQ_40M)
    {
        NST_WR_PWM_REG(ADDR_CHIP_MODE, CHIP_BW_MODE_20M|CHIP_BW_ENA|(CPU_CLK_MODE_40M<<CPU_CLK_SEL)|CPU_SEL_ENA|CHIP_SMID_RST_DONE);
        NST_WR_PWM_REG(ADDR_CLK_CTRL, APB_CLK_MODE_10M|WLAN_GATE_ENA|APB1_GATE_ENA|APB2_GATE_ENA);
        ApbClkFreq = APB_CLK_FREQ_10M;
    }

}

/*
******************************************************************************
**                        VOID  BSP_Init (VOID)
**
** Description  : BSP initialize
** Arguments    : 
                  
                  
** Returns      : 无
** Author       :                                   
** Date         : 
**
******************************************************************************
*/
void  BSP_Init (void)
{
    BSP_SetPwmRegDefaultVal();	    //设置PWM默认值，切勿改变。
    BSP_SetMacRegDefaultVal();      //设置MAC默认值，切勿改变。

    BSP_IntCtrlInit(NST_FALSE);     //Cortex-M3中断优先级	                              

    BSP_RFInit();					//RF射频初始化

    MPU_Init();						//MPU内存规划

#ifdef ADD_IQ_CALIBRATION
    IQCalibration();				//校准参数
#endif

    USART_Init_Demo(DEFAULT_UART_BAUDRATE); //初始化串口，波特率115200

    BSP_NvInfoInit();               //FLASH初始化，校验系统数据
    
	
	NL6621_BSP_TEST(); //NL6621 BSP测试			           
}

#ifdef USE_NV_INFO

static void CheckParam(PNV_INFO pNvInfo)
{
	extern BOOL_T CheckAuthCode(VOID);
	extern  const UINT8                  ZERO_MAC_ADDR[MAC_ADDR_LEN];
	#define COPY_MAC_ADDR(Addr1, Addr2)  NST_MOVE_MEM((Addr1), (Addr2), MAC_ADDR_LEN)
	#define MAC_ADDR_IS_GROUP(Addr)      (((Addr[0]) & 0x01))
	#define TX_GAIN_MAP_TBL_SIZE         0x28//0x26

    UINT32  i = 0;

    // CHECK TX PWR
    for (i=0; i<MAX_TXPOWER_ARRAY_SIZE*MAX_CH_NUM; i++)
        if (*((PUINT8)&pNvInfo->MaxTxPwr[0] + i) > (TX_GAIN_MAP_TBL_SIZE-1))
        {
            *((PUINT8)&pNvInfo->MaxTxPwr[0] + i) = *((PUINT8)&DefaultTxPwrIdxTbl[0] + i);
        }
    if (i != MAX_TXPOWER_ARRAY_SIZE*MAX_CH_NUM)
    {
        DBGPRINT(DEBUG_INFO, "Power of NvInfo is invalid [%d] = 0x%0x \n", i, *((PUINT8)&pNvInfo->MaxTxPwr[0] + i));
        NST_MOVE_MEM(pNvInfo->MaxTxPwr, DefaultTxPwrIdxTbl, sizeof(pNvInfo->MaxTxPwr));
    }

    // CHECK MAC ADDR
    if ((!MAC_ADDR_IS_GROUP(pNvInfo->MacAddr)) && (!NST_EQUAL_MEM(pNvInfo->MacAddr, ZERO_MAC_ADDR, MAC_ADDR_LEN)))
        COPY_MAC_ADDR(PermanentAddress, pNvInfo->MacAddr);

#ifdef CHECK_AUTH_CODE
    if (CheckAuthCode() == NST_FALSE)
    {
        DBGPRINT(DEBUG_TRACE, "Check auth code failed, dead loop\n");
        while (1)
            ;
    }
#endif

}

#endif // USE_NV_INFO

int32_t  BSP_NvInfoInit(void)
{
    PNV_INFO pNvInfo = (PNV_INFO)&EeBuffer;

#ifndef USE_NV_INFO

    //
    // 1: USE DEFAULT PARAM
    //
    NST_MOVE_MEM(pNvInfo->MaxTxPwr, DefaultTxPwrIdxTbl, sizeof(pNvInfo->MaxTxPwr));

#else   // USE_NV_INFO

    //
    // 2: USE FLASH PARAM
    //

    BSP_QSpiInit(DEFAULT_QSPI_CLK);

    QSpiFlashRead(NV_INFO_START, (UINT8*)pNvInfo, sizeof(NV_INFO));

    // CHECK PARAM
    CheckParam(pNvInfo);
            
#endif // USE_NV_INFO
    
    return NST_STATUS_SUCCESS;
}



/*
******************************************************************************
**                        VOID BSP_WakeupCpuIntISR(VOID)
**
** Description  : Wakeup cpu INT handler
** Arguments    : 
                  
                  
** Returns      : 无
** Author       :                                   
** Date         : 
**
******************************************************************************
*/
void BSP_WakeupCpuIntISR(void)
{
    NST_TskMsg* msg;

    //DBGPRINT_PS(DEBUG_TRACE, "MLME_AUTO_WAKEUP_ID\n");
    
    //Set Mask
    NST_WR_PWM_REG(ADDR_WAKEUP_CPU_MASK, 0x00000003);
    
    //  关WAKE UP中断
    NVIC_DisableIRQ(WAKEUP_CPU_IRQn);
    /*  清M3 的中断*/
    NVIC_ClearIRQChannelPendingBit(WAKEUP_CPU_IRQn);
    //发消息给syscore
    msg = NST_AllocTskMsg();
    if(msg)
    {
        msg->msgId = MLME_AUTO_WAKEUP_ID;  
     //   NST_ZERO_MEM(msg->msgBody, sizeof(msg->msgBody));
        
        NST_SendMsg(gpMacMngTskMsgQ, msg);
    }
    // 开中断
    NVIC_EnableIRQ(WAKEUP_CPU_IRQn);
}

/*
******************************************************************************
**                            End Of File                                    **
******************************************************************************
*/
