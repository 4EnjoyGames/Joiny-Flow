#ifndef FONTS_H
#define FONTS_H
#include <string>
#include "ADLib/Device/ADLanguage.h"

class Fonts
{
public:
    static const char* getFontName();
private:
    static char* _font_name;

};

#endif // FONTS_H
