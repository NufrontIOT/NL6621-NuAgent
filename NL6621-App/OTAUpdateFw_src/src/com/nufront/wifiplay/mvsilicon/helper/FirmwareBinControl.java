package com.nufront.wifiplay.mvsilicon.helper;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.Timer;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.mina.core.buffer.IoBuffer;

import android.util.Log;
import android.widget.Toast;
import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.provider.MediaStore.Files;

import com.example.firmwareupgrade.ui.MainActivity;
import com.nufront.thread.UdpSendTimer;
import com.nufront.util.socket.UdpClientSocket;
import com.nufront.wifiplay.signaling.FirmwareBinDataSignaling;
import com.nufront.wifiplay.signaling.FirmwareUpgradeSignaling;
import com.nufront.wifiplay.signaling.SoundPlaySignalingHandler;
import com.nufront.wifiplay.socket.MinaTCPClient;
import com.nufront.wifiplay.socket.MinaUDPClient;

public class FirmwareBinControl {
	CalTime calTime = null;
	public static boolean lock2 = false;
	private static FirmwareBinControl instance;
	private final static Lock lock = new ReentrantLock();
	private Thread currentWorkThread = null;
	private FileInputStream inputStream;
	private String tag = "FirmwareBinControl";
	public static final int FIRMMARE_START = 1;
	public static final int UPGRADE_IN_PROCESS = 2;
	public static final int FIRMMARE_FAILURE = 3;
	public static final int FIRMMARE_SUCCESS = 4;
	public static final int GETIDSUCESS = 5;
	public static final int NOTConTOSevice = 6;
	private UdpClientSocket udpClientSocket = null;
	UdpSendTimer udpSendTimer = null;
	DatagramSocket server = null;
	UdpBrocastToGetIpAndUpgrade udpBrocastToGetIpAndUpgrade;
	String ip = "";
	Timer timer;
	Context mcontext = null;
	Handler mHandler =null;
	String path = null;
	private static final ScheduledExecutorService executePool = Executors
			.newSingleThreadScheduledExecutor();

	public static FirmwareBinControl getInstance() {
		if (instance == null) {
			lock.lock();
			try {
				if (instance == null) {
					instance = new FirmwareBinControl();
				}
			} finally {
				lock.unlock();
			}
		}
		return instance;
	}

	public void sendFirmwareBinFile(Context context, final Handler handler,
			final String path) {
		this.path = path;
		mcontext = context;
		mHandler = handler;
		//计时，若过去20S还没有收到更新完成命令，表示更新失败
		calTime = new CalTime();
		//启动固件更新线程
		UdpRecevieThread udpRecevieThread = new UdpRecevieThread();
		udpRecevieThread.start();
		udpBrocastToGetIpAndUpgrade = new UdpBrocastToGetIpAndUpgrade();
		udpBrocastToGetIpAndUpgrade.start();
		Log.i("zhou", executePool.isShutdown()+"");
		

	}

	// 写在/mnt/sdcard/目录下面的文件

	public void writeFileSdcard(String fileName, byte[] message) {

		try {

			// FileOutputStream fout = openFileOutput(fileName, MODE_PRIVATE);

			FileOutputStream fout = new FileOutputStream(fileName, true);

			/* byte [] bytes = message.getBytes(); */

			fout.write(message);

			fout.close();

		}

		catch (Exception e) {

			e.printStackTrace();

		}

	}

	class CalTime extends Thread {

		@Override
		public void run() {
			// TODO Auto-generated method stub

			try {
				Log.i("time","--------------------------" );
				sleep(25000);
				
				lock2 = true;
				Log.i("time","-----------true-------------" );
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				Log.i("time","-----------true2-------------" );
				e.printStackTrace();
			}
		}

	}

	class UdpBrocastToGetIpAndUpgrade extends Thread {

