/*==============================================================================
            Copyright (c) 2004-2012 Nufront Incorporated.
            All Rights Reserved.
            Nufront Confidential and Proprietary
==============================================================================*/
package com.nufront.wifiplay.socket;

import java.net.InetSocketAddress;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.future.ConnectFuture;
import org.apache.mina.core.service.IoConnector;
import org.apache.mina.core.service.IoHandler;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.transport.socket.nio.NioSocketConnector;

import android.os.Handler;
import android.util.Log;



import com.nufront.wifiplay.signaling.LoginInSignaling;

/**
 * @author Nufront
 * 
 */
public class MinaTCPClient {
	public static final int FIRMMARE_START = 1;	
	public static final int UPGRADE_IN_PROCESS = 2;
	public static final int FIRMMARE_FAILURE = 3;
	public static final int FIRMMARE_SUCCESS = 4;

	public static String udp_net_address = "192.168.0.";

	private static String udp_address = "192.168.0.101";

	public static String udp_big_net_address = "192.168.";

	private static String tcp_address = "www.mystarway.net";

	//private static String tcp_address = "192.168.7.107";

	private static int tcp_port = 8200;

	private static int udp_port = 7200;

	private IoSession session;

	private static final ScheduledExecutorService checkSession = Executors
			.newSingleThreadScheduledExecutor();
	// .newScheduledThreadPool(2);

	private static final int TIME_OUT = 2 * 000;

	private IoHandler dataHandler;

	private static ConnectFuture future;

	private static ScheduledFuture<?> checkFuture;

	private LoginInSignaling loginIn;

/*	private SoundPlay0x01Signaling x01Signaling;*/

	private IoConnector connector;

	private IoBuffer ioBuffer;

	private static MinaTCPClient instance;

	private final static Lock lock = new ReentrantLock();
	
	private boolean tcpOnLine = false;

	private long lastOnlineTime = 0;
	static Handler handler = null;
	String SeviceIp =null;
	/**
	 * TCP初始化时不进行连接，需要发送是连接，发送完毕是关闭该连接
	 */
	private MinaTCPClient() {
		super();
		// 初始化信令
		Log.w("MinaTCPClient", "开始连接！");

		// 连接服务器
		//connector = new NioSocketConnector();
		//connector.setConnectTimeoutMillis(TIME_OUT);

		//connector.setHandler(new MinaTCPClientHandler());

		//InetSocketAddress address = new InetSocketAddress(MinaTCPClient.getUdp_address(), tcp_port);


		// 绑定解码器，解决断包和粘包问题
		//DefaultIoFilterChainBuilder chain = connector.getFilterChain();


		//Log.w("service signaling", "remote " + address + " connected.");

	}

	/**
	 * 第一次连接TCP方法
	 */
	public void connect(String ip,Handler handler2) {
		SeviceIp =ip;
		//InetSocketAddress address = new InetSocketAddress(MinaUDPClient.getUdp_address(), tcp_port);
		handler = handler2;
		if(future != null && future.isConnected() == true) {
			if(future.getSession() != null) {
				future.getSession().close(true);
			}
		}
		
		if(connector!=null)
			connector.dispose();
		

		// 如果没有连接上服务器，则执行循环连接线程
		new Thread(new Runnable() {
			@Override
			public void run() {
				Log.e("service signaling", "~~~~~~~连接错误");
				
				connector = new NioSocketConnector();
				connector.setConnectTimeoutMillis(TIME_OUT);

				connector.setHandler(new MinaTCPClientHandler());
				
				
				int connectCount = 0;
				InetSocketAddress address = new InetSocketAddress(SeviceIp, tcp_port);
				
				Log.w("service signaling", "try to connect remote." + SeviceIp);
				Log.w("service signaling", "remote " + address + " connected.");
				try {
					future = connector.connect(address);
				} catch (Exception e2) {
					// TODO Auto-generated catch block
					e2.printStackTrace();
					instance =  null;
					
				}
				future.awaitUninterruptibly();
				

				while (!future.isConnected() && connectCount++ < 5) {

					try {

						future = connector.connect(address);
						future.awaitUninterruptibly();
						//reconnect();
						Thread.sleep(200);
					} catch (Exception e) {
						instance =  null;
						e.printStackTrace();

					}
				}
				
				if(!future.isConnected()) {		//连接失败
					Log.w("MinaTcpClient", "TCP连接失败");
					handler.sendEmptyMessage(FIRMMARE_FAILURE);
				} else {						//连接成功
					Log.w("MinaTcpClient", "TCP连接成功");
					session = future.getSession();
					tcpOnLine = true;
				}
				
			}

		}).start();


	}
	
