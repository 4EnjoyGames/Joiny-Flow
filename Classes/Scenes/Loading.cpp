#include "Loading.h"
//#include "SelectLevel.h"
#include "MainScene.h"
#include "Core/CCTime.h"
#include "SimpleAudioEngine.h"
#include "../Core/MusicSettings.h"
#include <ADLib/Device/ADInApp.h>
#include "Logic/LevelManager.h"

Loading::Loading() : _sprites_to_load(0), _loaded_sprites(0)
{
}
CCScene* Loading::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    Loading *layer = Loading::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void Loading::drawEverything(float)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCSprite* logo = CCSprite::create("universal/x4Enjoy.png");
    this->addChild(logo);

    logo->setPosition(ccp(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height/2));

#ifdef CC_WIN8_METRO
	this->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Loading::loadAllAsync), this, 0.1, 0, 0, false);
#endif

}

void Loading::loadSprite(const char* name)
{
    _sprites_to_load++;

    CCTextureCache::sharedTextureCache()->
            addImageAsync(name, this, callfuncO_selector(Loading::loadingCallBack));
	
}
void Loading::hideLogo()
{
    this->runAction(CCCallFunc::create(this, callfunc_selector(Loading::openMenu)));
}

void Loading::openMenu()
{
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());   
}

void Loading::loadingCallBack(CCObject *)
{
    _loaded_sprites++;

    if(_loaded_sprites == _sprites_to_load)
    {
        const float splash_screen_time = 1;

        this->runAction(CCSequence::create(
                            CCDelayTime::create(splash_screen_time),
                            CCCallFunc::create(this, callfunc_selector(Loading::hideLogo)),
                            NULL));


    }
}
#include <ADLib/Device/ADInfo.h>
#include "Logic/RW.h"
#include "AppDelegate.h"
void Loading::initRW()
{
	//TODO: write cleanup
    //RW::onInit();
    /*if(ADInfo::getPlatform() != ADPlatform::Android)
        RW::onInit();*/

}
void Loading::addAll()
{
    addSpriteToLoad("banners/mif_kids.png");

    addSpriteToLoad("collection-menu/collection_button.png");
    addPlistToLoad("collection-menu/collection_button.plist");

    addSpriteToLoad("level-end/big_stars.png");
    addPlistToLoad("level-end/big_stars.plist");

    addSpriteToLoad("level-end/level_end.png");
    addPlistToLoad("level-end/level_end.plist");

    addSpriteToLoad("level-menu/level_buttons.png");
    addPlistToLoad("level-menu/level_buttons.plist");

    addSpriteToLoad("level-menu/save_stars.png");
    addPlistToLoad("level-menu/save_stars.plist");

    addSpriteToLoad("level-scene/flow.png");
    addPlistToLoad("level-scene/flow.plist");

    addSpriteToLoad("level-scene/level_stars.png");
    addPlistToLoad("level-scene/level_stars.plist");

    addSpriteToLoad("level-scene/level_novigation.png");
    addPlistToLoad("level-scene/level_novigation.plist");

    addSpriteToLoad("level-scene/line.png");
    addSpriteToLoad("level-scene/line_v.png");
    addSpriteToLoad("level-scene/progress_bar.png");
    addSpriteToLoad("level-scene/progress_bar_title.png");


    addSpriteToLoad("main-menu/facebook_logo.png");
    addSpriteToLoad("main-menu/main-logo.png");
    addSpriteToLoad("main-menu/play-button.png");
    addSpriteToLoad("main-menu/settings-button.png");

    addSpriteToLoad("pop-up/background.png");

    addSpriteToLoad("settings-menu/flag.png");
    addSpriteToLoad("settings-menu/music.png");
    addSpriteToLoad("settings-menu/node.png");
    addSpriteToLoad("settings-menu/settings_4Enjoy.png");
    addSpriteToLoad("settings-menu/sound.png");

    addSpriteToLoad("universal/back_button.png");
    addSpriteToLoad("universal/game_background.jpg");
    addSpriteToLoad("universal/white-background.png");
    addSpriteToLoad("universal/x4Enjoy.png");


    //RW::getLevelManager().loadSettings();
    ADInApp::loadStore(ADInApp::Mode::Production);

    //addSoundToLoad("music/silly_fun.mp3");
    addMusicToLoad(MusicSettings::BACKGROUND_MUSIC);
    addMusicToLoad(MusicSettings::LEVEL_SCENE_MUSIC);
    addSoundToLoad("music/choose.wav");
    addSoundToLoad("music/flow_click.wav");

    bool previev_mode = false;
    if(previev_mode)
        RW::getLevelManager().makePreviews(5,200);

//    const CollectionsArr& arr = RW::getCollections();
//    for(CollectionsArr::const_iterator it = arr.begin(); it != arr.end(); ++it)
//    {
//        Collection* a = it->second;
//        std::stringstream ss;
//        std::stringstream ss_f;
//        ss << "medium/select_collection/coll_" << a->getCollectionID() << ".png";
//        ss_f << "medium/select_collection/clabel_" << a->getCollectionID() << ".png";
//        addSpriteToLoad(ss.str().c_str());
//        addSpriteToLoad(Language::localizeFileName(ss_f.str()).c_str());
//    }
//    addSpriteToLoad("medium/select_collection/coll_lock.png");
//    addSpriteToLoad(Language::localizeFileName("medium/select_collection/clabel_locked.png").c_str());
}
void Loading::addSpriteToLoad(const char* sp)
{
	_q_sprites_to_load.push(sp);
}
void Loading::addMusicToLoad(const char* s)
{
	_q_music_to_load.push(s);
}
void Loading::addSoundToLoad(const char* s)
{
	_q_sound_to_load.push(s);
}
void Loading::addPlistToLoad(const char* s)
{
	_q_plists_to_load.push(s);
}
void Loading::loadAll()
{
	CCTextureCache* cache = CCTextureCache::sharedTextureCache();
	while(!_q_sprites_to_load.empty())
	{
		std::string str = _q_sprites_to_load.front();
		cache->addImage(str.c_str());
		_q_sprites_to_load.pop();
	}

	CocosDenshion::SimpleAudioEngine* engine = CocosDenshion::SimpleAudioEngine::sharedEngine();
	while(!_q_music_to_load.empty())
	{
		std::string str = _q_music_to_load.front();
		engine->preloadBackgroundMusic(str.c_str());
		_q_music_to_load.pop();
	}

	while(!_q_sound_to_load.empty())
	{
		std::string str = _q_sound_to_load.front();
		engine->preloadEffect(str.c_str());
		_q_sound_to_load.pop();
	}

	CCSpriteFrameCache* plist_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	while(!_q_plists_to_load.empty())
	{
		std::string str = _q_plists_to_load.front();
		plist_cache->addSpriteFramesWithFile(str.c_str());
		_q_plists_to_load.pop(); 
	}
}
void Loading::onLoadFinished()
{
	const float splash_screen_time = float(SPLASH_SCREEN_TIME);


    this->runAction(CCSequence::create(
                        CCDelayTime::create(splash_screen_time),
                        CCCallFunc::create(this, callfunc_selector(Loading::hideLogo)),
                        NULL));

}

#ifdef CC_WIN8_METRO
#include <ppltasks.h>
void Loading::loadAllAsync(float)
{
	using namespace concurrency;
	create_task(create_async([this](){
		this->addAll();
		this->loadAll();
	})).then([this](){
		this->onLoadFinished();
	});
}
#else
void Loading::loadAllAsync(float)
{
    addAll();
    loadAll();
    onLoadFinished();
}

#endif

bool Loading::init()
{
    if (!DrawLayer::init())
    {
        return false;
    }


    
//#ifndef CC_WIN8_PHONE
#ifdef CC_WIN8_METRO
	this->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Loading::drawEverything), this, 0.1, 0, 0, false);
	initRW();
#else
	drawEverything(0);
	initRW();
    this->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Loading::loadAllAsync), this, 0.1, 0, 0, false);
#endif
    return true;
}