		@Override
		public void run() {
			// TODO Auto-generated method stub

			try {
			
				mHandler.sendEmptyMessage(GETIDSUCESS);
			
				udpClientSocket = new UdpClientSocket();
				String udpSendInfo = "scan";
				byte[] udpsendbyte = udpSendInfo.getBytes();
				int i = 0;
				while (i < 3) {
					sleep(300);
					udpClientSocket.send("255.255.255.255", 7878, udpsendbyte);
					Log.i("zhou", "udp broacast: " + udpsendbyte);
					i++;
				}
				udpClientSocket.send("255.255.255.255", 7878, udpsendbyte);
				Log.i("zhou", "udp broacast: " + udpsendbyte);
				/*
				 * udpSendTimer = new UdpSendTimer(udpClientSocket,
				 * udpsendbyte);
				 * 
				 * Timer timer = new Timer(); timer.schedule(udpSendTimer, 0,
				 * 1000);
				 */
			} catch (Exception e2) {
				// TODO Auto-generated catch block
				e2.printStackTrace();
			}

		}
	}
	class UdpRecevieThread extends Thread{

		@Override
		public void run() {
			// TODO Auto-generated method stub
			// udp接收
			try {
				byte[] udpreceivebyte = new byte[1024];
				// DatagramSocket server = new DatagramSocket(8787);
				DatagramPacket packet = new DatagramPacket(udpreceivebyte,
						udpreceivebyte.length);
				Log.i("zhou", "udp receiveing..............");
				server = new DatagramSocket(8787);
				server.setSoTimeout(5000);
				server.receive(packet);

				/* udpSendTimer.cancel(); */

				server.close();
				/*
				 * interpretReceiver = new InterpretReceive(context, handler,
				 * packet, null);
				 */
				ip = "";
				ip = packet.getAddress().toString();
				ip = ip.substring(1, ip.length()).trim();
				Message msg = new Message();
				Log.i("zhou", "udp receiveing..............ip:" + ip);
				if (!ip.equals("")) {
					Log.i("zhou", "upgrade.............");
					currentWorkThread = new Thread(new Runnable() {
						@Override
						public void run() {
						
							File file = new File(path);
							int size = (int) file.length();
							int signalingRetryTimes = 5;
							int intervalTime = 100;
							mHandler.sendEmptyMessage(FIRMMARE_START);
							// 发送固件开始升级命令
							SoundPlaySignalingHandler.getInstance().setX20Rcvd(false);
							int retryCount = 0;
							//判断固件能否开始和连接的次数
							while (!SoundPlaySignalingHandler.getInstance().isX20Rcvd()
									&& retryCount < signalingRetryTimes) {
								Log.w(tag, "start Firmware upgrade signaling! bin size = "
										+ size);
								IoBuffer buffer = new FirmwareUpgradeSignaling(0, size)
										.getSignaling();
								MinaUDPClient.getInstance().sendSignaling(ip, buffer);
								retryCount++;
								try {
									Thread.sleep(intervalTime);
								} catch (InterruptedException e) {
									e.printStackTrace();
									mHandler.sendEmptyMessage(FIRMMARE_FAILURE);
									Log.i("fail", "1111111111111111111");
									
								}
							}

							// 建立TCP连接
							FileWriter writer = null;
							MinaTCPClient.getInstance().connect(ip, mHandler);

							int sendTotalSize = 0;
							try {
								inputStream = new FileInputStream(path);
								byte[] cache = new byte[1455];
								int i = inputStream.read(cache);
								/*writeFileSdcard("/sdcard/Y.txt", cache);*/
								sendTotalSize += i;
								int tcpDisconnect = 0;
								IoBuffer buffer2 = null;
								int count = 0;
								while (i != -1
										&& SoundPlaySignalingHandler.getInstance()
												.isX20Rcvd()) {
									if (MinaTCPClient.getInstance().isTcpOnLine()) {

										// 确保最后一次，不是传1455字节
										if (i < 1455) {
											byte[] cache2 = new byte[i];
											System.arraycopy(cache, 0, cache2, 0, i);
											buffer2 = new FirmwareBinDataSignaling(cache2)
													.getSignaling();
											/*writeFileSdcard("/sdcard/Y.txt", cache2);*/
										} else {
											buffer2 = new FirmwareBinDataSignaling(cache)
													.getSignaling();
											/*writeFileSdcard("/sdcard/Y.txt", cache);*/
										}

										MinaTCPClient.getInstance().sendSignaling(buffer2);

										count++;
										i = inputStream.read(cache);
										/*writeFileSdcard("/sdcard/Y.txt", cache);*/
										sendTotalSize += i;
										int percent = (int) (((float) sendTotalSize / (float) size) * 100);
										Log.w(tag, "sendFirmwareBinFile,total size = "
												+ sendTotalSize + ",percent = " + percent);
										Thread.sleep(100);

										if (count % 10 == 0) {
											Message msg = new Message();
											msg.what = UPGRADE_IN_PROCESS;
											msg.arg1 = percent;
											mHandler.sendMessage(msg);
										}
										
										tcpDisconnect = 0;
									} else {
										Log.w(tag, "Tcp is not On Line");
										tcpDisconnect++;
										if (tcpDisconnect > 20000) {
											mHandler.sendEmptyMessage(FIRMMARE_FAILURE);
											Log.i("fail", "222222222222222222");
											break;

										}
									}
								}

							} catch (Exception e) {
								Log.w(tag, "sendFirmwareBinFile, Exception");
								mHandler.sendEmptyMessage(FIRMMARE_FAILURE);
								Log.i("fail", "3333333333333333");
								/*calTime.destroy();*/
							}

							try {
								Thread.sleep(1000);
							} catch (InterruptedException e) {
								mHandler.sendEmptyMessage(FIRMMARE_FAILURE);
								/*calTime.destroy();*/
								Log.i("fail", "444444444444444444");
							}

							lock2 = false;
								calTime.start();
								
							
							SoundPlaySignalingHandler.getInstance().setX20Rcvd(false);
							Log.i("zhou", SoundPlaySignalingHandler.getInstance().isX20Rcvd()+":"+lock2);
							while (true) {
								if (SoundPlaySignalingHandler.getInstance().isX20Rcvd()
										&& sendTotalSize > 100 * 1024 && lock2 == false) {
									Log.i("zhou", SoundPlaySignalingHandler.getInstance().isX20Rcvd()+":"+lock2);
									calTime.interrupt();
									Log.i("zhou", executePool.isShutdown()+"1111");
									mHandler.sendEmptyMessage(FIRMMARE_SUCCESS);
									
									
									sendTotalSize = 0;
									
									break;
								} else if (lock2 == true) {
									calTime = null;
									mHandler.sendEmptyMessage(FIRMMARE_FAILURE);
									sendTotalSize = 0;
									Log.i("fail", "55555555555555555555");
									
					
									break;
								}
							}
							/*// 发送固件升级完成命令
							SoundPlaySignalingHandler.getInstance().setX20Rcvd(false);
							int retryTimes = 0;
							while (!SoundPlaySignalingHandler.getInstance().isX20Rcvd()
									&& retryTimes < signalingRetryTimes) {
								Log.w(tag, "stop Firmware upgrade signaling! ");
								IoBuffer buffer = new FirmwareUpgradeSignaling(1, size)
										.getSignaling();
								MinaUDPClient.getInstance().sendSignaling(ip, buffer);
								retryTimes++;
								try {
									Thread.sleep(intervalTime);
								} catch (InterruptedException e) {
									e.printStackTrace();
								}
							}
							if (SoundPlaySignalingHandler.getInstance().isX20Rcvd()
									&& sendTotalSize > 100 * 1024) {
							
								Log.i("zhou", executePool.isShutdown()+"");
								mHandler.sendEmptyMessage(FIRMMARE_SUCCESS);
								
							} else {
								mHandler.sendEmptyMessage(FIRMMARE_FAILURE);
				

							}*/
							/*currentWorkThread.stop();
							currentWorkThread = null;*/
							/*executePool.shutdownNow();*/
							Log.i("zhou", executePool.isShutdown()+"finish");
							
						}
					});
					currentWorkThread.setPriority(Thread.MAX_PRIORITY);
					/*currentWorkThread.start();*/
					executePool.execute(currentWorkThread);
				}
			} catch (Exception e1) {
				e1.printStackTrace();
				// udpSendTimer.cancel();
				server.close();

				Log.i("zhou", "udp have no responds..............");
				mHandler.sendEmptyMessage(NOTConTOSevice);
				
			}
		
		}
		
	}

}
