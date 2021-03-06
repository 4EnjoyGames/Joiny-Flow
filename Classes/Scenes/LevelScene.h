#ifndef __LEVEL_SCENE_SCENE_H__
#define __LEVEL_SCENE_SCENE_H__

#include "cocos2d-A.h"
#include "Layers/YesNoDialog.h"
#include "Flow/FlowGame.h"
#include "Flow/JoinyPair.h"
#include "Logic/JoinyLevel.h"
#include "BackButton.h"
#include "Core/DrawLayer.h"
#include "Logic/Hints.h"
#include <ADLib/Device/ADAds.h>

class LevelScene : public DrawLayer //cocos2d::CCLayer
{
private:
    class RateMePopUp;
    class TesterEndPopUp;
    class BuyHintPopUp;
    class LevelEndPopUp;
    class FlowDelegate;


    CCSequence* createHintAction();
    CCSequence* _hint_action;
    AnimatedMenuItem* _hint_button;

    void doShowRateMe();
    bool showRateMe();
    void onNextLevel(const bool show_ads=true);


    Hints _hints;
    CCLabelTTF* _score_label;
    PopUpWindowManager _pop_up_manager;
    void hideEverything(cocos2d::CCCallFunc *callback);
    //use to back button clicked
    void hideEverythingAndBack(cocos2d::CCCallFunc *callback);

    //on back clicked
    void keyBackClicked();
    void onButtonBackClicked(CCObject*)
    {
        keyBackClicked();
    }

    //for renew hints after purchase
    static LevelScene* _last_scene;
    void renewOneHint();

    void hintBlink();

    void onScoreChanged(const FlowScore);
    void onWin();

    void doReloadLevel();
    void doGoToCollection();
    void doOpenNextLevel();
    void doOpenPreviousLevel();

    void onPreviousLevelClicked(CCObject*);
    void onNextLevelClicked(CCObject*);
    void onReloadLevelClicked(CCObject*);


    bool _show_open_animation;
    void showAnimation();

    FlowScore _last_score;
    FlowScore _bronze;
    FlowScore _silver;
    FlowScore _gold;
    FlowScore _max_score;

    JoinyInfo _current_info;
    FlowGame* _flow_game;

    const JoinyLevel* _current_level;
    const JoinyLevel* _next_level;
    const JoinyLevel* _previous_level;

    bool _showed_ads;
    bool _show_rate_me;

    CCProgressTimer* _progress_timer;
    float _procc;

    BackButton _back;

    SpritesLoader _spl;

    CCSprite* _bronze_star;
    CCPoint _bronze_position;
    CCSprite* _silver_star;
    CCPoint _silver_position;
    CCSprite* _gold_star;
    CCPoint _gold_position;

    MenuSpriteBatch* _buttons_menu;

    CCLabelTTF* _hint_number_text;

    CCMenu* _back_menu;

    //progress bar title
    CCSprite* _progress ;

    //buttons
    std::vector<AnimatedMenuItem*> _buttons;
    ADAds::Banner* _banner;
public:
    LevelScene(const JoinyLevel *l,
               bool show_animation,
               bool show_rate_me);

    static void purchaseUpdateHints();
    void onHintClicked(CCObject*);
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void onEnter();
    void onExit();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(const JoinyLevel *current_level,
                                   bool show_animation = false,
                                   bool show_rate_me = false);


    static LevelScene* create(const JoinyLevel *level,
                              bool show_animation,
                              bool show_rate_me);

    void onHintClickedHelper();

};


#endif // __LEVEL_SCENE_SCENE_H__
