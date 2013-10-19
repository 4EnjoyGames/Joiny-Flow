#include "MusicSettings.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
const char* MusicSettings::BACKGROUND_MUSIC = "music/math_is_fun.wav";
#else
#ifdef CC_WIN8_PHONE
	const char* MusicSettings::BACKGROUND_MUSIC = "music/math_is_fun.wav";
#else
	#ifdef CC_WIN8_METRO
	const char* MusicSettings::BACKGROUND_MUSIC = "music/math_is_fun.mp3";
#else
    #ifdef CC_TARGET_OS_IPHONE
    const char* MusicSettings::BACKGROUND_MUSIC = "music/math_is_fun.mp3";
	#else
	const char* MusicSettings::BACKGROUND_MUSIC = "music/math_is_fun.ogg";
	#endif
    #endif
#endif
#endif

MusicSettings MusicSettings::_ms;

MusicSettings::MusicSettings(): _music_on(true), _sound_effect_on(true),
                                _backgrounholder_music_on(true)
{
}
MusicSettings::~MusicSettings()
{
}
