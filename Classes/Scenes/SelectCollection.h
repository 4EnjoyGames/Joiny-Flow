#ifndef SELECTCOLLECTION_H
#define SELECTCOLLECTION_H
#include "cocos2d-A.h"
#include "Core/Screen.h"
#include "Logic/JoinyCollection.h"

class SelectCollection: public  CCLayerColor
{
public:
    SelectCollection();
    static cocos2d::CCScene* scene();
    static SelectCollection* create();

private:
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
