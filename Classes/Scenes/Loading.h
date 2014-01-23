#ifndef LOADING_H
#define LOADING_H
#include "cocos2d-A.h"
#include <queue>
#include <string>
//#include "../Core/CCMutex.h"
#include "Core/DrawLayer.h"
class Loading : public DrawLayer
{
private:
    static const unsigned int SPLASH_SCREEN_TIME = 1;
    unsigned int _sprites_to_load;
    unsigned int _loaded_sprites;
    int _started_in;
    //CCMutex _mutex;
    CCSprite* _logo;
    void loadSprite(const char*);
    void loadingCallBack(CCObject *obj);
    void hideLogo();
    void openMenu();
    void preloadSprites();
    void drawEverything(float);
	void initRW();
    //void loadingBackground(CCObject *obj);
	void loadMusicAsync();

	void addAll();
	void addSpriteToLoad(const char*);
	void addPlistToLoad(const char*);
	void addMusicToLoad(const char*);
	void addSoundToLoad(const char*);
    void loadAllAsync(float);
	void loadAll();
	void onLoadFinished();

	std::queue<std::string> _q_sprites_to_load;
	std::queue<std::string> _q_music_to_load;
	std::queue<std::string> _q_sound_to_load;
	std::queue<std::string> _q_plists_to_load;
public:
    Loading();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    CREATE_FUNC(Loading)
};

#endif // LOADING_H
