package com.nufront.wifiplay.socket;

import java.net.InetSocketAddress;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.future.ConnectFuture;
import org.apache.mina.core.service.IoConnector;
import org.apache.mina.core.service.IoHandler;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.transport.socket.nio.NioDatagramConnector;

import android.util.Log;

public class UdpConnector {
	private ConnectFuture future;
	
	private IoConnector connector;
	
	private InetSocketAddress address;

	private int port;

	private String host;

	private IoHandler dataHandler = new MinaUDPClientHandler();
	
	private IoSession session;

	//private ProtocolCodecFactory codecFactory;

	public UdpConnector(String host,int port){
		this.host=host;
		this.port=port;
		address = new InetSocketAddress(host, port);
		/*connector = new NioDatagramConnector();
		connector.setHandler(dataHandler);*/
		//connector.setConnectTimeoutMillis(1000);
		/*connector.getFilterChain().addLast("codec",
				new ProtocolCodecFilter(codecFactory));*/

		connect();
	}
	public UdpConnector(String host,int port,IoBuffer buffer){
		this(host,port);
		this.session.write(buffer);
	}


	public void connect() {
		Log.i("UdpConnector","try to connect remote." + host + "/" + port);
		if(connector!=null)
			connector.dispose();
		connector = new NioDatagramConnector();
		connector.setHandler(dataHandler);
		
		future = connector.connect(address);
		future.awaitUninterruptibly();
		
		if (!future.isConnected()) {
			//Log.e("UdpConnector","Cannot connect to remote.");
		}else{
			Log.i("UdpConnector","remote " + address + " connected.");
			this.session = future.getSession();
			//ServerCache.allDevSession.put(host, future.getSession());//收到回应才能加入
		}
	}
	
	public void sendBuffer(IoBuffer buffer){
		if (future.isConnected()) {
			Log.i("UdpConnector","send len:" + buffer.remaining());
			future.getSession().write(buffer);
		}
	}
	
	public synchronized void reconnectAndSend(IoBuffer buffer){
		connect();
		sendBuffer(buffer);
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

	/**
	 * @param port
	 *            the port to set
	 */
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
