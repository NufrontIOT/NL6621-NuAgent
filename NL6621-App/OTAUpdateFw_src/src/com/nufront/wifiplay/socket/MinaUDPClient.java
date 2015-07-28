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
	 * 扫描所有设备用本类中方法,发现设备后,用UDPCONNECT重新建立连接并维持
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
	 * 发送单个信令,在连续发送时用,不考虑掉线情况下
	 * @param buffer	信令buffer
	 */
	public void sendSignaling(final String ipAddr,final IoBuffer buffer) {
		checkSession.schedule(new Runnable() {
			public void run() {
				Log.w("service signaling,sendSignaling", "发送" + buffer.get(3) + "号信令:"+(buffer.limit()<200?buffer.getHexDump():buffer.limit()));
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
	 * 发送单个信令,发送之前，重连接一次UDP
	 * 会马上建成原会话断开,需要保证信令发送且不管原会话情况下才使用
	 * 使用此方法可能会造成原会话正在返回的响应信息丢失,应慎重使用
	 * @param buffer	信令buffer
	 */
	public void sendSignalingForCurrentIp(final String ipAddr,final IoBuffer buffer) {
		checkSession.schedule(new Runnable() {
			public void run() {
				Log.w("service signaling,ForCurrentIp", "ForCurrentIp发送" + buffer.get(3) + "号信令:"+(buffer.limit()<100?buffer.getHexDump():buffer.limit()));
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
	 * 获取子网编号
	 * @return	子网字符串
	 */
	/*public String getLanNetNumber(Context context) {
		String ip = DeviceInfo.getLocalWifiIpAddress(context);
		Log.w("MinaUDPClient", "My Wifi Ip is：" + ip);
		
		return ip.substring(0, ip.lastIndexOf(".") + 1);
	}
*/
	
	/**
	 * 扫描网段内的255个IP
	 *//*
	public void scanLanDevice(final Context context) {
		
		if(null != udpScanThread) {
			return;
		}
		
		udpScanThread = new Thread(new Runnable() {
			@Override
			public void run() {
				Log.e("MinaUDPClient", "~~~~~~~开始扫描本网段IP地址");

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
					if(!neighbourIpSet.contains(hostIp)){//排除在线设备
						//Log.e("MinaUDPClient", "~~~~~~~开始扫描IP地址:" + hostIp);
						buffer = new SoundPlay0x01Signaling(DeviceInfo.getInstance()
								.getDeviceName()).getSignaling();
						address = new InetSocketAddress(hostIp, udp_port);
						IoConnector connectorTemp = connector;//MINA的connector必须dispose,否则会造成异常
						sendUdpForIp(address, buffer);
						
						connectList.add(connectorTemp);
				
					}
				}
				//必须休眠一点时间,否则dispose后收不到扫描回应包
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
	 * 为指定IP发送UDP信令,仅用于网段扫描
	 * @param address	指定IP地址
	 * @param buffer	信令内容
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
	 * 获得UDP地址
	 * @return the udp_address
	 */
	public static final String getUdp_address() {
		return udp_address;
	}

	/**
	 * 设置UDP地址
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
