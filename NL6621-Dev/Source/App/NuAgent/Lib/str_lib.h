/*
 * =====================================================================================
 *
 *       Filename:  str_lib.h
 *
 *    Description:  Lib of string 
 *
 *        Version:  0.0.1
 *        Created:  2015/6/19 16:01:42
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date          Purpose
 *        0.0.1      Lin Hui    2015/6/19      
 *
 * =====================================================================================
 */

#ifndef  __STR_LIB_H__
#define  __STR_LIB_H__

char *strchr(char *s, char c);

int strncmp(register const char *s1, register const char *s2, register unsigned int n);
char* strrmvspace(char *dst, const char *src);
char* strtoupper(char *dst, const char *src);
int strncmpi(const char *s1, const char *s2, size_t n);
char* utoa(unsigned val, char *buf, int radix);
char* itoa(int val, char *buf, int radix);
unsigned int atou_hex(const char *s);

#endif   /* ----- #ifndef __STR_LIB_H__  ----- */
