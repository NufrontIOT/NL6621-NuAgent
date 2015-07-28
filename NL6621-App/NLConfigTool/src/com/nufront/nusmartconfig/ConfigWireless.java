package com.nufront.nusmartconfig;

import java.nio.ByteBuffer;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import android.util.Log;

public class ConfigWireless {
	public static byte[] ssid;
	public static String key;

	public static byte[] configPacket;

	public static boolean sendFlag = false;

	public static boolean sendFlagAp = false;
	public static String localIp = "";

	private static ConfigWireless instance;

	private static ScheduledExecutorService service = Executors
			.newSingleThreadScheduledExecutor();
	private ScheduledFuture<?> future;

	private ConfigWireless() {
	}

	public static ConfigWireless getInstance() {
		if (instance == null) {
			try {
				if (instance == null) {
					instance = new ConfigWireless();
				}
			} finally {
				;
			}
		}

		return instance;
	}

	public void run() {
		future = service.schedule(new Runnable() {
			public void run() {
				Log.e("ConfigWireless", "scheduleAtFixedRate check.");
				try {
					if (sendFlag)
						checkRetry();
					if (sendFlagAp)
						sendData();
				} catch (Exception e) {
					Log.e("ConfigWireless",
							"RetrySendScheduler checkRetry error."
									+ e.toString());
					e.printStackTrace();
				}

			}
		}, 1, TimeUnit.SECONDS);
	}

	public void cancel() {
		if (future != null)
			this.future.cancel(true);

	}

	public void shutdown() {
		if (future != null)
			this.future.cancel(true);
		if (!service.isShutdown())
			service.shutdown();
	}

	private void checkRetry() throws Exception {
		int l = 0;
		int k = 1;
		while (sendFlag) {

			configPacket = ConfigwlHelper.buildConfigPacket(ssid, key);
			Log.i("ConfigWireless", "checkRetry configPacket.length="
					+ configPacket.length);
			byte[] srcbs = new byte[1024];
			System.arraycopy(configPacket, 0, srcbs, 0, configPacket.length);

			k = 1;
			for (int i = 0; i < configPacket.length; i++) {

				l = (configPacket[i] + 256) % 256;
				l = l == 0 ? 129 : l;
				l = k == 1 ? (156 + l) : (156 - l);
				k = 1 - k;

				byte[] bs = new byte[l];
				Log.i("ConfigWireless", "configPacket send len=" + l);
				System.arraycopy(srcbs, 0, bs, 0, l);

				for (int j = 0; j < 20; j++) {

					UdpClient.getInstance().sendBroadCastSignaling(
							"239.1.2.110", bs);// send

					Thread.sleep(3);
				}
				Thread.sleep(30); // wait 30 ms
			}
			// Thread.sleep(1000); // wait 1000 ms
		}
	}

	private void sendData() throws Exception {
		while (sendFlagAp) {

			configPacket = DataPacket(ssid, key);
			Log.i("ConfigWireless", "sendData configPacket.length="
					+ configPacket.length);
			byte[] srcbs = new byte[200];
			System.arraycopy(configPacket, 0, srcbs, 0, configPacket.length);

			UdpClient.getInstance().sendBroadCastSignaling(localIp, srcbs);// send

			Thread.sleep(10);
		}
	}

	public static byte[] DataPacket(byte[] ssid, String key) {
		ByteBuffer buffer = ByteBuffer.allocate(200);
		short size = 200;
		short cmd = 0x11;
		short func = 0;
		short msg = 0;
		int ssidLen = 0;
		int keyLen = 0;

		buffer.put((byte) (size & 0xff));
		buffer.put((byte) ((size >> 8) & 0xff));
		buffer.put((byte) (cmd & 0xff));
		buffer.put((byte) ((cmd >> 8) & 0xff));
		buffer.put((byte) (func & 0xff));
		buffer.put((byte) ((func >> 8) & 0xff));
		buffer.put((byte) (msg & 0xff));
		buffer.put((byte) ((msg >> 8) & 0xff));
		
		Log.i("ConfigWireless", "size:" + size + " cmd:" + cmd + " func:" + func + " msg:" + msg);

		// ssid len
		if (ssid != null)
			ssidLen = ssid.length;
		buffer.put((byte) ssidLen);

		// ssid
		if (ssidLen != 0) {
			byte[] ssids = ssid;

			buffer.put(ssids);
		}

		// key len
		if (key != null)
			keyLen = key.getBytes().length;
		buffer.put((byte) keyLen);

		// key
		if (keyLen != 0) {
			byte[] keys = key.getBytes();
			buffer.put(keys);
		}
		buffer.put((byte) 0x08);
		buffer.put((byte) 0x09);
		buffer.put((byte) 0x0A);
		buffer.put((byte) 0x0B);
		buffer.flip();

		byte[] re = new byte[buffer.remaining()];
		buffer.get(re);

		return re;
	}
}
