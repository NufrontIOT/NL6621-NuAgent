package com.nufront.util.socket;

import java.io.IOException;
import java.net.Socket;


/**
 * @author tiant5
 * ע��ÿ��������Task��Ҫʵ��run����, run������, ��ͬ�������ҵ��, �м���Ҫʵ��ͬ��
 * 
 */
public abstract class SingleTask implements Runnable{
	
	
	protected Socket serverClientSocket;
	
	protected long lastValidTime;
	
	public void setSocket(Socket serverClientSocket)
	{
		this.serverClientSocket = serverClientSocket;
		lastValidTime = System.currentTimeMillis();
	}
	
	public boolean isAlive()
	{
		if (serverClientSocket.isClosed())
		{
			return false;
		}
		
		long timeInterval = System.currentTimeMillis()- lastValidTime;
		
		if (SanySocketServer.TIME_OUT < timeInterval)
		{
			return false;
		}
		
		return true;
	}
	
	
	

	public void close()
	{
		try {
			serverClientSocket.close();
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
	

}
