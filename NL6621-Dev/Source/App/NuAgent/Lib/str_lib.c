/*
 * =====================================================================================
 *
 *       Filename:  str_lib.c
 *
 *    Description:  Process string function
 *
 *        Version:  0.0.1
 *        Created:  2015/1/6 16:31:02
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date          Purpose
 *        0.0.1      Lin Hui    2015/1/6      Initialize
 *
 * =====================================================================================
 */
#include "includes.h"
#include "str_lib.h"


char *strchr(char *s, char c)
{
    while (*s && (*s != c))
        s++;
    if (*s == c)
        return s;

    return NULL;
}   


int strncmp(register const char *s1, register const char *s2, register unsigned int n)   
{   
    if (n) {   
        do {   
            if (*s1 != *s2++)   
                break;   
            if (*s1++ == '\0')   
                return 0;   
        } while (--n > 0);   
        if (n > 0) {   
            if (*s1 == '\0') return -1;   
            if (*--s2 == '\0') return 1;   
            return (unsigned char) *s1 - (unsigned char) *s2;   
        }   
    }   
    return 0;   
}

char* strrmvspace(char *dst, const char *src)
{
    unsigned int i;

    if(*src == 0)
    {
        *dst = 0;
    }
    else
    {
        for (i = 0; isspace(src[i]); i++);
        strcpy(dst, &src[i]);

        i = strlen(dst) - 1;
        for (i = strlen(dst) - 1; isspace(dst[i]); i--)
        {
            dst[i] = 0;
        }
    }

    return dst;
}


char* strtoupper(char *dst, const char *src)
{
    char c;

    while(*src)
    {
        c    = *src++;
        *dst++ = toupper(c);
    }
    *dst = 0;

    return dst;
}

int strncmpi(const char *s1, const char *s2, size_t n)
{
    unsigned char c1, c2;

    if(n == 0)
    {
        return 0;
    }

    do
    {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
        if((c1 == 0) || (c2 == 0))
        {
            break;
        }
    }
    while((c1 == c2) && --n);

    return c1-c2;
}

char* utoa(unsigned val, char *buf, int radix)
{
    char *s, *p;
    s = "0123456789abcdefghijklmnopqrstuvwxyz";

    if(radix == 0)
    {
        radix = 10;
    }
    if(buf == NULL)
    {
        return NULL;
    }
    if(val < (unsigned)radix)
    {
        buf[0] = s[val];
        buf[1] = '\0';
    }
    else
    {
        for(p = utoa(val / ((unsigned)radix), buf, radix); *p; p++);
        utoa(val % ((unsigned)radix), p, radix);
    }
    return buf;
}

char* itoa(int val, char *buf, int radix)
{
    char *p;
    unsigned u;

    p = buf;
    if(radix == 0)
    {
        radix = 10;
    }
    if(buf == NULL)
    {
        return NULL;
    }
    if(val < 0)
    {
        *p++ = '-';
        u = -val;
    }
    else
    {
        u = val;
    }
    utoa(u, p, radix);

    return buf;
}

unsigned int atou_hex(const char *s)
{
    unsigned int value=0;

    if(!s)
    {
        return 0;
    }  

    while(*s)
    {
        if((*s >= '0') && (*s <= '9'))
        {
            value = (value*16) + (*s-'0');
        }
        else if((*s >= 'A') && ( *s <= 'F'))
        {
            value = (value*16) + (*s-'A'+10);
        }
        else if((*s >= 'a') && (*s <= 'f'))
        {
            value = (value*16) + (*s-'a'+10);
        }
        else
        {
            break;
        }
        s++;
    }

    return value;
}

 
