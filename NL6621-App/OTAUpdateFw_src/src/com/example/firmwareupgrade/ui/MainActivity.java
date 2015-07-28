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
	 * �������� items�������ʾ������ paths������ļ�·�� rootPath����ʼĿ¼
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
							"��ʼ�����̼������Ժ�......",true,false);
					
					break;
				case UPGRADE_IN_PROCESS:
					int percent = msg.arg1;
					upgradeDialog.setMessage("���ڹ̼�����,��ǰ�����"+percent+"%");
					break;
				case FIRMMARE_FAILURE:
					upgrade.setEnabled(true);
					Toast.makeText(MainActivity.this, "�����̼�ʧ�ܣ�", Toast.LENGTH_LONG).show();
					FirmwareBinControl.lock2 = false;
					upgradeDialog.dismiss();
					
					break;
				case FIRMMARE_SUCCESS:
					upgrade.setEnabled(true);
					Toast.makeText(MainActivity.this, "�����̼��ɹ���", Toast.LENGTH_LONG).show();
					FirmwareBinControl.lock2 = false;
					upgradeDialog.dismiss();
					
					break;
				case GETIDSUCESS:
					Toast.makeText(MainActivity.this, "��ʼ�����豸", Toast.LENGTH_LONG).show();
					upgrade.setEnabled(false);
					break;
				case NOTConTOSevice:
					Toast.makeText(MainActivity.this, "û���������豸", Toast.LENGTH_LONG).show();
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
				Log.w("onItemClick", "onItemClickִ��");
				File file = new File(paths.get(position));
				if (file.canRead()) {
					currentFilePath = paths.get(position);
					if (file.isDirectory()) {
						/* ������ļ��о��ٽ�ȥ��ȡ */						
						getFileDir(currentFilePath);					
					} else {
						mPath.setText("��ǰ·��:"+currentFilePath);
						((FileAdapter)fileListView.getAdapter()).setSelectedItem(position);
						for (int i = 0; i < av.getChildCount(); i++) {//ѭ������ÿ��item�ı���ɫ
                            if (av.getChildAt(i) == view) {//����ǵ�ǰѡ�еľ�����Ϊ���׵ı���ɫ
                            	view.setBackgroundColor(Color.rgb(54, 172,234));
                            }else {//�����ľ�����ΪĬ��ɫ
                            	av.getChildAt(i).setBackgroundColor(Color.rgb(239,239, 247));
                            }
						}				
						fileListView.invalidate();
						
					}
				} else {
					/* ����AlertDialog��ʾȨ�޲��� */
					new AlertDialog.Builder(MainActivity.this)
							.setTitle("Message")
							.setMessage("Ȩ�޲���!")
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
				
				//���Bin�ļ�У����ȷ�������TCP���������ļ�
				Log.i("zhou", parseBinFile(currentFilePath)+"");
				if(parseBinFile(currentFilePath) == 0){		
						//һ��׼�����������͹̼������ļ�					
						FirmwareBinControl.getInstance().sendFirmwareBinFile(mcontext,handler,currentFilePath);			
					}
					
				}

			
		});
		getFileDir(defaultPath);
	}
	
	/**
	 * ȡ���ļ��ܹ���method����FileListʹ��
	 * 
	 * @param filePath
	 *            �ļ�·��
	 */
	private void getFileDir(String filePath) {
		Log.w("getFileDir", "getFiles size:");
		
		/* �趨Ŀǰ����·�� */
		mPath.setText("��ǰ·��:"+filePath);

		items = new ArrayList<String>();
		paths = new ArrayList<String>();
		File f = new File(filePath);
		File[] files = f.listFiles();

		if (!filePath.equals(rootPath)) {
			/* ��һ���趨Ϊ[�ص���Ŀ¼] */
			//items.add("[Back to " + rootPath + "]");
			items.add("[�ص���Ŀ¼]");
			paths.add(rootPath);
			/* �ڶ����趨Ϊ[���ϲ�] */
			//items.add("[Back to ../]");
			items.add("[������]");
			paths.add(f.getParent());
		}
		
		if(files!=null)
		/* �������ļ�����ArrayList�� */
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
			 Toast.makeText(this, "��ѡ���ļ���׺��ΪBIN���ļ�", Toast.LENGTH_SHORT).show();
			 return -1;
		 }
		 if(fileSize == 0 || fileSize > 200*1024){
			 Toast.makeText(this, "�������ļ���С�����⣬��ȷ�ϣ�", Toast.LENGTH_SHORT).show();
			 return -1;
		 }
		 InputStream in = null;
	        try {	        	 
	            //���ļ�ͷ��һ�ζ�8���ֽ�
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
	            		Toast.makeText(this, "�������̼����ļ�ͷ�����⣬��ȷ�ϣ�", Toast.LENGTH_SHORT).show();
	       			 	return -1;
	            	}
	            }
	            
	            //���ļ�β��һ�ζ�8���ֽ�
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
	            		Toast.makeText(this, "�������̼����ļ�β�����⣬��ȷ�ϣ�", Toast.LENGTH_SHORT).show();
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
     * listview�е�����������Ի��� 
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
		// ��ȡһ�������ݼ���ָ����������ͼ����ʾ����
		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			//super.getView(position, convertView, parent);
			//Log.w("", "getViewִ�У���������");
			ViewHolder holder = null;			
			
			if (convertView == null) {
				holder = new ViewHolder();
				// �����Զ����Item���ּ��ز���
				convertView = mInflater.inflate(R.layout.file_list_text, null);

				holder.titleText = (TextView) convertView.findViewById(R.id.file_text1);

				// �����úõĲ��ֱ��浽�����У�������������Tag��Ա���淽��ȡ��Tag
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
			 // �����˳��Ի���  
	            AlertDialog isExit = new AlertDialog.Builder(this).create();  
	            // ���öԻ������  
	            isExit.setTitle("ϵͳ��ʾ");  
	            // ���öԻ�����Ϣ  
	            isExit.setMessage("ȷ��Ҫ�˳���");  
	            // ���ѡ��ť��ע�����  
	            isExit.setButton("ȷ��", listener);  
	            isExit.setButton2("ȡ��", listener);  
	            // ��ʾ�Ի���  
	            isExit.show();  
	            
			 
	  
	        }  
		 return false;
	}
	  /**�����Ի��������button����¼�*/  
    DialogInterface.OnClickListener listener = new DialogInterface.OnClickListener()  
    {  
        public void onClick(DialogInterface dialog, int which)  
        {  
            switch (which)  
            {  
            case AlertDialog.BUTTON_POSITIVE:// "ȷ��"��ť�˳�����  
                finish();  
                break;  
            case AlertDialog.BUTTON_NEGATIVE:// "ȡ��"�ڶ�����ťȡ���Ի���  
                break;  
            default:  
                break;  
            }  
        }  
    };

}
