/*
******************************************************************************
**
** Project     : 
** Description :    entry of firware
** Author      :    pchn                             
** Date        : 
**    
** UpdatedBy   : 
** UpdatedDate : 
**
******************************************************************************
*/

/*
******************************************************************************
**                               Include Files
******************************************************************************
*/

#include "includes.h"


#pragma arm section zidata="main_only_cpu_access_mem"    /* ��CPU����*/
#ifdef LWIP_SUPPORT
static OS_STK   sAppStartTaskStack[NST_APP_START_TASK_STK_SIZE];  
#endif
#pragma arm section

/*
******************************************************************************
**                               FUNCTION DELEARATIONS
******************************************************************************
*/
extern VOID SystemInit(VOID);
extern void nl6621_main_entry(void * pParam);

/*
******************************************************************************
**                               FUNCTION PROTOTYPES
******************************************************************************
*/
#include "simu_uart.h"


void NuAgent_bsp_init(void)
{
    uart_init(); //ģ�⴮�ڳ�ʼ����	 
}



/*
******************************************************************************
**                        INT32 main(VOID)
**
** Description  : Application entry point
** Arguments    : ��
** Returns      : ��
** Author       :                                   
** Date         : 
**
******************************************************************************
*/	

INT32 main(VOID)
{
    /* system Init */
    SystemInit();

	/* nuagent bsp init */
	NuAgent_bsp_init();

   /* create application tasks here */
    OSTaskCreate(nl6621_main_entry, NULL, &sAppStartTaskStack[NST_APP_START_TASK_STK_SIZE -1],  NST_APP_TASK_START_PRIO); 

    /* Start Os */
    OSStart();
    
    return 1;
}


/*
******************************************************************************
**                            End Of File                                   **
******************************************************************************
*/


