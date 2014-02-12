#ifndef FONTS_H
#define FONTS_H
#include <string>
#include "ADLib/Device/ADLanguage.h"

class Fonts
{
public:
    static const char* getFontName();
    /**
     * @brief Returns the name of BMFont for current language
     * @return Name of BMFont file
     */
    static const char* getBMFontName();
private:
    static char* _font_name;
    static char* _bm_font_name;
};

#endif // FONTS_H
