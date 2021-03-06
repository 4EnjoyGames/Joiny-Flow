#include "Fonts.h"

char* Fonts::_font_name = "";
char* Fonts::_bm_font_name = "";

const char* Fonts::getFontName()
{
    if(_font_name[0] == '\0')
    {
        std::string curr_language = ADLanguage::getLanguage();

        if(curr_language =="en" || curr_language =="hu" ||
                curr_language =="fr" || curr_language =="de" ||
                curr_language == "pt" || curr_language == "es" ||
                curr_language == "it" || curr_language == "pl" ||
                curr_language == "tr")
        {
            _font_name = "fonts/Fredoka One.ttf";
        }
        else
        {
            _font_name = "fonts/Lasco-Bold.ttf";
        }
    }

    return _font_name;
}

const char* Fonts::getBMFontName()
{
    if(_bm_font_name[0] == '\0')
    {
        std::string curr_language = ADLanguage::getLanguage();

        if(curr_language =="en" || curr_language =="hu" ||
                curr_language =="fr" || curr_language =="de" ||
                curr_language == "pt")
        {
            _bm_font_name = "fonts/Fredoka One.fnt";
        }
        else
        {
            _bm_font_name = "fonts/Lasco-Bold.fnt";
        }
    }

    return _bm_font_name;
}
