package com.nufront.wifiplay.socket;


import java.net.InetSocketAddress;

import org.apache.mina.core.future.ConnectFuture;
import org.apache.mina.core.service.IoConnector;
import org.apache.mina.core.service.IoHandler;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.transport.socket.nio.NioSocketConnector;

import android.util.Log;

import com.nufront.wifiplay.util.ServerCache;

public class TcpConnector {
	private static final int TIME_OUT = 2 * 000;

	private static ConnectFuture future;

	private int port;

	private String host;

	private IoHandler dataHandler = new MinaTCPClientHandler();
	
	private IoSession session;
	
	public TcpConnector(String host,int port){
		this.host=host;
		this.port=port;
		run();
	}
	
	public void run() {
		Log.i("","Connector starting...");
		InetSocketAddress address = new InetSocketAddress(host, port);
		IoConnector connector = new NioSocketConnector();
		connector.setConnectTimeoutMillis(TIME_OUT);

		connector.setHandler(dataHandler);
		/*connector.getFilterChain().addLast("codec",
				new ProtocolCodecFilter(codecFactory));*/

		connect(address, connector);
	}


	private void connect(final InetSocketAddress address, final IoConnector connector) {
		Log.i("","try to connect remote.");
		try {
			future = connector.connect(address);
			future.awaitUninterruptibly();
		} catch (Exception e2) {
			e2.printStackTrace();
		}
		
		int connectCount = 0;
		while (!future.isConnected() && connectCount++ < 5) {

			try {
				future = connector.connect(address);
				future.awaitUninterruptibly();
				Thread.sleep(200);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		
		if(!future.isConnected()) {		//连接失败
			Log.w("MinaTcpClient", "TCP连接失败");
		} else {						//连接成功
			Log.w("MinaTcpClient", "TCP连接成功");
			ServerCache.allTcpSession.put(host, future.getSession());//
		}
		
		if (future.isConnected()) 
			Log.i("","remote " + address + " connected.");
	}

	/**
	 * @return the dataHandler
	 */
	public IoHandler getDataHandler() {
		return dataHandler;
	}

	/**
	 * @param dataHandler
	 *            the dataHandler to set
	 */
	public void setDataHandler(IoHandler dataHandler) {
		this.dataHandler = dataHandler;
	}

	/**
	 * @return the host
	 */
	public String getHost() {
		return host;
	}

	/**
	 * @param host
	 *            the host to set
	 */
	public void setHost(String host) {
		this.host = host;
	}

	/**
	 * @return the port
	 */
	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}
	
	public IoSession getSession() {
		return session;
	}

	public void setSession(IoSession session) {
		this.session = session;
	}


}
