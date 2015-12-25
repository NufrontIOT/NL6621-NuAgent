/*
 * =====================================================================================
 *
 *       Filename:  usart.h
 *
 *    Description:  This file contains all the functions prototypes for the USART 
 *                  firmware library.
 *
 *        Version:  0.0.1
 *        Created:  2015/11/26 11:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date         Purpose
 *        0.0.1      Hui Lou     2015.11.26   Create and initialize    
 *
 * =====================================================================================
 */
							
#ifndef USART_H
#define USART_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_conf.h"

//RX DATA:0x0000
#define RBR_OFFSET                               0x00000000


//TX DATA:0x0000
#define THR_OFFSET                               0x00000000

//BaudRate:0x0000
#define DLL_OFFSET                               0x00000000

//BaudRate:0x0004
#define DLH_OFFSET                               0x00000004

//IQR_ENABLE:0X0004
#define IER_OFFSET                               0x00000004
//#define IER_PTIME_MASK                           0x00000080
#define IER_EDDSI_MASK                           0x00000008
#define IER_ELSI_MASK                            0x00000004
#define IER_ETBEI_MASK                           0x00000002
#define IER_ERBFI_MASK                           0x00000001


//IRQ Error
#define IIR_OFFSET                               0x00000008
#define IIR_FIFO_STATUS_MASK                     0x000000c0
#define IIR_IID_MASK                             0x0000000f


//IQR_ENABLE U0IIR中断标识寄存器。
#define UART_IIR_MDS                             0x00
#define UART_IIR_THRE                            0x02
#define UART_IIR_RDA                             0x04
#define UART_IIR_RLS                             0x06
#define UART_IIR_BUSY                            0x07
#define UART_IIR_CTI                             0x0C

//控制UART0 Rx和Tx的操作。
#define FCR_OFFSET                               0x00000008

//fifo rx  
#define FCR_FIFO_OFF        0X00
#define FCR_FIFO_ON         0X01
#define FCR_RCVR_FIFO_RST   0X02
#define FCR_RCVR_TRIG_1     0X00
#define FCR_RCVR_TRIG_4     0X40
#define FCR_RCVR_TRIG_8     0X80
#define FCR_RCVR_TRIG_14    0XC0
//fifo tx  
#define FCR_XMIT_FIFO_RST   0X04
//fifo dma mode
#define FCR_DMA_MODE        0X08


//配置串口波特率的参数
#define LCR_OFFSET                               0x0000000c



//IRQ Error flag only read
#define LSR_ADDRESS                              0x00000014
#define LSR_OFFSET                               0x00000014
#define LSR_FERR_MSB                             7
#define LSR_FERR_LSB                             7
#define LSR_FERR_MASK                            0x00000080
#define LSR_FERR_GET(x)                          (((x) & LSR_FERR_MASK) >> LSR_FERR_LSB)
#define LSR_FERR_SET(x)                          (((x) << LSR_FERR_LSB) & LSR_FERR_MASK)
#define LSR_TEMT_MSB                             6
#define LSR_TEMT_LSB                             6
#define LSR_TEMT_MASK                            0x00000040
#define LSR_TEMT_GET(x)                          (((x) & LSR_TEMT_MASK) >> LSR_TEMT_LSB)
#define LSR_TEMT_SET(x)                          (((x) << LSR_TEMT_LSB) & LSR_TEMT_MASK)
#define LSR_THRE_MSB                             5
#define LSR_THRE_LSB                             5
#define LSR_THRE_MASK                            0x00000020
#define LSR_THRE_GET(x)                          (((x) & LSR_THRE_MASK) >> LSR_THRE_LSB)
#define LSR_THRE_SET(x)                          (((x) << LSR_THRE_LSB) & LSR_THRE_MASK)
#define LSR_BI_MSB                               4
#define LSR_BI_LSB                               4
#define LSR_BI_MASK                              0x00000010
#define LSR_BI_GET(x)                            (((x) & LSR_BI_MASK) >> LSR_BI_LSB)
#define LSR_BI_SET(x)                            (((x) << LSR_BI_LSB) & LSR_BI_MASK)
#define LSR_FE_MSB                               3
#define LSR_FE_LSB                               3
#define LSR_FE_MASK                              0x00000008
#define LSR_FE_GET(x)                            (((x) & LSR_FE_MASK) >> LSR_FE_LSB)
#define LSR_FE_SET(x)                            (((x) << LSR_FE_LSB) & LSR_FE_MASK)
#define LSR_PE_MSB                               2
#define LSR_PE_LSB                               2
#define LSR_PE_MASK                              0x00000004
#define LSR_PE_GET(x)                            (((x) & LSR_PE_MASK) >> LSR_PE_LSB)
#define LSR_PE_SET(x)                            (((x) << LSR_PE_LSB) & LSR_PE_MASK)
#define LSR_OE_MSB                               1
#define LSR_OE_LSB                               1
#define LSR_OE_MASK                              0x00000002
#define LSR_OE_GET(x)                            (((x) & LSR_OE_MASK) >> LSR_OE_LSB)
#define LSR_OE_SET(x)                            (((x) << LSR_OE_LSB) & LSR_OE_MASK)
#define LSR_DR_MSB                               0
#define LSR_DR_LSB                               0
#define LSR_DR_MASK                              0x00000001
#define LSR_DR_GET(x)                            (((x) & LSR_DR_MASK) >> LSR_DR_LSB)
#define LSR_DR_SET(x)                            (((x) << LSR_DR_LSB) & LSR_DR_MASK)


