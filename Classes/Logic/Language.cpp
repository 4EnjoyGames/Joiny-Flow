#include "Language.h"

void Language::setLanguage(Languages lang)
{
    _lang = lang;

    if(_lang == Russian)
        _lang_code = "ru";
    else
        _lang_code = "en";
}

Language::Languages Language::getLanguage()
{
    return _lang;
}
const std::string& Language::getLangCode()
{
    return _lang_code;
}

std::string Language::localizeFileName(const std::string& file_name)
{

    unsigned int last_dot=0;
    for(unsigned int i=0; i<file_name.size(); ++i)
    {
        if(file_name[i] == '.')
            last_dot = i;
    }
    std::string res(file_name.begin(), file_name.begin()+last_dot+1);
    res.reserve(file_name.size() + 3);
    res += _lang_code;
    res += ".";
    res += &file_name[last_dot+1];
    return res;

}

Language::Languages Language::_lang = Language::Russian;
std::string Language::_lang_code = "ru";
