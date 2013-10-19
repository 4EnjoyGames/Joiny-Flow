package com.x4enjoy;
import java.util.HashMap;
import java.util.Map;

import com.flurry.android.FlurryAgent;

import android.util.Log;

public class Statistics {
	public static void statisticEvent(int id)
	{
		String action="Undifined Action";
		
		Log.d("statisticEvent", id+"");
		Map<String, String> map = new HashMap<String, String>();
		if (id==100)
			action="OnMoreLevelsClick";
		else if (id==200)
			action="OnDevelopersClick";
		else if (id==300)
			action="UseHint";
		else if (id==400)
			action="SkipLevel";
		else if (id==500)
			action="OnLevelComplete";
		
        FlurryAgent.logEvent(action, map);
	}
   public static void detailStatisticEvent(
		    int id,
		    int level_id,
            int collection_id,
            int stamps_num)
    {
	   String action="Undifined Action";
	   if (id==400)
			action="OnSkipLevel";
	   else if (id==500)
		   action="OnLevelComplete";
	   
	   Log.d(action,"levelID="+level_id+' '+
			   "collectionID="+collection_id+' '+
			   "stumpsNum="+stamps_num);
	   
	   Map<String, String> map = new HashMap<String, String>();
       map.put("levelID", String.valueOf(level_id));
       map.put("collectionID", String.valueOf(collection_id));
       map.put("stampsNum", String.valueOf(stamps_num));
       
       FlurryAgent.logEvent(action, map);
    }
}
