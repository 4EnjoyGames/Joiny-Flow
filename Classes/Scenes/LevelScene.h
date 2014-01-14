#ifndef __LEVEL_SCENE_SCENE_H__
#define __LEVEL_SCENE_SCENE_H__

#include "cocos2d-A.h"
#include "Layers/YesNoDialog.h"
#include "Flow/FlowGame.h"
#include "Flow/JoinyPair.h"
#include "Logic/JoinyLevel.h"
#include "BackButton.h"

class LevelScene : public CCLayerColor //cocos2d::CCLayer
{
private:
    class LevelEndPopUp;
    class FlowDelegate;


    void onNextLevel();


    CCLabelTTF* _score_label;
    PopUpWindowManager _pop_up_manager;
    void hideEverything(cocos2d::CCCallFunc *callback);

    //on back clicked
    void keyBackClicked();
    void onButtonBackClicked(CCObject*)
    {
        keyBackClicked();
    }


    void onScoreChanged(const FlowScore);
    void onWin();

    void doGoToCollection();
    void doOpenNextLevel();

    FlowScore _last_score;
    JoinyInfo _current_info;
    FlowGame* _flow_game;

    const JoinyLevel* _current_level;
    const JoinyLevel* _next_level;

    CCProgressTimer* _progress_timer;
    float _procc;

    BackButton _back;
public:
    LevelScene(const JoinyLevel*);


    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(const JoinyLevel * current_level);


    static LevelScene* create(const JoinyLevel* level);

};


#endif // __LEVEL_SCENE_SCENE_H__
