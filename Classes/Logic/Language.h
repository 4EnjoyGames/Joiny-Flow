#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <string>
class Language
{
public:
    enum Languages
    {
        English=0,
        Russian=1
    };

    static void setLanguage(Languages lang);
    static Languages getLanguage();
    static const std::string& getLangCode();
    static std::string localizeFileName(const std::string& file_name);

    static Languages getDeviceLanguage();
private:
    static Languages _lang;
    static std::string _lang_code;

    Language();
};

#endif // LANGUAGE_H