	public void reconnect() {
		Log.w("service signaling", "try to connect remote.");
		InetSocketAddress address = new InetSocketAddress(SeviceIp, tcp_port);
		
		try {
			future = connector.connect(address);
			future.awaitUninterruptibly();
			
			if (!future.isConnected()) {
				Log.w("service signaling", "###Cannot connect to remote.");
				tcpOnLine = false;
				return;
			} else {
				Log.w("service signaling", "remote " + address + " connected.");
				session = future.getSession();
				tcpOnLine = true;
			}
		} catch (Exception e) {
			e.printStackTrace();
			handler.sendEmptyMessage(FIRMMARE_FAILURE);
		}

		
	}

	public static MinaTCPClient getInstance() {
		if (instance == null) {
			lock.lock();
			
			try {
				if (instance == null) {
					instance = new MinaTCPClient();
					
				}
			} finally {
				lock.unlock();
			}
		}

		return instance;
	}

	public void sendSignaling(IoBuffer buffer) {
		ioBuffer = buffer;
		checkSession.schedule(new Runnable() {
			public void run() {
				if(!session.isClosing()) {
					session.write(ioBuffer);
					Log.w("service signaling", "发送" + ioBuffer.get(2) + "号信令");
				} else {
					Log.w("service signaling", "没有连接上服务器，信息发送失败");
				}
			}
		}, 0, TimeUnit.SECONDS);

	}

/*	public String getLanNetNumber() {
		String ip = DeviceInfo.getLocalIpAddress();
		// Log.w("network length:", "" + ip.lastIndexOf("."));

		return ip.substring(0, ip.lastIndexOf(".") + 1);
	}*/


	/**
	 * @return the udp_address
	 */
	public static final String getUdp_address() {
		return udp_address;
	}

	/**
	 * @param udp_address
	 *            the udp_address to set
	 */
	public static final void setUdp_address(String udp_address) {
		MinaTCPClient.udp_address = udp_address;
	}

	/**
	 * @return the tcp_address
	 */
	public static final String getTcp_address() {
		return tcp_address;
	}

	/**
	 * @param tcp_address
	 *            the tcp_address to set
	 */
	public static final void setTcp_address(String tcp_address) {
		MinaTCPClient.tcp_address = tcp_address;
	}

	/**
	 * @return the tcp_port
	 */
	public static final int getTcp_port() {
		return tcp_port;
	}

	/**
	 * @param tcp_port
	 *            the tcp_port to set
	 */
	public static final void setTcp_port(int tcp_port) {
		MinaTCPClient.tcp_port = tcp_port;
	}

	/**
	 * @return the udp_port
	 */
	public static final int getUdp_port() {
		return udp_port;
	}

	/**
	 * @param udp_port
	 *            the udp_port to set
	 */
	public static final void setUdp_port(int udp_port) {
		MinaTCPClient.udp_port = udp_port;
	}

	/**
	 * @return the lastOnlineTime
	 */
	public final long getLastOnlineTime() {
		return lastOnlineTime;
	}

	/**
	 * @param lastOnlineTime
	 *            the lastOnlineTime to set
	 */
	public final void setLastOnlineTime(long lastOnlineTime) {
		this.lastOnlineTime = lastOnlineTime;
	}

	/**
	 * @return the tcpOnLine
	 */
	public final boolean isTcpOnLine() {
		return tcpOnLine;
	}

	/**
	 * @param tcpOnLine the tcpOnLine to set
	 */
	public final void setTcpOnLine(boolean tcpOnLine) {
		this.tcpOnLine = tcpOnLine;
	}

}
