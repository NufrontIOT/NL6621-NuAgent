package com.nufront.util.socket;

import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * ����һ��ServerSocket. ע�����ServerSocket����ַ�������Ϣ.
 * ��: ������յ�һ����Ϣ, �����������߳�
 */

public class SanySocketServer {


	private ServerSocket serverSocket;
	
	private Class<?> singleTaskCls;
	
	private ThreadPoolExecutor threadPool;
	
	private int maxClientCon;
	
	private List<SingleTask> activeClients;
	
	public static final long TIME_OUT = 30 * 60 * 1000; 


	/**
	 * ����˳�ʼ��
	 * @param serverPort ����˿�
	 * @param threadSize ������ӵĿͻ��˸���
	 * @param taskClass  Ӧ�Ե����ͻ��˵���, ÿ���ͻ��˴����Լ��Ĺ���
	 * @throws Exception
	 */
	public SanySocketServer(int serverPort, int threadSize, Class<?> taskClass) throws Exception {
		

		serverSocket = new ServerSocket(serverPort);
		
		
		ArrayBlockingQueue<Runnable> queue = new ArrayBlockingQueue<Runnable>(
	            5);
		
		//�����̵߳Ĵ�СΪthreadSize, ����sizeΪthreadSize, ÿ�����Ӵ����ʱ�䲻�ܳ���5s
		//ԭ����, ����5S, �ͻ��˵���������Ѿ��ж���
		threadPool = new ThreadPoolExecutor(threadSize, threadSize + 5,
                5, TimeUnit.SECONDS, queue);
		
		singleTaskCls = taskClass;
		
		maxClientCon = threadSize;
		
		activeClients = new ArrayList<SingleTask>();
	}
	

	public void process() throws Exception {
		
		while (true) {
			
			checkValidClient();
			
			int size = activeClients.size();
			
			if (size > maxClientCon)
			{

				Thread.sleep(60000);
				continue;
			}
			
			System.out.println("��ǰ�������ӵĸ���Ϊ:" + size);
			Socket client = serverSocket.accept();
			SingleTask task = (SingleTask)singleTaskCls.newInstance();
			task.setSocket(client);
			activeClients.add(task);
			threadPool.execute(task);
	
		}
	}
	

	private void checkValidClient()
	{
		for (int i = 0; i < activeClients.size(); i++)
		{
			SingleTask client = activeClients.get(i);
			if (!client.isAlive())
			{
				client.close();
				activeClients.remove(client);
			}
		}
	}

}
