package com.nufront.wifiplay.util;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.mina.core.session.IoSession;
import com.nufront.wifiplay.socket.UdpConnector;


public class ServerCache {
	public static Map<String,IoSession> allUdpSession = new HashMap<String,IoSession>();
	public static Map<String,UdpConnector> allUdpUdpConnector = new HashMap<String,UdpConnector>();
	
	public static Map<String,IoSession> allTcpSession = new HashMap<String,IoSession>();
	
	public static Map<String,RetrySendCommand> devRetrySend  = new HashMap<String,RetrySendCommand>();
	
	/*public static Map<String,List<WebRadioInfo>> webRadioMap = new HashMap<String,List<WebRadioInfo>>();*/
	
}