//modem status 
#define MSR_ADDRESS                              0x00000018
#define MSR_OFFSET                               0x00000018
#define MSR_DCD_MSB                              7
#define MSR_DCD_LSB                              7
#define MSR_DCD_MASK                             0x00000080
#define MSR_DCD_GET(x)                           (((x) & MSR_DCD_MASK) >> MSR_DCD_LSB)
#define MSR_DCD_SET(x)                           (((x) << MSR_DCD_LSB) & MSR_DCD_MASK)
#define MSR_RI_MSB                               6
#define MSR_RI_LSB                               6
#define MSR_RI_MASK                              0x00000040
#define MSR_RI_GET(x)                            (((x) & MSR_RI_MASK) >> MSR_RI_LSB)
#define MSR_RI_SET(x)                            (((x) << MSR_RI_LSB) & MSR_RI_MASK)
#define MSR_DSR_MSB                              5
#define MSR_DSR_LSB                              5
#define MSR_DSR_MASK                             0x00000020
#define MSR_DSR_GET(x)                           (((x) & MSR_DSR_MASK) >> MSR_DSR_LSB)
#define MSR_DSR_SET(x)                           (((x) << MSR_DSR_LSB) & MSR_DSR_MASK)
#define MSR_CTS_MSB                              4
#define MSR_CTS_LSB                              4
#define MSR_CTS_MASK                             0x00000010
#define MSR_CTS_GET(x)                           (((x) & MSR_CTS_MASK) >> MSR_CTS_LSB)
#define MSR_CTS_SET(x)                           (((x) << MSR_CTS_LSB) & MSR_CTS_MASK)
#define MSR_DDCD_MSB                             3
#define MSR_DDCD_LSB                             3
#define MSR_DDCD_MASK                            0x00000008
#define MSR_DDCD_GET(x)                          (((x) & MSR_DDCD_MASK) >> MSR_DDCD_LSB)
#define MSR_DDCD_SET(x)                          (((x) << MSR_DDCD_LSB) & MSR_DDCD_MASK)
#define MSR_TERI_MSB                             2
#define MSR_TERI_LSB                             2
#define MSR_TERI_MASK                            0x00000004
#define MSR_TERI_GET(x)                          (((x) & MSR_TERI_MASK) >> MSR_TERI_LSB)
#define MSR_TERI_SET(x)                          (((x) << MSR_TERI_LSB) & MSR_TERI_MASK)
#define MSR_DDSR_MSB                             1
#define MSR_DDSR_LSB                             1
#define MSR_DDSR_MASK                            0x00000002
#define MSR_DDSR_GET(x)                          (((x) & MSR_DDSR_MASK) >> MSR_DDSR_LSB)
#define MSR_DDSR_SET(x)                          (((x) << MSR_DDSR_LSB) & MSR_DDSR_MASK)
#define MSR_DCTS_MSB                             0
#define MSR_DCTS_LSB                             0
#define MSR_DCTS_MASK                            0x00000001
#define MSR_DCTS_GET(x)                          (((x) & MSR_DCTS_MASK) >> MSR_DCTS_LSB)
#define MSR_DCTS_SET(x)                          (((x) << MSR_DCTS_LSB) & MSR_DCTS_MASK)

#define SCR_ADDRESS                              0x0000001c
#define SCR_OFFSET                               0x0000001c
#define SCR_SCR_MSB                              7
#define SCR_SCR_LSB                              0
#define SCR_SCR_MASK                             0x000000ff
#define SCR_SCR_GET(x)                           (((x) & SCR_SCR_MASK) >> SCR_SCR_LSB)
#define SCR_SCR_SET(x)                           (((x) << SCR_SCR_LSB) & SCR_SCR_MASK)


