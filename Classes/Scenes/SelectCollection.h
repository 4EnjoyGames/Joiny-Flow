#ifndef SELECTCOLLECTION_H
#define SELECTCOLLECTION_H
#include "cocos2d-A.h"
#include "Core/Screen.h"
#include "Logic/JoinyCollection.h"
#include "BackButton.h"
#include "Core/DrawLayer.h"
class SelectCollection: public  DrawLayer//CCLayer
{
public:
    SelectCollection();
    static cocos2d::CCScene* scene();
    static SelectCollection* create();

    void onButtonBackClicked(CCObject*)
    {
        keyBackClicked();
    }

private:
    BackButton _back;
    CCMenu* _back_menu;
    void onCollectionSelect(CCObject* sender);
    AnimatedMenuItem* createCollectionItem(
            const JoinyCollection* collection, const SpritesLoader& spl);

    virtual bool init();

    void keyBackClicked();

    void onKeyBackClicked()
    {
        keyBackClicked();
    }
    void doGoBack();
    void doOpenCollection();

    void hideEverything(cocos2d::CCCallFunc *callback);
    void buildCollectionTiles();
    void newScrolling(MenuSpriteBatch* menu);

    typedef std::map<AnimatedMenuItem*, const JoinyCollection*> ButtonsMap;
    ButtonsMap _buttons_map;
    MenuSpriteBatch* _buttons_menu;
    SpritesLoader _col_spl;
    const JoinyCollection* _last_selected_collection;
};

#endif // SELECTCOLLECTION_H
