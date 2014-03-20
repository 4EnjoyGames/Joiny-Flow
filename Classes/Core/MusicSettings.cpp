#include "MusicSettings.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
const char* MusicSettings::BACKGROUND_MUSIC = "music/silly_fun.wav";
const char* MusicSettings::LEVEL_SCENE_MUSIC = "music/demain_je_change.wav";
#else
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8
    const char* MusicSettings::BACKGROUND_MUSIC = "music/silly_fun.wav";
    const char* MusicSettings::LEVEL_SCENE_MUSIC = "music/demain_je_change.wav";
#else
	#ifdef CC_WIN8_METRO
    const char* MusicSettings::BACKGROUND_MUSIC = "music/silly_fun.mp3";
    const char* MusicSettings::LEVEL_SCENE_MUSIC = "music/demain_je_change.mp3";
#else
    #ifdef CC_TARGET_OS_IPHONE
    const char* MusicSettings::BACKGROUND_MUSIC = "music/silly_fun.mp3";
    const char* MusicSettings::LEVEL_SCENE_MUSIC = "music/demain_je_change.mp3";
	#else
    const char* MusicSettings::BACKGROUND_MUSIC = "music/silly_fun.ogg";
    const char* MusicSettings::LEVEL_SCENE_MUSIC = "music/demain_je_change.ogg";
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
