package com.nufront.nusmartconfig;

import java.util.List;

import android.content.Context;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.util.Log;

public class WifiAdmin {
	private static final String TAG = "WifiAdmin";
	// 定义WifiManager对象
	private WifiManager mWifiManager;
	// 定义WifiInfo对象
	private WifiInfo mWifiInfo;
	// 扫描出的网络连接列表
	private List<ScanResult> mWifiList;
	// 网络连接列表
	private List<WifiConfiguration> mWifiConfiguration;
	// 定义一个WifiLock
	WifiLock mWifiLock;

	// 构造器
	public WifiAdmin(Context context) {
		// 取得WifiManager对象
		mWifiManager = (WifiManager) context
				.getSystemService(Context.WIFI_SERVICE);
		// 取得WifiInfo对象
		mWifiInfo = mWifiManager.getConnectionInfo();
	}

	// 打开WIFI
	public void openWifi() {
		if (!mWifiManager.isWifiEnabled()) {
			mWifiManager.setWifiEnabled(true);
		}
	}

	public boolean isWifiOpen() {

		return mWifiManager.isWifiEnabled();
	}

	// 关闭WIFI
	public void closeWifi() {
		if (mWifiManager.isWifiEnabled()) {
			mWifiManager.setWifiEnabled(false);
		}
	}

	// 检查当前WIFI状态
	public int checkState() {
		return mWifiManager.getWifiState();
	}

	// 锁定WifiLock
	public void acquireWifiLock() {
		mWifiLock.acquire();
	}

	// 解锁WifiLock
	public void releaseWifiLock() {
		// 判断时候锁定
		if (mWifiLock.isHeld()) {
			mWifiLock.acquire();
		}
	}

	// 创建一个WifiLock
	public void creatWifiLock() {
		mWifiLock = mWifiManager.createWifiLock("Test");
	}

	// 得到配置好的网络
	public List<WifiConfiguration> getConfiguration() {
		return mWifiConfiguration;
	}

	// 指定配置好的网络进行连接
	public void connectConfiguration(int index) {
		// 索引大于配置好的网络索引返回
		if (index > mWifiConfiguration.size()) {
			return;
		}
		// 连接配置好的指定ID的网络
		mWifiManager.enableNetwork(mWifiConfiguration.get(index).networkId,
				true);
	}

	public void startScan() {
		mWifiManager.startScan();
		// 得到扫描结果
		mWifiList = mWifiManager.getScanResults();
		// 得到配置好的网络连接
		mWifiConfiguration = mWifiManager.getConfiguredNetworks();
	}

	// 得到网络列表
	public List<ScanResult> getWifiList() {
		return mWifiList;
	}

	// 查看扫描结果
	public StringBuilder lookUpScan() {
		StringBuilder stringBuilder = new StringBuilder();
		for (int i = 0; i < mWifiList.size(); i++) {
			stringBuilder
					.append("Index_" + new Integer(i + 1).toString() + ":");
			// 将ScanResult信息转换成一个字符串包
			// 其中把包括：BSSID、SSID、capabilities、frequency、level
			stringBuilder.append((mWifiList.get(i)).toString());
			stringBuilder.append("/n");
		}
		return stringBuilder;
	}

	// 得到MAC地址
	public String getMacAddress() {
		return (mWifiInfo == null) ? "NULL" : mWifiInfo.getMacAddress();
	}

	// 得到接入点的BSSID
	public String getBSSID() {
		return (mWifiInfo == null) ? "NULL" : mWifiInfo.getBSSID();
	}

	public String getSSID() {
		return (mWifiInfo == null) ? "NULL" : mWifiInfo.getSSID();
	}

	// 得到IP地址
	public int getIPAddress() {
		return (mWifiInfo == null) ? 0 : mWifiInfo.getIpAddress();
	}

	// 得到连接的ID
	public int getNetworkId() {
		return (mWifiInfo == null) ? 0 : mWifiInfo.getNetworkId();
	}

	// 得到WifiInfo的所有信息包
	public String getWifiInfo() {
		return (mWifiInfo == null) ? "NULL" : mWifiInfo.toString();
	}