#define USR_ADDRESS                              0x0000007c
#define USR_OFFSET                               0x0000007c

#define UART_5BIT_MODE   0
#define UART_6BIT_MODE   1
#define UART_7BIT_MODE   2
#define UART_8BIT_MODE   3


#define NST_WR_UART_REG(Value, Adr) \
    {\
        (*(volatile UINT32 *)(_UART_BASE_ADR + Adr)) = (UINT32)(Value);\
    }

#define NST_RD_UART_REG(Adr)  (*(volatile UINT32 *)(_UART_BASE_ADR + Adr))



#ifndef __ASSEMBLER__

typedef struct uart_reg_reg_s {
  volatile unsigned int rbr;
  volatile unsigned int dlh;
  volatile unsigned int iir;
  volatile unsigned int lcr;
  volatile unsigned int mcr;
  volatile unsigned int lsr;
  volatile unsigned int msr;
  volatile unsigned int scr;
  volatile unsigned int srbr;
  unsigned char pad0[4]; /* pad to 0x28 */
  volatile unsigned int siir;
  volatile unsigned int mwr;
  unsigned char pad1[4]; /* pad to 0x34 */
  volatile unsigned int slsr;
  volatile unsigned int smsr;
  volatile unsigned int mrr;
} uart_reg_reg_t;

#endif /* __ASSEMBLER__ */






/** 
  * @brief  USART Init Structure definition  
  */ 
  
typedef struct
{
  uint32_t USART_BaudRate;            /*!< This member configures the USART communication baud rate.
                                           The baud rate is computed using the following formula:
                                            - IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->USART_BaudRate)))
                                            - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */

  uint16_t USART_WordLength;          /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref USART_Word_Length */

  uint16_t USART_StopBits;            /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref USART_Stop_Bits */

  uint16_t USART_Parity;              /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref USART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */
 
  uint16_t USART_FIFOMode;                /*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref USART_Mode */
                   
} USART_InitTypeDef;


/** @defgroup USART_Word_Length 
  * @{
  */ 
  
#define USART_WordLength_5b                  (0)
#define USART_WordLength_6b                  (1)
#define USART_WordLength_7b                  (2)
#define USART_WordLength_8b                  (3)

                                    
#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_5b) || \
                                      ((LENGTH) == USART_WordLength_6b) || \
									  ((LENGTH) == USART_WordLength_7b) || \
                                      ((LENGTH) == USART_WordLength_8b))
/**
  * @}
  */ 

/** @defgroup USART_Stop_Bits 
  * @{
  */ 
  
#define USART_StopBits_1                     (0)
#define USART_StopBits_1_5                   (0x04)
#define USART_StopBits_2                     (0x04)
#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_1_5)||((STOPBITS) == USART_StopBits_2))
/**
  * @}
  */ 

/** @defgroup USART_Parity 
  * @{
  */ 
  
#define USART_Parity_No                      ((uint16_t)0x40)
#define USART_Parity_Even                    ((uint16_t)0x38)
#define USART_Parity_Odd                     ((uint16_t)0x28) 
#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))



/** @defgroup USART_Flags 
  * @{
  */

#define USART_FLAG_RXE                       ((uint16_t)0x00000080)
#define USART_FLAG_LBD                       ((uint16_t)0x00000010)
#define USART_FLAG_TXE                       ((uint16_t)0x00000040)
#define USART_FLAG_TC                        ((uint16_t)0x00000020)
#define USART_FLAG_RXNE                      ((uint16_t)0x00000001)
#define USART_FLAG_ORE                       ((uint16_t)0x00000002)
#define USART_FLAG_FE                        ((uint16_t)0x00000008)
#define USART_FLAG_PE                        ((uint16_t)0x00000004)
#define IS_USART_FLAG(FLAG) (((FLAG) == USART_FLAG_PE) || ((FLAG) == USART_FLAG_TXE) || \
                             ((FLAG) == USART_FLAG_TC) || ((FLAG) == USART_FLAG_RXNE) || \
                              ((FLAG) == USART_FLAG_LBD) || (FLAG) == USART_FLAG_ORE) || \
                              ((FLAG) == USART_FLAG_FE)
                              
#define IS_USART_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0xFC9F) == 0x00) && ((FLAG) != (uint16_t)0x00))
#define IS_USART_PERIPH_FLAG(PERIPH, USART_FLAG) ((((*(uint32_t*)&(PERIPH)) != UART4_BASE) &&\
                                                  ((*(uint32_t*)&(PERIPH)) != UART5_BASE)) \
                                                  || ((USART_FLAG) != USART_FLAG_CTS)) 
