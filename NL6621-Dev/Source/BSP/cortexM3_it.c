/*
******************************************************************************
**
** Project     : 
** Description :    Cortex-M3 interrupt interface 
** Author      :    pchn                             
** Date        : 
**    
** UpdatedBy   : 
** UpdatedDate : 
**
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "log.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern void BSP_LowMacIntISR(void);
extern void Simu_UartIRQ_Func(void);
extern void SimuUartOutPut(void);
extern int  simu_uart_timer_task(void);

/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void NMIException(void)
{
}

/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__asm void FaultHandler_asm(void)
{
    IMPORT FaultHandler_c
    TST   LR, #4
    ITE   EQ
    MRSEQ  R0, MSP   ; z=1, LR.2=0   0xFFFFFFF1/0xFFFFFFF9
    MRSNE  R0, PSP   ; z=0, LR.2=1   0xFFFFFFFD
    B   FaultHandler_c
}

#define DUMP_SIZE             256
#define DUMP_REG_SIZE         32
#define DUMP_STACK_ADR        (_DATA_SRAM_BASE_ADR + DATA_SRAM_SIZE - DUMP_SIZE)

void FaultHandler_c(uint32_t * FaultArgs)
{
    uint32_t StackedR0;
    uint32_t StackedR1;
    uint32_t StackedR2;
    uint32_t StackedR3;
    uint32_t StackedR12;
    uint32_t StackedLR;
    uint32_t StackedPC;
    uint32_t StackedPSR;

    StackedR0 = (uint32_t)FaultArgs[0];
    StackedR1 = (uint32_t)FaultArgs[1];
    StackedR2 = (uint32_t)FaultArgs[2];
    StackedR3 = (uint32_t)FaultArgs[3];
    StackedR12 = (uint32_t)FaultArgs[4];
    StackedLR = (uint32_t)FaultArgs[5];
    StackedPC = (uint32_t)FaultArgs[6];
    StackedPSR = (uint32_t)FaultArgs[7];

    // dump common regs and fault regs
    NST_MOVE_MEM((UINT8*)DUMP_STACK_ADR, FaultArgs, DUMP_REG_SIZE);
    NST_MOVE_MEM((UINT8*)(DUMP_STACK_ADR + DUMP_REG_SIZE), (UINT8*)SCB_BASE, sizeof(SCB_TypeDef));
    
    DBGPRINT(DEBUG_ERROR, "FaultException\n");
    DBGPRINT(DEBUG_ERROR, "R0(0x%0X), R1(0x%0X), R2(0x%0X), R3(0x%0X)\n", StackedR0, StackedR1, StackedR2, StackedR3);
    DBGPRINT(DEBUG_ERROR, "R12(0x%0X), LR(0x%0X), PC(0x%08X), PSR(0x%0X)\n", StackedR12, StackedLR, StackedPC, StackedPSR);
    DBGPRINT(DEBUG_ERROR, "BFAR(0x%0X), MMFAR(0x%0X), CFSR(0x%0X), HFSR(0x%0X)\n", 
        NST_SCB->BFAR, NST_SCB->MMFAR, 
        NST_SCB->CFSR, NST_SCB->HFSR);
    DBGPRINT(DEBUG_ERROR, "AFSR(0x%0X), DFSR(0x%0X)\n", 
        NST_SCB->AFSR, NST_SCB->DFSR);

    // clear FSR
    NST_SCB->HFSR = 0;
    NST_SCB->AFSR = 0;
    NST_SCB->DFSR = 0;
}


__irq void HardFaultException(void)
{
    FaultHandler_asm();
    /* Go to infinite loop when Hard Fault exception occurs */
  //  while (1)
    {
    //   DBGPRINT(DEBUG_ERROR, "HardFaultException\n");
    }
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void MemManageException(void)
{
    FaultHandler_asm();
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    //DBGPRINT(DEBUG_ERROR, "MemManageException\n");
    }
}

