package com.x4enjoy;

import org.apache.http.protocol.HTTP;

import com.x4enjoy.joiny.App;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;

public class Browser {
	public static void openURL(int id)
	{
		/*String action="Undefined Action";
		if (id==100)
			action="facebookGroup";
		else if (id==200)
			action="site4Enjoy";
		else if (id==300)
			action="sendEmail";
		
		Log.d("browser", action);*/
		
		
			
		
		if(id==300)
		{	
			
			String mailTo="feedback@4enjoy.com";
	        
	        Intent i = new Intent(Intent.ACTION_SEND);
	        i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	        i.setType("message/rfc822");
	        i.putExtra(Intent.EXTRA_EMAIL  , new String[]{mailTo});
	        i.putExtra(Intent.EXTRA_SUBJECT, "Math Is Fun Android");
	        i.putExtra(Intent.EXTRA_TEXT   , "");
	        try {
	        	App.getAppContext().startActivity(i);
	        	//App.getAppContext().startActivity(Intent.createChooser(i, "Send mail..."));
	        } catch (android.content.ActivityNotFoundException ex) {
	           Log.d("Browser", "ERRROR");
	        }

			
		}
		else
		{
			Intent browserIntent = new Intent(Intent.ACTION_VIEW, 
					Uri.parse("http://4enjoy.com/r/android/"+id+'/'));
					
			browserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			App.getAppContext().startActivity(browserIntent);
		}
	}
}
