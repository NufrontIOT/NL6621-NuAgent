package com.nufront.nusmartconfig;

import android.app.TabActivity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.TabHost;

public class MainActivity extends TabActivity {
	private TabHost mTabhost;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mTabhost = getTabHost();
		mTabhost.addTab(mTabhost
				.newTabSpec("tab1")
				.setIndicator(
						getResources().getString(R.string.easymode),
						getResources().getDrawable(
								android.R.drawable.ic_menu_add))
				.setContent(new Intent(this, EasyMode.class)));
		mTabhost.addTab(mTabhost
				.newTabSpec("tab2")
				.setIndicator(
						getResources().getString(R.string.adhocmode),
						getResources()
								.getDrawable(android.R.drawable.ic_delete))
				.setContent(new Intent(this, AdhocMode.class)));
		mTabhost.setCurrentTab(0);
	}

}
