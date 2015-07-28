package com.nufront.wifiplay.socket;

import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.future.ConnectFuture;
import org.apache.mina.core.service.IoConnector;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.transport.socket.nio.NioDatagramConnector;

import com.nufront.wifiplay.util.ServerCache;

import android.content.Context;
import android.util.Log;





/**
 * @author Nufront
 * 
 */
public class MinaUDPClient {
	/**
	 * ɨ�������豸�ñ����з���,�����豸��,��UDPCONNECT���½������Ӳ�ά��
	 */

	public static String udp_net_address = "192.168.0.";
	
	private static String udp_address = "192.168.0.1";
	
	public static String udp_big_net_address = "192.168.";
	
	private static int udp_port = 7200;

	private static final ScheduledExecutorService checkSession = Executors
			.newSingleThreadScheduledExecutor();
	
	//private static final int TIME_OUT = 5 * 000;

	private static ConnectFuture future;

	private IoConnector connector;
	
	private static List<IoConnector> connectList = new ArrayList<IoConnector>();
	
	private static MinaUDPClient instance;

	private final static Lock lock = new ReentrantLock();

	private long lastOnlineTime = 0;
	
	private Thread udpScanThread;

	


	public static MinaUDPClient getInstance() {
		if (instance == null) {
			lock.lock();
			try {
				if (instance == null) {
					instance = new MinaUDPClient();
				}
			} finally {
				lock.unlock();
			}
		}

		return instance;
	}

	/**
	 * ���͵�������,����������ʱ��,�����ǵ��������
	 * @param buffer	����buffer
	 */
	public void sendSignaling(final String ipAddr,final IoBuffer buffer) {
		checkSession.schedule(new Runnable() {
			public void run() {
				Log.w("service signaling,sendSignaling", "����" + buffer.get(3) + "������:"+(buffer.limit()<200?buffer.getHexDump():buffer.limit()));
				IoSession session = ServerCache.allUdpSession.get(ipAddr);
				if(session!=null&&session.isConnected()){
					session.write(buffer);
				}else{
					UdpConnector udpConnector = ServerCache.allUdpUdpConnector.get(ipAddr);
					if(udpConnector==null){
						udpConnector = new UdpConnector(ipAddr,udp_port,buffer);
						ServerCache.allUdpUdpConnector.put(ipAddr, udpConnector);
					}else{
						udpConnector.connect();
						udpConnector.sendBuffer(buffer);
					}
				}
			}
		}, 0, TimeUnit.SECONDS);

	}

	
	/**
	 * ���͵�������,����֮ǰ��������һ��UDP
	 * �����Ͻ���ԭ�Ự�Ͽ�,��Ҫ��֤������Ҳ���ԭ�Ự����²�ʹ��
	 * ʹ�ô˷������ܻ����ԭ�Ự���ڷ��ص���Ӧ��Ϣ��ʧ,Ӧ����ʹ��
	 * @param buffer	����buffer
	 */
	public void sendSignalingForCurrentIp(final String ipAddr,final IoBuffer buffer) {
		checkSession.schedule(new Runnable() {
			public void run() {
				Log.w("service signaling,ForCurrentIp", "ForCurrentIp����" + buffer.get(3) + "������:"+(buffer.limit()<100?buffer.getHexDump():buffer.limit()));
				UdpConnector udpConnector = ServerCache.allUdpUdpConnector.get(ipAddr);
				if(udpConnector==null){
					udpConnector = new UdpConnector(ipAddr,udp_port,buffer);
					ServerCache.allUdpUdpConnector.put(ipAddr, udpConnector);
				}else{
					udpConnector.connect();
					udpConnector.sendBuffer(buffer);
				}
				
			}
		}, 0, TimeUnit.SECONDS);

	}

	/**
	 * ��ȡ�������
	 * @return	�����ַ���
	 */
	/*public String getLanNetNumber(Context context) {
		String ip = DeviceInfo.getLocalWifiIpAddress(context);
		Log.w("MinaUDPClient", "My Wifi Ip is��" + ip);
		
		return ip.substring(0, ip.lastIndexOf(".") + 1);
	}
*/
	
	/**
	 * ɨ�������ڵ�255��IP
	 *//*
	public void scanLanDevice(final Context context) {
		
		if(null != udpScanThread) {
			return;
		}
		
		udpScanThread = new Thread(new Runnable() {
			@Override
			public void run() {
				Log.e("MinaUDPClient", "~~~~~~~��ʼɨ�豾����IP��ַ");

				if(connector!=null)
					connector.dispose();
				connector = new NioDatagramConnector();
				connector.setHandler(new MinaUDPClientHandler());
				
				String netAddress = getLanNetNumber(context);
				String hostIp;
				InetSocketAddress address;
				IoBuffer buffer = new SoundPlay0x01Signaling(DeviceInfo.getInstance()
								.getDeviceName()).getSignaling();
				Set<String> neighbourIpSet = NeighbourHandler.getInstance().getNeighbourMap().keySet();
				
				for(int i = 1; i < 255; i++) {
					hostIp = netAddress + i;
					if(!neighbourIpSet.contains(hostIp)){//�ų������豸
						//Log.e("MinaUDPClient", "~~~~~~~��ʼɨ��IP��ַ:" + hostIp);
						buffer = new SoundPlay0x01Signaling(DeviceInfo.getInstance()
								.getDeviceName()).getSignaling();
						address = new InetSocketAddress(hostIp, udp_port);
						IoConnector connectorTemp = connector;//MINA��connector����dispose,���������쳣
						sendUdpForIp(address, buffer);
						
						connectList.add(connectorTemp);
				
					}
				}
				//��������һ��ʱ��,����dispose���ղ���ɨ���Ӧ��
				try {
					Thread.sleep(200);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				while(connectList.size()>0){
					connectList.remove(0).dispose();
				}
				udpScanThread = null;
			}
		});
		udpScanThread.start();
	}*/

	
	/**
	 * Ϊָ��IP����UDP����,����������ɨ��
	 * @param address	ָ��IP��ַ
	 * @param buffer	��������
	 */
	private void sendUdpForIp(InetSocketAddress address, IoBuffer buffer) {
		try {
			future = connector.connect(address);
			future.awaitUninterruptibly();
			
			IoSession session = future.getSession();
			session.write(buffer);
		} catch (Exception e2) {
			e2.printStackTrace();
		}
	
	}
	/**
	 * ���UDP��ַ
	 * @return the udp_address
	 */
	public static final String getUdp_address() {
		return udp_address;
	}

	/**
	 * ����UDP��ַ
	 * @param udp_address the udp_address to set
	 */
	public static final void setUdp_address(String udp_address) {
		MinaUDPClient.udp_address = udp_address;
	}



	/**
	 * @return the udp_port
	 */
	public static final int getUdp_port() {
		return udp_port;
	}

	/**
	 * @param udp_port the udp_port to set
	 */
	public static final void setUdp_port(int udp_port) {
		MinaUDPClient.udp_port = udp_port;
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

}