	// 添加一个网络并连接
	public void addNetwork(WifiConfiguration wcg) {
		int wcgID = mWifiManager.addNetwork(wcg);
		boolean b = mWifiManager.enableNetwork(wcgID, true);
		Log.i(TAG, "addNetwork------>netid = " + wcgID);
		Log.i(TAG, "boolean = " + b);
		// mWifiInfo = mWifiManager.getConnectionInfo();
	}

	// 断开指定ID的网络
	public void disconnectWifi(int netId) {
		mWifiManager.disableNetwork(netId);
		mWifiManager.disconnect();
		mWifiInfo = null;
	}

	// 然后是一个实际应用方法，只验证过没有密码的情况：

	public WifiConfiguration CreateWifiInfo(String SSID, String Password,
			int Type) {
		WifiConfiguration config = new WifiConfiguration();
		config.allowedAuthAlgorithms.clear();
		config.allowedGroupCiphers.clear();
		config.allowedKeyManagement.clear();
		config.allowedPairwiseCiphers.clear();
		config.allowedProtocols.clear();
		config.SSID = "\"" + SSID + "\"";

		/*
		 * WifiConfiguration tempConfig = this.IsExsits(SSID); if(tempConfig !=
		 * null) { mWifiManager.removeNetwork(tempConfig.networkId); }
		 */

		if (Type == 1) // WIFICIPHER_NOPASS
		{
			config.wepKeys[0] = "";
			config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
			config.wepTxKeyIndex = 0;
		}
		if (Type == 2) // WIFICIPHER_WEP
		{
			config.hiddenSSID = true;
			config.wepKeys[0] = "\"" + Password + "\"";
			config.allowedAuthAlgorithms
					.set(WifiConfiguration.AuthAlgorithm.SHARED);
			config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
			config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
			config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
			config.allowedGroupCiphers
					.set(WifiConfiguration.GroupCipher.WEP104);
			config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
			config.wepTxKeyIndex = 0;
		}
		if (Type == 3) // WIFICIPHER_WPA
		{
			config.preSharedKey = "\"" + Password + "\"";
			config.hiddenSSID = true;
			config.allowedAuthAlgorithms
					.set(WifiConfiguration.AuthAlgorithm.OPEN);
			config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
			config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
			config.allowedPairwiseCiphers
					.set(WifiConfiguration.PairwiseCipher.TKIP);
			// config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
			config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
			config.allowedPairwiseCiphers
					.set(WifiConfiguration.PairwiseCipher.CCMP);
			config.status = WifiConfiguration.Status.ENABLED;
		}
		return config;
	}

	private WifiConfiguration IsExsits(String SSID) {
		List<WifiConfiguration> existingConfigs = mWifiManager
				.getConfiguredNetworks();
		for (WifiConfiguration existingConfig : existingConfigs) {
			if (existingConfig.SSID.equals("\"" + SSID + "\"")) {
				return existingConfig;
			}
		}
		return null;
	}

	/*
	 * public void isConnect() { // TODO Auto-generated method stub
	 * mWifiManager.getWifiState() }
	 */

}
/*
 * //分为三种情况：1没有密码2用wep加密3用wpa加密
 * 
 * 改动主要集中在CreateWifiInfo这个方法中，并且添加了一个私有方法：
 * 
 * （1）将与方法的第三个参数有关的变量都改成int型，或者使用原作者的枚举型（存在bug需要改正），但枚举会在后续的开发中遇到些困难；
 * 
 * （2）在if(type ==
 * 3)中注释掉“config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);”，并添加“
 * 
 * config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
 * config.allowedPairwiseCiphers
 * .set(WifiConfiguration.PairwiseCipher.CCMP);“这两句，否则当wifi热点需要输入密码时，无法加入网络。
 * 
 * （3）在代码末尾添加了方法IsExsits，原因在于如果按照网上介绍的方法成功加入指定的wifi后，都会在终端的wifi列表中新添加一个以该指定ssid的网络
 * ，所以每运行一次程序，列表中就会多一个相同名字的ssid。而该方法就是检查wifi列表中是否有以输入参数为名的wifi热点，如果存在，
 * 则在CreateWifiInfo方法开始配置wifi网络之前将其移除，以避免ssid的重复：
 * 
 * WifiConfiguration tempConfig = this.IsExsits(SSID);
 * 
 * if(tempConfig != null) {
 * 
 * mWifiManager.removeNetwork(tempConfig.networkId);
 * 
 * }
 */
