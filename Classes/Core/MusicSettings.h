#ifndef MUSICSETTINGS_H
#define MUSICSETTINGS_H
#include "cocos2d-A.h"
#include "SimpleAudioEngine.h"

class MusicSettings
{
public:
    static const char* BACKGROUND_MUSIC;
    static const char* LEVEL_SCENE_MUSIC;

    inline static const bool isMusicOn(void)
    {
        return _ms.do_isMusicOn();
    }
    inline static const bool isSoundEffectOn(void)
    {
        return _ms.do_isSoundEffectOn();
    }
    static void turnOnMusic(void)
    {
        return _ms.do_turnOnMusic();
    }
    static void turnOffMusic(void)
    {
        return _ms.do_turnOffMusic();
    }
    static void turnOnSoundEffect(void)
    {
        return _ms.do_turnOnSoundEffect();
    }
    static void turnOffSoundEffect(void)
    {
        return _ms.do_turnOffSoundEffect();
    }
    static void playSoundEffect(const char* str)
    {
        return _ms.do_playSoundEffect(str);
    }
    static void playMusic(const char* str)
    {
        return _ms.do_playMusic(str);
    }
    static void stopMusicForBackgrHolder(void)
    {
        return _ms.do_stopMusicForBackgrHolder();
    }
    static bool isBackgrHolderMusic(void)
    {
        return _ms.do_isBackgrHolderMusic();
    }

private:
    const bool do_isMusicOn(void)
    {
        return _music_on;
    }
    const bool do_isSoundEffectOn(void)
    {
        return _sound_effect_on;
    }
    void do_turnOnMusic(void)
    {
        _music_on=true;
        //        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic
        //                ("music/silly_fun.ogg", true);
    }
    void do_turnOffMusic(void)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        _music_on=false;
    }
    void do_turnOnSoundEffect(void)
    {
        //TODO: may be it needs changes
        //CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
        _sound_effect_on=true;
    }
    void do_turnOffSoundEffect(void)
    {
        //CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
        _sound_effect_on=false;
    }
    void do_playSoundEffect(const char* str)
    {
        if (_sound_effect_on)
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(str);
        }

    }
    void do_playMusic(const char* str)
    {
        if (_music_on)
        {
            if(_past_music_file_name!=str)
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic
                        (str, true);
            }
            else if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()==false)
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic
                        (str, true);
            }

            _past_music_file_name = str;
        }
    }
    void do_stopMusicForBackgrHolder(void)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        _backgrounholder_music_on=false;
    }
    bool do_isBackgrHolderMusic(void)
    {
        return _backgrounholder_music_on;
    }

    static MusicSettings _ms;
    MusicSettings();
    ~MusicSettings();
    bool _music_on;
    bool _sound_effect_on;

    bool _backgrounholder_music_on;

    const char* _past_music_file_name;
};


#endif // MUSICSETTINGS_H
