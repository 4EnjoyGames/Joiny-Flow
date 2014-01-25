#ifndef SELECTLEVEL_H
#define SELECTLEVEL_H
#include "Core/Screen.h"
#include "cocos2d-A.h"
#include "Logic/RW.h"
#include <vector>
#include "Layers/PopUpWindow.h"
#include "BackButton.h"
#include "Core/DrawLayer.h"
class SelectLevel : public DrawLayer //CCLayer//public cocos2d::CCLayer
{
public:


    SelectLevel(const JoinyCollection* collection);
    static cocos2d::CCScene* scene(const JoinyCollection* collection);
    static SelectLevel* create(const JoinyCollection* collection);

    void onButtonBackClicked(CCObject*)
    {
        keyBackClicked();
    }
private:
    BackButton _back;
    CCMenu* _back_menu;
    void onLevelSelect(CCObject* pSender);
    virtual bool init();

    void keyBackClicked();

    void onKeyBackClicked()
    {
        keyBackClicked();
    }
    void doGoBack();
    void doOpenLevel();

    void hideEverything(cocos2d::CCCallFunc *callback);
    void buildCollectionTiles();
    void newScrolling(MenuSpriteBatch* menu);

    AnimatedMenuItem* createLevelItem(const JoinyLevel* level,
                                      const SpritesLoader& spl);

    AnimatedMenuItem* createStars(AnimatedMenuItem* item,
                                  const JoinyLevel* level);



    typedef std::map<AnimatedMenuItem*, const JoinyLevel*> ButtonsMap;
    ButtonsMap _buttons_map;
    MenuSpriteBatch* _buttons_menu;
    SpritesLoader _col_spl;
    const JoinyLevel* _last_selected_level;

    const JoinyCollection* _current_collection;

    float _scale;
    SpritesLoader _spl;
};

#endif // SELECTLEVEL_H
