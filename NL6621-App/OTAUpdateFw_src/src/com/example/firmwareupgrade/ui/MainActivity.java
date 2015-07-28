package com.example.firmwareupgrade.ui;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

import com.example.firmwareupgrade.R;




import com.nufront.wifiplay.mvsilicon.helper.FirmwareBinControl;
import com.nufront.wifiplay.socket.MinaUDPClient;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.graphics.Color;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
	/*
	 * 对象声明 items：存放显示的名称 paths：存放文件路径 rootPath：起始目录
	 */
	Context mcontext = null;
	private List<String> items = null;
	private List<String> paths = null;
	private String defaultPath = "/sdcard";
	private String rootPath = "/";
	private String currentFilePath = "";
	private static final String binHead = "Nu_link";
	private static final String binTail = "DEADBEEF";

	private TextView mPath;
	private ListView fileListView;
	private Button upgrade;
	private ProgressDialog upgradeDialog;
	
	private String tag = "FirmwareUpgradeActivity";
	public static final int FIRMMARE_START = 1;	
	public static final int UPGRADE_IN_PROCESS = 2;
	public static final int FIRMMARE_FAILURE = 3;
	public static final int FIRMMARE_SUCCESS = 4;
	public static final int GETIDSUCESS = 5;
	public static final int NOTConTOSevice = 6;
	private Handler handler = new Handler(){
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch(msg.what){
				case FIRMMARE_START:
					upgradeDialog = ProgressDialog.show(MainActivity.this, getResources().getString(R.string.firmware_upgrade), 
							"开始升级固件！请稍后......",true,false);
					
					break;
				case UPGRADE_IN_PROCESS:
					int percent = msg.arg1;
					upgradeDialog.setMessage("正在固件升级,当前已完成"+percent+"%");
					break;
				case FIRMMARE_FAILURE:
					upgrade.setEnabled(true);
					Toast.makeText(MainActivity.this, "升级固件失败！", Toast.LENGTH_LONG).show();
					FirmwareBinControl.lock2 = false;
					upgradeDialog.dismiss();
					
					break;
				case FIRMMARE_SUCCESS:
					upgrade.setEnabled(true);
					Toast.makeText(MainActivity.this, "升级固件成功！", Toast.LENGTH_LONG).show();
					FirmwareBinControl.lock2 = false;
					upgradeDialog.dismiss();
					
					break;
				case GETIDSUCESS:
					Toast.makeText(MainActivity.this, "开始连接设备", Toast.LENGTH_LONG).show();
					upgrade.setEnabled(false);
					break;
				case NOTConTOSevice:
					Toast.makeText(MainActivity.this, "没有连接上设备", Toast.LENGTH_LONG).show();
					upgrade.setEnabled(true);
					FirmwareBinControl.lock2 = false;
					break;
			}
		}
	};
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mcontext = this;
		mPath = (TextView) findViewById(R.id.mPath);
		fileListView = (ListView) findViewById(R.id.file_list);
		upgrade = (Button) findViewById(R.id.ok);	
		fileListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> av, View view, int position, long id) {
				// TODO Auto-generated method stub
				Log.w("onItemClick", "onItemClick执行");
				File file = new File(paths.get(position));
				if (file.canRead()) {
					currentFilePath = paths.get(position);
					if (file.isDirectory()) {
						/* 如果是文件夹就再进去读取 */						
						getFileDir(currentFilePath);					
					} else {
						mPath.setText("当前路径:"+currentFilePath);
						((FileAdapter)fileListView.getAdapter()).setSelectedItem(position);
						for (int i = 0; i < av.getChildCount(); i++) {//循环设置每个item的背景色
                            if (av.getChildAt(i) == view) {//如果是当前选中的就设置为特俗的背景色
                            	view.setBackgroundColor(Color.rgb(54, 172,234));
                            }else {//其他的就设置为默认色
                            	av.getChildAt(i).setBackgroundColor(Color.rgb(239,239, 247));
                            }
						}				
						fileListView.invalidate();
						
					}
				} else {
					/* 弹出AlertDialog显示权限不足 */
					new AlertDialog.Builder(MainActivity.this)
							.setTitle("Message")
							.setMessage("权限不足!")
							.setPositiveButton("OK",
							new DialogInterface.OnClickListener() {
								public void onClick(
										DialogInterface dialog,
										int which) {
								}
							}).show();		
				}
			}

		});
		upgrade.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				
				//如果Bin文件校验正确，则进行TCP传送升级文件
				Log.i("zhou", parseBinFile(currentFilePath)+"");
				if(parseBinFile(currentFilePath) == 0){		
						//一切准备就绪后，推送固件升级文件					
						FirmwareBinControl.getInstance().sendFirmwareBinFile(mcontext,handler,currentFilePath);			
					}
					
				}

			
		});
		getFileDir(defaultPath);
	}
	
	/**
	 * 取得文件架构的method，供FileList使用
	 * 
	 * @param filePath
	 *            文件路径
	 */
	private void getFileDir(String filePath) {
		Log.w("getFileDir", "getFiles size:");
		
		/* 设定目前所在路径 */
		mPath.setText("当前路径:"+filePath);

		items = new ArrayList<String>();
		paths = new ArrayList<String>();
		File f = new File(filePath);
		File[] files = f.listFiles();

		if (!filePath.equals(rootPath)) {
			/* 第一笔设定为[回到根目录] */
			//items.add("[Back to " + rootPath + "]");
			items.add("[回到根目录]");
			paths.add(rootPath);
			/* 第二笔设定为[回上层] */
			//items.add("[Back to ../]");
			items.add("[↑向上]");
			paths.add(f.getParent());
		}
		
		if(files!=null)
		/* 将所有文件加入ArrayList中 */
		for (int i = 0; i < files.length; i++) {
			File file = files[i];
			if (file.isDirectory()) {
				items.add("[" + file.getName() + "]");
				paths.add(file.getPath());
			} else {
				if (Pattern.compile("([^\\*]+(\\.(?i)(Bin|bin))$)")
						.matcher(file.getName()).matches()) {
					items.add(file.getName());
					paths.add(file.getPath());
				}
			}

		}		
		FileAdapter fileList = new FileAdapter(this,R.layout.file_list_text, items);
		fileListView.setAdapter(fileList);

	}
	private int parseBinFile(String path){
		Log.w(tag, "currentFilePath = "+currentFilePath);
		 File file = new File(path);
		 long fileSize = file.length();
		 Log.i("zhou", file.getName().substring(file.getName().lastIndexOf(".")+1));
		 if(!file.getName().substring(file.getName().lastIndexOf(".")+1).equals("bin")){
			 Toast.makeText(this, "请选择文件后缀名为BIN的文件", Toast.LENGTH_SHORT).show();
			 return -1;
		 }
		 if(fileSize == 0 || fileSize > 200*1024){
			 Toast.makeText(this, "该升级文件大小有问题，请确认！", Toast.LENGTH_SHORT).show();
			 return -1;
		 }
		 InputStream in = null;
	        try {	        	 
	            //读文件头，一次读8个字节
	            byte[] headBytes = new byte[7];
	            int byteread = 0;
	            in = new FileInputStream(path);	  
	            if((byteread = in.read(headBytes)) != -1){
	            	String head = "";
	            	try {
	            		head = new String(headBytes ,"UTF-8");
	        		    	
	        		} catch (UnsupportedEncodingException e) {
	        			// TODO Auto-generated catch block
	        			e.printStackTrace();
	        		}   		           	
	            	Log.w(tag, "head of Bin : "+head);
	            	if(!head.equalsIgnoreCase(binHead)){
	            		Toast.makeText(this, "该升级固件的文件头有问题，请确认！", Toast.LENGTH_SHORT).show();
	       			 	return -1;
	            	}
	            }
	            
	            //读文件尾，一次读8个字节
	            in.skip(fileSize-15);
	            byte[] tailBytes = new byte[8];
	            if((byteread = in.read(tailBytes)) != -1){
	            	String tail = "";
	            	try {
	            		tail = new String(tailBytes ,"UTF-8");
	        		    	
	        		} catch (UnsupportedEncodingException e) {
	        			// TODO Auto-generated catch block
	        			e.printStackTrace();
	        		}   		           	
	            	Log.w(tag, "tail of Bin : "+tail);
	            	if(!tail.equalsIgnoreCase(binTail)){
	            		Toast.makeText(this, "该升级固件的文件尾有问题，请确认！", Toast.LENGTH_SHORT).show();
	       			 	return -1;
	            	}
	            }	            

	        } catch (Exception e1) {
	            e1.printStackTrace();
	        } finally {
	            if (in != null) {
	                try {
	                    in.close();
	                } catch (IOException e1) {
	                }
	            }
	        }   
	        return 0;
	}
	/** 
     * listview中点击按键弹出对话框 
     */  
    public final class ViewHolder {  
        public TextView titleText;  

    } 
	public class FileAdapter extends ArrayAdapter<String> {
		private LayoutInflater mInflater = null;
		private int selectedItem = -1;

		public FileAdapter(Context context, int textId, List<String> item) {
			super(context,textId,item);
			mInflater = LayoutInflater.from(MainActivity.this);

		}

		public void setSelectedItem(int selectedItem) {
			this.selectedItem = selectedItem;
		}

		/**
		 * @return the selectedItem
		 */
		public final int getSelectedItem() {
			return selectedItem;
		}

		// Get a View that displays the data at the specified position in the
		// data set.
		// 获取一个在数据集中指定索引的视图来显示数据
		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			//super.getView(position, convertView, parent);
			//Log.w("", "getView执行！！！！！");
			ViewHolder holder = null;			
			
			if (convertView == null) {
				holder = new ViewHolder();
				// 根据自定义的Item布局加载布局
				convertView = mInflater.inflate(R.layout.file_list_text, null);

				holder.titleText = (TextView) convertView.findViewById(R.id.file_text1);

				// 将设置好的布局保存到缓存中，并将其设置在Tag里，以便后面方便取出Tag
				convertView.setTag(holder);
			} else {
				holder = (ViewHolder) convertView.getTag();
			}
			holder.titleText.setTextColor(Color.BLACK);
			holder.titleText.setText(items.get(position));

			if (position == selectedItem) {
				holder.titleText.setBackgroundColor(Color.rgb(54, 172,234)); 

			} else {
				holder.titleText.setBackgroundColor(Color.rgb(239,239, 247)); 

			}						
			
			return convertView;
		}

	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		 if (keyCode == KeyEvent.KEYCODE_BACK )  
	        {  
			 // 创建退出对话框  
	            AlertDialog isExit = new AlertDialog.Builder(this).create();  
	            // 设置对话框标题  
	            isExit.setTitle("系统提示");  
	            // 设置对话框消息  
	            isExit.setMessage("确定要退出吗");  
	            // 添加选择按钮并注册监听  
	            isExit.setButton("确定", listener);  
	            isExit.setButton2("取消", listener);  
	            // 显示对话框  
	            isExit.show();  
	            
			 
	  
	        }  
		 return false;
	}
	  /**监听对话框里面的button点击事件*/  
    DialogInterface.OnClickListener listener = new DialogInterface.OnClickListener()  
    {  
        public void onClick(DialogInterface dialog, int which)  
        {  
            switch (which)  
            {  
            case AlertDialog.BUTTON_POSITIVE:// "确认"按钮退出程序  
                finish();  
                break;  
            case AlertDialog.BUTTON_NEGATIVE:// "取消"第二个按钮取消对话框  
                break;  
            default:  
                break;  
            }  
        }  
    };

}
