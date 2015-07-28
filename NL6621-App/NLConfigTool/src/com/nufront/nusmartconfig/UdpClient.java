package com.nufront.nusmartconfig;

import java.util.HashMap;
import java.util.Map;

import android.util.Log;

public class UdpClient {

	private static UdpClient instance;

	public static Map<String, UdpConnector> allUdpConnector = new HashMap<String, UdpConnector>();

	private static int broadcast_port = 60001;

	public static String localIp = "";

	public static UdpClient getInstance() {
		if (instance == null) {
			try {
				if (instance == null) {
					instance = new UdpClient();
				}
			} finally {
				;
			}
		}

		return instance;
	}

	/**
	 * 发送信令,在连续发送时用
	 * 
	 * @param buffer
	 *            信令buffer
	 */
	public void sendBroadCastSignaling(final String ipAddr, final byte[] buffer) {

		UdpConnector udpConnector = allUdpConnector.get(ipAddr);
		if (udpConnector == null) {
			udpConnector = new UdpConnector(ipAddr, broadcast_port);
			udpConnector.connect();
			udpConnector.sendBroadCasetBuffer(buffer);
			allUdpConnector.put(ipAddr, udpConnector);
		} else if (udpConnector.getUdpSocket().isConnected()) {
			udpConnector.sendBroadCasetBuffer(buffer);
		} else {
			udpConnector.connect();
			udpConnector.sendBroadCasetBuffer(buffer);
		}

	}
}
