package com.nufront.nusmartconfig;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import com.nufront.nusmartconfig.EasyMode.UIHandler;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.preference.ListPreference;
import android.text.InputType;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class AdhocMode extends Activity {
	private static final String TAG = "AdhocMode";
	private EditText ssid;
	private EditText password;
	private CheckBox showPasswd;
	private Button ok;
	private Button cancel;
	private Button search;
	private String ssidString = null;
	private PowerManager.WakeLock mFullWakeLock;
	private UDPServer udpServer;
	private UIHandler mUihandler = new UIHandler();
	public static final int MES_UDP_LOGIN = 0;
	public static final int MES_UPDATE_SSID = 1;
	private static final String MSG_KEY_SSID = "SSID";
	private static final int DIALOG_ID_WIFI_INFO = 4;
	/* private static final int MAX_WIFI_NUM = 30; */
	private static final int MAX_WIFI_LEVEL = 4;

	class UIHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch (msg.what) {
			case MES_UDP_LOGIN:
				// Log.i(TAG, "MSG: MES_UDP_LOGIN");
				ConfigWireless.getInstance().cancel();
				ConfigWireless.sendFlagAp = false;
				if (udpServer != null) {
					udpServer.CloseServer();
					udpServer = null;
				}
				ShowLoginDialog();
				break;
			case MES_UPDATE_SSID:
				// Log.i(TAG, "MSG: MES_UPDATE_SSID");
				String ssidString = msg.getData().getString(MSG_KEY_SSID);
				ssid.setText(ssidString);
				break;

			default:
				break;
			}
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.adhocmode);
		initUI();
		// Log.i(TAG, "onCreate");
		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		mFullWakeLock = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK
				| PowerManager.ON_AFTER_RELEASE, TAG);
	}

	private void initUI() {
		// TODO Auto-generated method stub
		this.ssid = (EditText) this.findViewById(R.id.ssid);
		this.password = (EditText) this.findViewById(R.id.password);
		this.showPasswd = (CheckBox) this.findViewById(R.id.dispaly_check);
		this.search = (Button) this.findViewById(R.id.search);
		this.ok = (Button) this.findViewById(R.id.ok);
		this.cancel = (Button) this.findViewById(R.id.cancel);
		this.search.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				showDialog(DIALOG_ID_WIFI_INFO);
			}

		});

		this.ok.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				String apSsid = ssid.getText().toString().trim();
				String key = password.getText().toString().trim();
				ConfigWireless.key = key;

				ConfigWireless.localIp = getAPIpAddr();
				// ConfigWireless.localIp = "172.16.96.132";
				try {
					ConfigWireless.ssid = apSsid.getBytes("UTF-8");
				} catch (UnsupportedEncodingException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				Log.i(TAG, "ssid :" + ConfigWireless.ssid.toString()
						+ " password :" + ConfigWireless.key);
				ConfigWireless.getInstance().cancel();
				if (!ConfigWireless.sendFlagAp)
					Toast("重新发送SSID和KEY！");
				else
					Toast("开始发送SSID和KEY！");
				ConfigWireless.sendFlagAp = true;
				ConfigWireless.getInstance().run();
				// if (udpServer == null) {
				// udpServer = new UDPServer(60002, mUihandler);
				// }
			}
		});

		this.cancel.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				ConfigWireless.getInstance().cancel();
				ConfigWireless.sendFlagAp = false;
				if (udpServer != null) {
					Toast("停止发送！");
					udpServer.CloseServer();
					udpServer = null;
				}
			}
		});
		this.showPasswd
				.setOnCheckedChangeListener(new OnCheckedChangeListener() {

					@Override
					public void onCheckedChanged(CompoundButton buttonView,
							boolean isChecked) {
						// TODO Auto-generated method stub
						if (isChecked)
							password.setInputType(InputType.TYPE_TEXT_VARIATION_PASSWORD);
						else
							password.setInputType(InputType.TYPE_CLASS_TEXT
									| InputType.TYPE_TEXT_VARIATION_PASSWORD);
					}

				});
	}

	private String getAPIpAddr() {
		// TODO Auto-generated method stub
		String ipAddr;
		WifiManager mWifiManager = (WifiManager) AdhocMode.this
				.getSystemService(Context.WIFI_SERVICE);
		WifiInfo wifiInfo = mWifiManager.getConnectionInfo();
		ipAddr = intToIp(wifiInfo.getIpAddress());
		// Log.i(TAG, "ipAddr = " + ipAddr );
		return ipAddr;
	}

	private String intToIp(int i) {
		return (i & 0xFF) + "." + ((i >> 8) & 0xFF) + "." + ((i >> 16) & 0xFF)
				+ "." + 1; // last value = 1
	}

	private void Toast(String text) {
		android.widget.Toast.makeText(getApplicationContext(), text,
				android.widget.Toast.LENGTH_SHORT).show();
	}

	public void ShowLoginDialog() {
		showDialog(2);
	}

	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		if (event.getKeyCode() == KeyEvent.KEYCODE_BACK
				&& event.getAction() == KeyEvent.ACTION_DOWN) {

			showDialog(0);

			return true;
		} else if (event.getKeyCode() == KeyEvent.KEYCODE_SEARCH
				&& event.getAction() == KeyEvent.ACTION_DOWN) {

			showDialog(1);

			return true;
		}
		return super.dispatchKeyEvent(event);
	}

	@Override
	protected Dialog onCreateDialog(int id) {
		Builder builder;
		if (id == 0) {
			builder = new Builder(this);
			builder.setIcon(R.drawable.ic_launcher);
			builder.setTitle(R.string.app_name);
			builder.setMessage(String.format(
					getResources().getString(R.string.alert_dialog_text),
					getResources().getString(R.string.app_name)));

			builder.setCancelable(true);
			builder.setPositiveButton(R.string.alert_dialog_ok,
					new DialogInterface.OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							ConfigWireless.getInstance().cancel();
							ConfigWireless.sendFlagAp = false;
							if (udpServer != null) {
								udpServer.CloseServer();
							}
							finish();
							System.exit(0);
						}
					});
			builder.setNegativeButton(R.string.alert_dialog_cancel, null);
			return builder.create();
		} else if (id == 1) {

		} else if (id == 2) {
			builder = new Builder(this);
			builder.setIcon(R.drawable.ic_launcher);
			builder.setTitle(R.string.app_name);
			builder.setMessage(String.format(
					getResources().getString(R.string.device_isconnected),
					getResources().getString(R.string.app_name)));

			builder.setCancelable(true);
			builder.setPositiveButton(R.string.alert_dialog_ok,
					new DialogInterface.OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							dialog.dismiss();
						}
					});
			return builder.create();
		} else if (id == DIALOG_ID_WIFI_INFO) {
			Log.i(TAG, "DIALOG_ID_WIFI_INFO");
			/*
			 * List<ScanResult> wifiList; WifiAdmin wifiAdmin = new
			 * WifiAdmin(AdhocMode.this); Log.i(TAG,"startScan");
			 * wifiAdmin.startScan(); Log.i(TAG,"getWifiList"); wifiList =
			 * wifiAdmin.getWifiList(); dialog = new Builder(this);
			 * dialog.setIcon(R.drawable.ic_launcher);
			 * dialog.setTitle(R.string.wifi_info);
			 * Log.i(TAG,"init wifiInfoList"); String [] wifiInfoList = new
			 * String[MAX_WIFI_NUM]; int size = wifiList.size(); if(size >
			 * MAX_WIFI_NUM) size = MAX_WIFI_NUM; for(int i=0; i<size; i++){
			 * Log.i(TAG, wifiList.get(i).SSID + "  " + wifiList.get(i).level);
			 * wifiInfoList[i] = wifiList.get(i).SSID + "                    " +
			 * translateLevelToSignal(wifiList.get(i).level); }
			 * 
			 * dialog.setItems(wifiInfoList, new
			 * DialogInterface.OnClickListener(){
			 * 
			 * @Override public void onClick(DialogInterface dialog, int which)
			 * { // TODO Auto-generated method stub
			 * 
			 * }
			 * 
			 * });
			 */
			List<ScanResult> wifiList;
			WifiAdmin wifiAdmin = new WifiAdmin(AdhocMode.this);
			wifiAdmin.startScan();
			wifiList = wifiAdmin.getWifiList();
			int size = wifiList.size();
			/*
			 * if (size > MAX_WIFI_NUM) size = MAX_WIFI_NUM;
			 */

			View wifiInfoView = getLayoutInflater().inflate(R.layout.wifi_info,
					null);
			ListView wifiInfoListView = (ListView) wifiInfoView
					.findViewById(R.id.wifiInfoListView);
			ArrayList<HashMap<String, Object>> listItem = new ArrayList<HashMap<String, Object>>();
			for (int i = 0; i < size; i++) {
				// Log.i(TAG, wifiList.get(i).SSID + "  " +
				// wifiList.get(i).level);
				HashMap<String, Object> map = new HashMap<String, Object>();
				map.put("signalImage",
						translateLevelToIcon(wifiList.get(i).level));
				map.put("ssidTextView", wifiList.get(i).SSID);
				listItem.add(map);

			}
			builder = new Builder(this);
			builder.setIcon(R.drawable.ic_launcher);
			builder.setTitle(R.string.wifi_info);
			builder.setView(wifiInfoView);
			SimpleAdapter simpleAdapter = new SimpleAdapter(this, listItem,
					R.layout.item,
					new String[] { "signalImage", "ssidTextView" }, new int[] {
							R.id.signalImage, R.id.ssidTextView });
			wifiInfoListView.setAdapter(simpleAdapter);
			wifiInfoListView.setOnItemClickListener(new OnItemClickListener() {

				@Override
				public void onItemClick(AdapterView<?> parent, View view,
						int position, long id) {
					// TODO Auto-generated method stub
					Log.i(TAG, "onItemClick");
					ListView listView = (ListView) parent;
					HashMap<String, Object> map = (HashMap<String, Object>) parent
							.getItemAtPosition(position);

					String ssidString = (String) map.get("ssidTextView");
					Log.i(TAG, "ssid=" + ssidString);
					Message msg = mUihandler.obtainMessage();
					msg.what = MES_UPDATE_SSID;
					Bundle bundle = new Bundle();
					bundle.putString(MSG_KEY_SSID, ssidString);
					msg.setData(bundle);
					mUihandler.sendMessage(msg);
					dismissDialog(DIALOG_ID_WIFI_INFO);
					// System.exit(0);
				}

			});
			return builder.create();
		}

		return null;
	}

	private String translateLevelToSignal(int level) {
		// TODO Auto-generated method stub
		String signal = null;
		switch (WifiManager.calculateSignalLevel(level, MAX_WIFI_LEVEL)) {
		case 3:
			signal = "Excellent";
			break;
		case 2:
			signal = "Good";
			break;
		case 1:
			signal = "Normal";
			break;
		case 0:
			signal = "Weak";
			break;
		}
		return signal;
	}

	private int translateLevelToIcon(int level) {
		// TODO Auto-generated method stub
		int icon = 0;
		switch (WifiManager.calculateSignalLevel(level, MAX_WIFI_LEVEL)) {
		case 3:
			icon = R.drawable.ic_wifi_signal_4;
			break;
		case 2:
			icon = R.drawable.ic_wifi_signal_3;
			break;
		case 1:
			icon = R.drawable.ic_wifi_signal_2;
			break;
		case 0:
			icon = R.drawable.ic_wifi_signal_1;
			break;
		}
		return icon;
	}

	private void releaseAll() {
		if (mFullWakeLock.isHeld()) {
			mFullWakeLock.release();
		}

		ConfigWireless.getInstance().cancel();
		ConfigWireless.sendFlagAp = false;
		if (udpServer != null) {
			Toast("停止发送！");
			udpServer.CloseServer();
			udpServer = null;
		}
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		// Log.i(TAG,"onResume");
		if (!mFullWakeLock.isHeld()) {
			mFullWakeLock.acquire();
		}
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		// Log.i(TAG,"onDestroy");
		if (mFullWakeLock.isHeld()) {
			mFullWakeLock.release();
		}
	}

	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		// Log.i(TAG,"onStart");
	}

	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		super.onRestart();
		// Log.i(TAG,"onRestart");
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		// Log.i(TAG,"onPause");
		releaseAll();
	}

	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		// Log.i(TAG,"onStop");
	}
}
