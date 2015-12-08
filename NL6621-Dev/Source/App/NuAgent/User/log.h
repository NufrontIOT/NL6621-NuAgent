/*
 * =====================================================================================
 *
 *       Filename:  log.h
 *
 *    Description:  log system for nl6621 
 *
 *        Version:  0.0.1
 *        Created:  2015.02.12 15:41:45
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date         Purpose
 *        0.0.1      Lin Hui    2015.02.12    Create and initialize    
 *        0.0.2      Hui Lou    2015.12.7     Add the baud rate selection 
 *
 * =====================================================================================
 */

#ifndef __LOG_H__
#define __LOG_H__

#define REAL_UART_USED	

#define LOG_ERROR                     	(0x01)
#define LOG_WARNING                    	(0x02)
#define LOG_NOTICE                     	(0x04)
#define LOG_INFO                       	(0x08)
#define LOG_DEBUG                      	(0x10)

/* This log macro must be set befor compiler, output error, warning, info debug lever */
#define DEBUG_LEVEL_SWITCH              (0x1F)


/*
//串口波特率

//347    //波特率115200
//694    //波特率57600
//714    //波特率56000
//1041   //波特率38400
//2083   //波特率19200
//2778   //波特率14400
//4166   //波特率9600
//8333   //波特率4800
*/
#define SIMU_UART_Baudrate	    1041   //波特率38400


#ifdef REAL_UART_USED /* Real Uart */

#if ((DEBUG_LEVEL_SWITCH & LOG_ERROR) == LOG_ERROR)
    #define log_err(fmt, arg...)       printf(fmt, ##arg)
#else
    #define log_err(fmt, arg...)
#endif

#if ((DEBUG_LEVEL_SWITCH & LOG_WARNING) == LOG_WARNING)
    #define log_warn(fmt, arg...)       printf(fmt, ##arg)
#else
    #define log_warn(fmt, arg...)
#endif

#if ((DEBUG_LEVEL_SWITCH & LOG_NOTICE) == LOG_NOTICE)
    #define log_notice(fmt, arg...)       printf(fmt, ##arg)
#else
    #define log_notice(fmt, arg...)
#endif

#if ((DEBUG_LEVEL_SWITCH & LOG_INFO) == LOG_INFO)
    #define log_info(fmt, arg...)       printf(fmt, ##arg)
#else
    #define log_info(fmt, arg...)
#endif

#if ((DEBUG_LEVEL_SWITCH & LOG_DEBUG) == LOG_DEBUG)
    #define log_debug(fmt, arg...)       printf(fmt, ##arg)
#else
    #define log_debug(fmt, arg...)
#endif

#else /* Simu Uart */	

#if ((DEBUG_LEVEL_SWITCH & LOG_ERROR) == LOG_ERROR)
    #define log_err(fmt, arg...)       print(fmt, ##arg)
#else
    #define log_err(fmt, arg...)
#endif

#if ((DEBUG_LEVEL_SWITCH & LOG_WARNING) == LOG_WARNING)
    #define log_warn(fmt, arg...)       print(fmt, ##arg)
#else
    #define log_warn(fmt, arg...)
#endif

#if ((DEBUG_LEVEL_SWITCH & LOG_NOTICE) == LOG_NOTICE)
    #define log_notice(fmt, arg...)       print(fmt, ##arg)
#else
    #define log_notice(fmt, arg...)
#endif

#if ((DEBUG_LEVEL_SWITCH & LOG_INFO) == LOG_INFO)
    #define log_info(fmt, arg...)       print(fmt, ##arg)
#else
    #define log_info(fmt, arg...)
#endif

#if ((DEBUG_LEVEL_SWITCH & LOG_DEBUG) == LOG_DEBUG)
    #define log_debug(fmt, arg...)       print(fmt, ##arg)
#else
    #define log_debug(fmt, arg...)
#endif

#define NUAGENT_UART_SWITCH   		(1)

#if NUAGENT_UART_SWITCH
void    print(char* fmt, ...);
void    printch(char ch);
void    printdec(int dec);
void    printflt(double flt);
void    printbin(int bin);
void    printhex(int hex);
void    printstr(char* str); 
#endif


#endif	 /* REAL_UART_USED end */

void dump_hex(unsigned char *str, unsigned int len);

#endif


