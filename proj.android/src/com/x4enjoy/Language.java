package com.x4enjoy;

import java.util.Locale;

public class Language {

	public static final int EN = 0;
	public static final int RU = 1;
	
	public static final int getLanguage()
	{
		String lang = Locale.getDefault().getLanguage();
		if("uk".equals(lang))
			return RU;
		if("ru".equals(lang))
			return RU;
		return EN;
	}
}
