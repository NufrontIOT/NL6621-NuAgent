/*
 * =====================================================================================
 *
 *       Filename:  weixin.c
 *
 *    Description:  Function of weixin
 *
 *        Version:  0.0.1
 *        Created:  2015/2/4 10:57:58
 *       Revision:  none
 *
 *         Author:  Lin Hui (Link), linhui.568@163.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date          Purpose
 *        0.0.1      Lin Hui    2015/2/4      Create and intialize
 *        0.0.2     zhaoweixian 2015/3/2      fix light on or off fail bug
 *
 * =====================================================================================
 */

#include "../include/common.h"
#include "AES.h"

#define malloc OSMMalloc
#define calloc OSMCalloc
#define free   OSMFree

unsigned char key[16] = {0x0};
unsigned char IV[16]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};	// 初始化向量


int print_hex_data(unsigned char * buf, int data_len)
{
	int ret = 0;
	int i;

	printf("\n");
	for (i = 0; i < data_len; i++) {
		if ((i != 0) && (i % 8 == 0)) printf("\n");
		printf("0x%x\t", buf[i]);
	}
	printf("\n\n");
	return ret;
}


int report_device_data(char *sendbuf, int data_len)
{
    int ret = 0;


	return ret;
}

#define AES_ENCRYPT_MAX_BUFFER	(1024)

int NL6621_AES_Encrypt(const unsigned char *plaintext, 
						unsigned char *ciphertext, 
						unsigned int data_len)
{
	unsigned char *tempbuf = NULL;
	unsigned int templen = data_len;

	if (templen == 0) {
		return -1;

	} else if ((templen % 16) != 0) {
		tempbuf = malloc(AES_ENCRYPT_MAX_BUFFER);
		memset(tempbuf, 0x0, AES_ENCRYPT_MAX_BUFFER);
		memcpy(tempbuf, plaintext, data_len);
		templen = (((templen / 16) + 1) * 16);

		AES_Encrypt(tempbuf, ciphertext, templen, IV);
		
		if (tempbuf != NULL) free(tempbuf);	

	} else {
		AES_Encrypt(plaintext, ciphertext, templen, IV);	
	}
	
	return templen;
}					

int NL6621_AES_Decrypt(unsigned char *plaintext, 
						const unsigned char *ciphertext, 
						unsigned int data_len)
{
	
	AES_Decrypt(plaintext, ciphertext, data_len, IV);
	return 0;
}



void AESThread(void *arg)
{
	unsigned char *sendbuf = "I am linhui, who are you?";
	unsigned char recevbuf[64] = {'\0'};
	unsigned char CipherText[64] = {0x0};
	unsigned int data_len = strlen(sendbuf);
	unsigned int templen = data_len;
	unsigned char *mykey = "nufront123456";
															
	log_notice("Create AES test demo process task thread.\n");
	OSTimeDly(100);
		 
	while (1) {
		OSTimeDly(300);
	
		memcpy(key, '\0', sizeof(key));
		memcpy(key, mykey, strlen(mykey));	
		printf("key:%s\n", key);
				
		AES_Init(key);	

		/* AES Encrypt plain data */
		templen = NL6621_AES_Encrypt(sendbuf, CipherText, data_len);		
		printf("plain data(len:%d):%s.\n", data_len, sendbuf);
		print_hex_data(CipherText, templen);		 
			
		/* AES Decrypt cipher data */		
		NL6621_AES_Decrypt(recevbuf, CipherText, templen);
		print_hex_data(recevbuf, templen);
		log_notice("Decrypt Data(len:%d):%s\n", templen, recevbuf); 
	}
}
