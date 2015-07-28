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
 *
 * =====================================================================================
 */

#ifndef __LOG_H__
#define __LOG_H__

#define LOG_ERROR                      (0x01)
#define LOG_WARNING                    (0x02)
#define LOG_NOTICE                     (0x04)
#define LOG_INFO                       (0x08)
#define LOG_DEBUG                      (0x10)

/* This log macro must be set befor compiler, output error, warning, info debug lever */
#define DEBUG_LEVEL_SWITCH             (0x1f)

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

#endif