#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x0044AA21))
#define IS_USART_ADDRESS(ADDRESS) ((ADDRESS) <= 0xF)
#define IS_USART_DATA(DATA) ((DATA) <= 0x1FF)






/*
* Name:         USART_Init
* Description:  Initializes the USARTx peripheral according to the specified parameters in the USART_InitStruct .
* 
* Parameter:    USART_InitStruct: pointer to a USART_InitTypeDef structure that contains the configuration 
*               information for the specified USART peripheral.	 
*
* Return:       None
* 
* Note:         None
*/
void USART_Init(USART_InitTypeDef* USART_InitStruct);

/*
* Name:         USART_StructInit
* Description:  Fills each USART_InitStruct member with its default value.
* 
* Parameter:    ART_InitStruct: pointer to a USART_InitTypeDef structure
*               which will be initialized.
*
* Return:       None
* 
* Note:         None
*/
void USART_StructInit(USART_InitTypeDef* USART_InitStruct);

/*
* Name:         USART_Cmd
* Description:  Enables or disables the specified USART peripheral.
* 
* Parameter:    NewState: new state of the USARTx peripheral.
*               This parameter can be: ENABLE or DISABLE.
*
* Return:       None
* 
* Note:         None
*/
void USART_Cmd(FunctionalState NewState);

/*
* Name:         USART_ITConfig
* Description:  Enables or disables the specified USART interrupts.
* 
* Parameter:    USART_IT: specifies the USART interrupt sources to be enabled or disabled.
*               This parameter can be one of the following values:
*                    @arg IER_PTIME_MASK:  THRE Interrupt
*                    @arg IER_EDDSI_MASK:  Modem Status Interrupt
*                    @arg IER_ELSI_MASK:   Receiver Line Status Interrupt
*                    @arg IER_ETBEI_MASK:  Transmit Holding Register Empty Interrupt
*                    @arg IER_ERBFI_MASK:  Received Data Available Interrupt and Character Timeout Interrupt
*				NewState: new state of the specified USART1 interrupts.
*
* Return:       None
* 
* Note:         None
*/
void USART_ITConfig(uint16_t USART_IT, FunctionalState NewState);

/*
* Name:         USART_SendData
* Description:  Transmits single data through the USART1 peripheral.
* 
* Parameter:    Data: the data to transmit.	 
*
* Return:       None
* 
* Note:         None
*/
void USART_SendData(uint16_t Data);

/*
* Name:         USART_ReceiveData
* Description:  Returns the most recent received data by the USART1 peripheral.
* 
* Parameter:    None	 
*
* Return:       None.
* 
* Note:         None
*/
uint16_t USART_ReceiveData(void);

/*
* Name:         USART_GetFlagStatus
* Description:  Checks whether the specified USART flag is set or not.
* 
* Parameter:    USART_FLAG:	 specifies the flag to check. 
*			    This parameter can be one of the following values:
*     @arg USART_FLAG_RXE:  Receiver FIFO Error
*     @arg USART_FLAG_LBD:  LIN Break detection flag
*     @arg USART_FLAG_TXE:  Transmit data register empty flag
*     @arg USART_FLAG_TC:   Transmission Complete flag
*     @arg USART_FLAG_RXNE: Receive data register not empty flag
*     @arg USART_FLAG_ORE:  OverRun Error 
*     @arg USART_FLAG_FE:   Framing Error flag
*     @arg USART_FLAG_PE:   Parity Error flag
*
* Return:       None.
* 
* Note:         None
*/
FlagStatus USART_GetFlagStatus(uint16_t USART_FLAG);

/*
* Name:         USART_ClearFlag
* Description:  Clears the USARTx's pending flags.
* 
* Parameter:    None 
*
* Return:       None
* 
* Note:         None
*/
uint32_t USART_ClearFlag(void);

/*
* Name:         USART_GetFlagStatus
* Description:  Checks whether the specified USART interrupt has occurred or not.
* 
* Parameter:    USART_FLAG:	  specifies the USART interrupt source to check.
*			    This parameter can be one of the following values:
*     @arg UART_IIR_MDS:  modem status
*     @arg UART_IIR_THRE: THR empty
*     @arg UART_IIR_RDA:  received data available
*     @arg UART_IIR_RLS:  received data available
*     @arg UART_IIR_BUSY: busy detect
*     @arg UART_IIR_CTI:  character timeout
*
* Return:       None.
* 
* Note:         None
*/
ITStatus USART_GetITStatus(uint16_t USART_IT);





#ifdef __cplusplus
}
#endif

#endif /* USART_H */



