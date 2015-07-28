/*
 * =====================================================================================
 *
 *       Filename:  TestMain.c
 *
 *    Description:  NuAgent test main entrance
 *
 *        Version:  0.0.1
 *        Created:  2015/7/27 10:18:05
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date          Purpose
 *        0.0.1      Lin Hui    2015/07/27     Create and Initialize
 * =====================================================================================
 */

#include "TestMain.h"
#include "common.h"

#ifdef JSON_TEST_DEBUG
// ��������JSON���ݰ�  
char text[] = "{\"timestamp\":\"2015-07-19T08:50:11\",\"value\":1}";
char text1[] = "{\"timestamp\":\"2015-07-19T15:24:18\",\"value\":12}";  

int TestJson (void)
{  
    cJSON *json , *json_value , *json_timestamp;  
    // �������ݰ�  
    json = cJSON_Parse(text);  
    if (!json)  
    {  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
    }  
    else  
    {  
        // ��������ֵ  
        json_value = cJSON_GetObjectItem( json , "value");  
        if( json_value->type == cJSON_Number )  
        {  
            // ��valueint�л�ý��  
            printf("value:%d\r\n",json_value->valueint);  
        }  
        // ����ʱ���  
        json_timestamp = cJSON_GetObjectItem( json , "timestamp");  
        if( json_timestamp->type == cJSON_String )  
        {  
            // valuestring�л�ý��  
            printf("%s\r\n",json_timestamp->valuestring);  
        }  
        // �ͷ��ڴ�ռ�  
        cJSON_Delete(json);  
    } 

    json = cJSON_Parse(text1);  
    if (!json)  
    {  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
    }  
    else  
    {  
        // ��������ֵ  
        json_value = cJSON_GetObjectItem( json , "value");  
        if( json_value->type == cJSON_Number )  
        {  
            // ��valueint�л�ý��  
            printf("value:%d\r\n",json_value->valueint);  
        }  
        // ����ʱ���  
        json_timestamp = cJSON_GetObjectItem( json , "timestamp");  
        if( json_timestamp->type == cJSON_String )  
        {  
            // valuestring�л�ý��  
            printf("%s\r\n",json_timestamp->valuestring);  
        }  
        // �ͷ��ڴ�ռ�  
        cJSON_Delete(json);  
    }  	
	 
    return 0;  
} 
#endif


void TestMain(void)
{

#ifdef JSON_TEST_DEBUG
	TestJson();
#endif
	
}




