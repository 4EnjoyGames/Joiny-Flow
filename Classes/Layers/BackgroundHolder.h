#ifndef BACKGROUNDLAYER_H
#define BACKGROUNDLAYER_H
#include "cocos2d-A.h"




class BackgroundHolder
{
public:
    static void backgroundSwitchTo(cocos2d::CCNode*,
                                   cocos2d::CCCallFunc* back_button_callback=0,
                                   const bool play_music=true);
    static void storeOpen();
    static void storeClose();
    static void resetMusic();
private:
    static BackgroundHolder& getHolder();
    BackgroundHolder();
    ~BackgroundHolder();
    void do_backgroundSwitchTo(cocos2d::CCNode*,
                               cocos2d::CCCallFunc* back_button_callback,
                               const bool play_music);
    class BackgroundLayer : public cocos2d::CCLayer
    {
    public:
        BackgroundLayer();
        ~BackgroundLayer();
        virtual bool init();
        void setBackButtonCallback(cocos2d::CCCallFunc* back_button_callback);

        CREATE_FUNC(BackgroundLayer)
    private:
        void onBackClicked(CCObject*);
        void triggerAnimations();
        cocos2d::CCCallFunc* _back_button_callback;
        AnimatedMenuItem* _back_button;
        CCPoint _back_button_position;
        CCSprite* _animated_sprite;
		MenuSpriteBatch* _back_button_menu;
        friend class BackgroundHolder;
    };
    BackgroundLayer* _background_layer;
    bool _store_opened;
    static bool _firstly;

};

#endif // BACKGROUNDLAYER_H
