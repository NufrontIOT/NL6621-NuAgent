package com.nufront.util.socket;

import java.io.InputStream;

/**
 * IO����ش���, �������ṩ�Ķ��Ǿ�̬��, ����IO����
 * @author tiant5
 *
 */
public class IOUTIL {
	
	
	public static byte[] inputStream2bytes(InputStream in) throws Exception
	{
		byte[] bufIn = new byte[BUFFER_SIZE];
		int bytesLen = in.available();
		int totalCount = 0;
		//ע��, Ҫ����Socket��ʱ. ��Ȼ�������ȴ�
		while( bytesLen > 0)
		{
			
			try
			{
				bytesLen = in.read(bufIn, totalCount, bytesLen);
				totalCount += bytesLen;
				bytesLen = in.available();
			}catch(Exception e)
			{
				//��ʱ�׳��쳣, ���������ж�read����
				break;
			}
		}
		
		byte[] stores = new byte[totalCount];
		System.arraycopy(bufIn, 0, stores, 0, totalCount);
		return stores;
	}
	// ����һ�ν������ݵĴ�С, ���������,Ĭ��Ϊ1M, ������ò�Ҫ�޸�
	private static int BUFFER_SIZE = 1024 * 1024;

}