/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void BusFaultException(void)
{
    FaultHandler_asm();
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    //  DBGPRINT(DEBUG_ERROR, "BusFaultException\n");
    }
}

/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void UsageFaultException(void)
{
    FaultHandler_asm();
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
   // DBGPRINT(DEBUG_ERROR, "BusFaultException\n");
    }
}

/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void DebugMonitor(void)
{
}

/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void SVCHandler(void)
{
}

/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void PendSVC(void)
{
}

/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

__irq void SysTickHandler(void)
{
    OSIntEnter();

    OSTimeTick();      /* Call uC/OS-II's OSTimeTick(),在os_core.c文件里定义,主要判断延时的任务是否计时到*/

    OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换             
}

/*******************************************************************************
* Function Name  : WakeupCpu_IRQHandler
* Description    : This function handles Cpu wake up interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void WakeupCpu_IRQHandler(void)
{
    BSP_WakeupCpuIntISR();
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void EXTI0_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void EXTI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void EXTI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void EXTI3_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External interrupt Line 4 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void EXTI4_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : EXTI5_IRQHandler
* Description    : This function handles External interrupt Line 5 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void EXTI5_IRQHandler(void)
{
   #ifdef GPIO_EXTI_Demo
   OSIntEnter();
   NVIC_DisableIRQ(EXTI5_IRQn); 
   NVIC_ClearIRQChannelPendingBit(EXTI5_IRQn);

   BSP_GPIOIntISR_Demo(EXTI_Line5);

   NVIC_EnableIRQ(EXTI5_IRQn); 
   OSIntExit();
   #endif
}

/*******************************************************************************
* Function Name  : EXTI6_IRQHandler
* Description    : This function handles External interrupt Line 6 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void EXTI6_IRQHandler(void)
{
    OSIntEnter();
    //NVIC_DisableIRQ(EXTI6_IRQn); 
    NVIC_ClearIRQChannelPendingBit(EXTI6_IRQn);

	Simu_UartIRQ_Func();

    //NVIC_EnableIRQ(EXTI6_IRQn); 
	OSIntExit();
}
/*******************************************************************************
* Function Name  : EXTI7_IRQHandler
* Description    : This function handles External interrupt Line 7 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void EXTI7_IRQHandler(void)
{
   #ifdef GPIO_EXTI_Demo

   OSIntEnter();
   NVIC_DisableIRQ(EXTI7_IRQn); 
   NVIC_ClearIRQChannelPendingBit(EXTI7_IRQn);

   BSP_GPIOIntISR_Demo(EXTI_Line7);

   NVIC_EnableIRQ(EXTI7_IRQn); 
   OSIntExit();

   #endif
}
/*******************************************************************************
* Function Name  : TMR1_IRQHandler
* Description    : This function handles timer 1 interrupt.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void TMR1_IRQHandler(void)
{   
    OSIntEnter();
    NVIC_DisableIRQ(TMR1_IRQn); 
    NVIC_ClearIRQChannelPendingBit(TMR1_IRQn);

	TIM_ClearITPendingBit(TIMER1); 
	//ADD UERS CODE...
	//... 
	#if(!defined REAL_UART_USED)
	SimuUartOutPut();//模拟串口发送数据
	#endif //#if(!defined REAL_UART_USED)	    

    NVIC_EnableIRQ(TMR1_IRQn); 
    OSIntExit();
}

/*******************************************************************************
* Function Name  : TMR0_IRQHandler
* Description    : This function handles timer 0 interrupt.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void TMR0_IRQHandler(void)
{
    OSIntEnter();
    NVIC_DisableIRQ(TMR0_IRQn); 
    NVIC_ClearIRQChannelPendingBit(TMR0_IRQn);

	TIM_ClearITPendingBit(TIMER0); 
	//ADD UERS CODE...
	//...
	#if(!defined REAL_UART_USED) 
	simu_uart_timer_task(); 
	#endif //#if(!defined REAL_UART_USED)

    NVIC_EnableIRQ(TMR0_IRQn); 
    OSIntExit();
}

/*******************************************************************************
* Function Name  : UART_IRQHandler
* Description    : This function handles uart interrupt.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void UART_IRQHandler(void)
{
    OSIntEnter();  
    NVIC_DisableIRQ(UART_IRQn);
    NVIC_ClearIRQChannelPendingBit(UART_IRQn);

    BSP_UartISR_Demo();

    NVIC_EnableIRQ(UART_IRQn);
    OSIntExit();
}

/*******************************************************************************
* Function Name  : WWDG0_IRQHandler
* Description    : This function handles WWDG01 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void WWDG0_IRQHandler(void)
{
    uint32_t RegEoi;

    OSIntEnter();
    NVIC_DisableIRQ(WWDG0_IRQn);     
    NVIC_ClearIRQChannelPendingBit(WWDG0_IRQn);

    // clear interrupt
    RegEoi = *Wdt0Eoi;
    RegEoi = RegEoi;

    NVIC_EnableIRQ(WWDG0_IRQn);
    OSIntExit();
}

/*******************************************************************************
* Function Name  : SMID_IRQHandler
* Description    : This function handles host smid interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void SMID_IRQHandler(void)
{
    OSIntEnter();

    BSP_HostIntISR();

    OSIntExit();
}

/*******************************************************************************
* Function Name  : LowMac_IRQHandler
* Description    : This function handles low mac interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void LowMac_IRQHandler(void)
{
    OSIntEnter();

    BSP_LowMacIntISR();

    OSIntExit();
}

/*******************************************************************************
* Function Name  : I2C_IRQHandler
* Description    : This function handles I2C interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void I2C_IRQHandler(void)
{}
__irq void SPI_IRQHandler(void)
{}
__irq void IQCalibration_IRQHandler(void)
{
}
__irq void EXTI15_IRQHandler(void)
{}
__irq void EXTI14_IRQHandler(void)
{}
__irq void EXTI13_IRQHandler(void)
{}
__irq void EXTI12_IRQHandler(void)
{}
__irq void EXTI11_IRQHandler(void)
{}
__irq void EXTI10_IRQHandler(void)
{}
__irq void EXTI9_IRQHandler(void)
{}
__irq void EXTI8_IRQHandler(void)
{}
__irq void DUMMY0_IRQHandler(void)
{}
__irq void DMA_IRQHandler(void)
{
    OSIntEnter();

    BSP_DmaIntISR();

    OSIntExit();
}
__irq void I2S_IRQHandler(void)
{
    OSIntEnter();

#ifdef HW_I2S_SUPPORT
     BSP_I2SIntISR();
#endif

    OSIntExit();
}
__irq void EXTI16_31_IRQnHandler(void)
{

}
__irq void DUMMY1_IRQHandler(void)
{}
__irq void DUMMY2_IRQHandler(void)
{}
__irq void DUMMY3_IRQHandler(void)
{}
__irq void DUMMY4_IRQHandler(void)
{}
__irq void DUMMY5_IRQHandler(void)
{}
__irq void DUMMY6_IRQHandler(void)
{}
__irq void DUMMY7_IRQHandler(void)
{}
__irq void DUMMY8_IRQHandler(void)
{}
__irq void DUMMY9_IRQHandler(void)
{}
__irq void DUMMY10_IRQHandler(void)
{}
__irq void DUMMY11_IRQHandler(void)
{}
__irq void DUMMY12_IRQHandler(void)
{}
__irq void DUMMY13_IRQHandler(void)
{}
__irq void DUMMY14_IRQHandler(void)
{}
__irq void DUMMY15_IRQHandler(void)
{}


/*
******************************************************************************
**                            End Of File                                    **
******************************************************************************
*/
