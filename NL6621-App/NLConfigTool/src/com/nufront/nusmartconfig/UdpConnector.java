package com.nufront.nusmartconfig;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketException;
import java.nio.ByteBuffer;

import android.util.Log;

public class UdpConnector {

	private InetSocketAddress address;

	private int port;

	private String host;

	private DatagramSocket udpSocket;

	private byte[] receiveBuffer = new byte[1024];

	private DatagramPacket receivePacket = new DatagramPacket(receiveBuffer,
			1024);

	public static boolean wanThreadFlag = false;
	private Thread currentPlayThread = null;

	private String tag = "UdpConnector";

	public UdpConnector(String host, int port) {
		this.host = host;
		this.port = port;
		Log.i(tag, "host=" + host + " port=" + String.valueOf(port));
		this.address = new InetSocketAddress(host, port);
		try {
			udpSocket = new DatagramSocket();
			/*
			 * if(host.trim().equals(UdpClient.wlan_address))
			 * udpSocket.setSoTimeout(4000); else
			 */udpSocket.setSoTimeout(200);
		} catch (SocketException e) {
			e.printStackTrace();
		}
	}

	public UdpConnector(InetSocketAddress address) {
		this.address = address;
		this.host = address.getHostName();
		this.port = address.getPort();
		try {
			udpSocket = new DatagramSocket();
			/*
			 * if(host.trim().equals(UdpClient.wlan_address))
			 * udpSocket.setSoTimeout(4000); else
			 */udpSocket.setSoTimeout(200);
		} catch (SocketException e) {
			e.printStackTrace();
		}
	}

	public void connect() {
		try {
			udpSocket.connect(address);
		} catch (SocketException e) {
			e.printStackTrace();
		}
		// if(!wanThreadFlag&&host!=null )
		// wanReceiver();

	}

	public void sendBroadCasetBuffer(byte[] sendBuffer) {
		if (isConnected()) {
			/*
			 * int length = buffer.remaining(); byte[] sendBuffer = new
			 * byte[length]; buffer.get(sendBuffer);
			 */
			// buffer.position(0);

			// Log.i(tag,"send remaining len:" +
			// buffer.remaining()+",content: "+strBuff);
			DatagramPacket sendPacket = new DatagramPacket(sendBuffer,
					sendBuffer.length);

			try {
				udpSocket.send(sendPacket);
				/*
				 * if(sendBuffer[3]!=(byte)0x01)
				 * Log.i(tag,"udpSocket send ip :"+
				 * sendPacket.getAddress().toString()
				 * +", port: "+sendPacket.getPort
				 * ()+",bytes:"+strBuff.toString());
				 */
			} catch (IOException e) {
				e.printStackTrace();
			}

		}
	}

	public void wanReceiver() {
		if (!wanThreadFlag && host != null) {
			wanThreadFlag = true;
		} else
			return;

		Log.i(tag, "wanReceiver start.");
		currentPlayThread = new Thread(new Runnable() {
			@Override
			public void run() {
				while (true) {
					try {
						udpSocket.receive(receivePacket);

						ByteBuffer receive = ByteBuffer.wrap(receiveBuffer);
						/*
						 * LedSignalingHandler.getInstance().signalingHandle(receive
						 * , receiveBuffer.length, udpSocket);
						 */
						Log.i(tag, "wanReceiver:" + receive.limit());
					} catch (IOException e) {
						// e.printStackTrace();
						// Log.i(tag,"wanReceiver Exception:"+e.toString());
					} catch (Exception e) {
						Log.i(tag, "wanReceiver Exception:" + e.toString());
						try {
							Thread.sleep(500);
						} catch (InterruptedException e1) {
							e1.printStackTrace();
						}
					}

				}
			}
		});
		currentPlayThread.start();
	}

	public synchronized void reconnectAndSend(byte[] buffer) {
		connect();
		sendBroadCasetBuffer(buffer);
	}

	private boolean isConnected() {
		return udpSocket.isConnected();
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

	public DatagramSocket getUdpSocket() {
		return udpSocket;
	}

	public void setUdpSocket(DatagramSocket udp) {
		this.udpSocket = udp;
	}

}
