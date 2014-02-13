#ifndef SELECTCOLLECTION_H
#define SELECTCOLLECTION_H
#include "cocos2d-A.h"
#include "Core/Screen.h"
#include "Logic/JoinyCollection.h"
#include "BackButton.h"
#include "Core/DrawLayer.h"
#include "Layers/YesNoDialog.h"

class SelectCollection: public  DrawLayer
{
public:
    SelectCollection();
    static cocos2d::CCScene* scene();
    static SelectCollection* create();

    void onButtonBackClicked(CCObject*)
    {
        keyBackClicked();
    }

    static SelectCollection* _last_scene;
    static void purchaseUpdateFullGame();


private:
    SpritesLoader _stars_spl;

    class BuyFullVerdionPopUp;
    PopUpWindowManager _pop_up_manager;

    BackButton _back;
    CCMenu* _back_menu;

    CCSize _item_size;
    float _scale;

    void onCollectionSelect(CCObject* sender);
    static void updateItems();

    bool isFreeOpenFullGame();

    AnimatedMenuItem* createStars(AnimatedMenuItem* item,
                                  const JoinyCollection* collection);
    AnimatedMenuItem* createCollectionItem(
            const JoinyCollection* collection, const SpritesLoader& spl);

    virtual bool init();
    void onEnter();
    void onExit();

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
