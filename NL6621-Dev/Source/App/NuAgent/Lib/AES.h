///////////////////////////////////////////////////////////////////////////////
// �� �� ����AES.h
// ��    ����AES�����㷨
// �� �� �ˣ�Liangbofu
// �������ڣ�2009-07-17
///////////////////////////////////////////////////////////////////////////////
#ifndef __AES_H
#define __AES_H

#ifdef __cplusplus
	extern "C" {
#endif

// ��bitΪ��λ����Կ���ȣ�ֻ��Ϊ 128��192 �� 256 ����
#define AES_KEY_LENGTH	128

// �ӽ���ģʽ
#define AES_MODE_ECB	0				// �������뱾ģʽ��һ��ģʽ��
#define AES_MODE_CBC	1				// �����������ģʽ
#define AES_MODE		AES_MODE_CBC


///////////////////////////////////////////////////////////////////////////////
//	��������	AES_Init
//	������		��ʼ�����ڴ�ִ����չ��Կ������
//	���������	pKey -- ԭʼ��Կ���䳤�ȱ���Ϊ AES_KEY_LENGTH/8 �ֽڡ�
//	���������	�ޡ�
//	����ֵ��	�ޡ�
///////////////////////////////////////////////////////////////////////////////
void AES_Init(const void *pKey);

//////////////////////////////////////////////////////////////////////////
//	��������	AES_Encrypt
//	������		��������
//	���������	pPlainText	-- ���ģ�������ܵ����ݣ��䳤��ΪnDataLen�ֽڡ�
//				nDataLen	-- ���ݳ��ȣ����ֽ�Ϊ��λ������ΪAES_KEY_LENGTH/8����������
//				pIV			-- ��ʼ�����������ʹ��ECBģʽ������ΪNULL��
//	���������	pCipherText	-- ���ģ��������ļ��ܺ�����ݣ�������pPlainText��ͬ��
//	����ֵ��	�ޡ�
//////////////////////////////////////////////////////////////////////////
void AES_Encrypt(const unsigned char *pPlainText, unsigned char *pCipherText, 
				 unsigned int nDataLen, const unsigned char *pIV);

//////////////////////////////////////////////////////////////////////////
//	��������	AES_Decrypt
//	������		��������
//	���������	pCipherText -- ���ģ�������ܵ����ݣ��䳤��ΪnDataLen�ֽڡ�
//				nDataLen	-- ���ݳ��ȣ����ֽ�Ϊ��λ������ΪAES_KEY_LENGTH/8����������
//				pIV			-- ��ʼ�����������ʹ��ECBģʽ������ΪNULL��
//	���������	pPlainText  -- ���ģ��������Ľ��ܺ�����ݣ�������pCipherText��ͬ��
//	����ֵ��	�ޡ�
//////////////////////////////////////////////////////////////////////////
void AES_Decrypt(unsigned char *pPlainText, const unsigned char *pCipherText, 
				 unsigned int nDataLen, const unsigned char *pIV);


#ifdef __cplusplus
	}
#endif


#endif	// __AES_H
