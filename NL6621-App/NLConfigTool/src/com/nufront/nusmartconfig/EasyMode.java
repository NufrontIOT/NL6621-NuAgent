package com.nufront.nusmartconfig;

import java.io.UnsupportedEncodingException;

import android.app.Activity;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.text.InputType;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;

public class EasyMode extends Activity {
	private static final String TAG = "EasyMode";
	private EditText ssid;
	private EditText password;
	private CheckBox showPasswd;
	private Button ok;
	private Button cancel;
	private String ssidString = null;
	private PowerManager.WakeLock mFullWakeLock;
	private UDPServer udpServer;
	private UIHandler mUihandler = new UIHandler();
	public static final int MES_UDP_LOGIN = 0;

	class UIHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch (msg.what) {
			case MES_UDP_LOGIN:
				// Log.i(TAG,"MSG: MES_UDP_LOGIN");
				ConfigWireless.getInstance().cancel();
				ConfigWireless.sendFlag = false;
				if (udpServer != null) {
					udpServer.CloseServer();
					udpServer = null;
				}
				ShowLoginDialog();
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
		setContentView(R.layout.easymode);
		initUI();
		// Log.i(TAG, "onCreate");
		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		mFullWakeLock = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK
				| PowerManager.ON_AFTER_RELEASE, TAG);
		WifiAdmin wifiAdmin = new WifiAdmin(EasyMode.this);
		if (wifiAdmin.isWifiOpen()) {
			ssidString = wifiAdmin.getSSID();// "NL6621"
			if (ssidString.contains("\"")) {
				ssidString = ssidString.substring(1, ssidString.length() - 1);
			}
			ssid.setText(ssidString);

		}
		if (!isWiFiActive()) {
			Builder dialog = new Builder(this);
			dialog.setIcon(R.drawable.ic_launcher)
					.setTitle(R.string.warning)
					.setMessage(
							getResources().getString(R.string.wifi_not_open))
					.setCancelable(false)
					.setPositiveButton(R.string.alert_dialog_ok,
							new DialogInterface.OnClickListener() {

								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									finish();
									System.exit(0);
								}
							}).create().show();
		}
	}

	private boolean isWiFiActive() {
		WifiManager mWifiManager = (WifiManager) EasyMode.this
				.getSystemService(Context.WIFI_SERVICE);
		WifiInfo wifiInfo = mWifiManager.getConnectionInfo();
		int ipAddress = wifiInfo == null ? 0 : wifiInfo.getIpAddress();
		if (mWifiManager.isWifiEnabled() && ipAddress != 0) {
			return true;
		} else {
			return false;
		}
	}

	private void initUI() {
		// TODO Auto-generated method stub
		this.ssid = (EditText) this.findViewById(R.id.ssid);
		this.password = (EditText) this.findViewById(R.id.password);
		this.showPasswd = (CheckBox) this.findViewById(R.id.dispaly_check);
		this.ok = (Button) this.findViewById(R.id.ok);
		this.cancel = (Button) this.findViewById(R.id.cancel);
		this.ok.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				String apSsid = ssid.getText().toString().trim();
				String key = password.getText().toString().trim();
				ConfigWireless.key = key;

				try {
					ConfigWireless.ssid = apSsid.getBytes("UTF-8");
				} catch (UnsupportedEncodingException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				Log.i(TAG, "ssid :" + ConfigWireless.ssid.toString()
						+ " password :" + ConfigWireless.key);
				ConfigWireless.getInstance().cancel();
				if (!ConfigWireless.sendFlag)
					Toast("重新广播发送SSID和KEY！");
				else
					Toast("开始广播发送SSID和KEY！");
				ConfigWireless.sendFlag = true;
				ConfigWireless.getInstance().run();
				if (udpServer == null) {
					udpServer = new UDPServer(60002, mUihandler);
				}
			}
		});

		this.cancel.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				ConfigWireless.getInstance().cancel();
				ConfigWireless.sendFlag = false;
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

	private void Toast(String text) {
		android.widget.Toast.makeText(getApplicationContext(), text,
				android.widget.Toast.LENGTH_SHORT).show();
	}

	public void ShowLoginDialog() {
		showDialog(2);
	}

	private void releaseAll() {
		if (mFullWakeLock.isHeld()) {
			mFullWakeLock.release();
		}

		ConfigWireless.getInstance().cancel();
		ConfigWireless.sendFlag = false;
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
		Builder dialog;
		if (id == 0) {
			dialog = new Builder(this);
			dialog.setIcon(R.drawable.ic_launcher);
			dialog.setTitle(R.string.app_name);
			dialog.setMessage(String.format(
					getResources().getString(R.string.alert_dialog_text),
					getResources().getString(R.string.app_name)));

			dialog.setCancelable(true);
			dialog.setPositiveButton(R.string.alert_dialog_ok,
					new DialogInterface.OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							ConfigWireless.getInstance().cancel();
							ConfigWireless.sendFlag = false;
							if (udpServer != null) {
								udpServer.CloseServer();
							}
							finish();
							System.exit(0);
						}
					});
			dialog.setNegativeButton(R.string.alert_dialog_cancel, null);
			return dialog.create();
		} else if (id == 1) {

		} else if (id == 2) {
			dialog = new Builder(this);
			dialog.setIcon(R.drawable.ic_launcher);
			dialog.setTitle(R.string.app_name);
			dialog.setMessage(String.format(
					getResources().getString(R.string.device_isconnected),
					getResources().getString(R.string.app_name)));

			dialog.setCancelable(true);
			dialog.setPositiveButton(R.string.alert_dialog_ok,
					new DialogInterface.OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							dialog.dismiss();
						}
					});
			return dialog.create();
		}
		return null;
	}
}
