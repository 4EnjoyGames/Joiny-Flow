package com.x4enjoy;



import android.app.Activity;
import android.widget.Toast;

import com.x4enjoy.joiny.App;
import com.x4enjoy.joiny.R;

public class Notification {
	public static final int RESTORE_PURCHASES_FAILED = 200;
	public static final int RESTORE_PURCHASES_OK = 100;
	
	public static void notify(int code)
	{
		String message = "";
		if(code == RESTORE_PURCHASES_FAILED)
			message = App.getStringRes(R.string.restore_purchases_failed);
		else if(code == RESTORE_PURCHASES_OK)
			message = App.getStringRes(R.string.restore_purchases_ok);
		if(message.length() > 0)
		{
			final String text = message;
			final Activity main = App.getActivity();
			
			if(main != null)
			{
				main.runOnUiThread(new Runnable() {
				    public void run() {
				    	Toast.makeText(App.getAppContext(), text, Toast.LENGTH_LONG).show();
				    }
				});
			}
			
		}
	}
}
