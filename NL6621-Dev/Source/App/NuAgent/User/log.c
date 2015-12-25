/*
 * =====================================================================================
 *
 *       Filename:  log.c
 *
 *    Description:  nl6621 log system interface
 *
 *        Version:  0.0.1
 *        Created:  2015/7/21 13:30:44
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date          Purpose
 *        0.0.1      Lin Hui    2015/7/21    
 *        0.0.2		 Lin Hui    2015/12/5      Add simulator uart used hard timer1.  
 *        0.0.3		 Hui Lou    2015/12/7      Optimize code and repair bug.  
 *
 * =====================================================================================
 */
#include "includes.h"
#include "log.h"
#include "ring_buffer.h"


#if(!defined REAL_UART_USED)  

extern void SimuSendOneByte(unsigned char ch);

void printch(char ch)
{
    SimuSendOneByte(ch);
}

void printdec(int dec)
{
    if (dec==0)
    {
        return;
    }
    printdec(dec/10);
    printch((char)(dec%10 + '0'));
}

void printflt(double flt)
{
    int tmpint = 0;

    tmpint = (int)flt;
    printdec(tmpint);
    printch('.');
    flt = flt - tmpint;
    tmpint = (int)(flt * 1000000);
    printdec(tmpint);
}

void printstr(char* str)
{
    while(*str)
    {
        printch(*str++);
    }
}

void printbin(int bin)
{
    if(bin == 0)
    {
        printstr("0b");
        return;
    }
    printbin(bin/2);
    printch( (char)(bin%2 + '0'));
}

void printhex(int hex)
{
    if(hex==0)
    {
        //printstr("0x");
        return;
    }
    printhex(hex/16);

    printch((char)(hex%16 < 10)? hex%16+'0':(hex%16-10)+'A');
}

void print(char* fmt, ...)
{
    double vargflt = 0;
    int  vargint = 0;
    char* vargpch = NULL;
    char vargch = 0;
    char* pfmt = NULL;
    va_list vp;

    va_start(vp, fmt);
    pfmt = fmt;

    while(*pfmt)
    {
        if(*pfmt == '%')
        {
            switch(*(++pfmt))
            {                
                case 'c':
                    vargch = va_arg(vp, int); 
                    /* va_arg(ap, type), if type is narrow type (char, short, float) 
                     * an error is given in strict ANSI mode, or a warning otherwise.
                     * In non-strict ANSI mode, 'type' is allowed to be any expression. */
                    printch(vargch);
                    break;
                case 'd':
                case 'i':
                    vargint = va_arg(vp, int);
                    printdec(vargint);
                    break;
                case 'f':
                    vargflt = va_arg(vp, double);
                    /* va_arg(ap, type), if type is narrow type (char, short, float) 
                     * an error is given in strict ANSI mode, or a warning otherwise.
                     * In non-strict ANSI mode, 'type' is allowed to be any expression. */
                    printflt(vargflt);
                    break;
                case 's':
                    vargpch = va_arg(vp, char*);
                    printstr(vargpch);
                    break;
                case 'b':
                case 'B':
                    vargint = va_arg(vp, int);
                    printbin(vargint);
                    break;
                case 'x':
                case 'X':
                    vargint = va_arg(vp, int);
                    printhex(vargint);
                    break;
                case '%':
                    printch('%');
                    break;
                default:
                    break;
            }
            pfmt++;
        }
        else
        {
            printch(*pfmt++);
        }
    }
    va_end(vp);

	//¿ªÆôÖÐ¶Ï
    vargint = *Tmr1Eoi;
    vargint = *TmrsEoi;
	NVIC_EnableIRQ(TMR1_IRQn);
	*Tmr1Load = SIMU_UART_Baudrate;
	*Tmr1Ctl = (~TMR_INT_MASK) |TMR_ENA | TMR_USER_DEFINE_MODE;
}

#endif	//#if(!defined REAL_UART_USED)

void dump_hex(unsigned char *str, unsigned int len)
{
    int x = 0;

    for (x = 0; x < len; x++) {
        if (((x % 8) == 0) && (x != 0))
            printf("\n");
        printf("0x%02x\t", ((unsigned char)str[x]));
    }
    printf("\n");
}


