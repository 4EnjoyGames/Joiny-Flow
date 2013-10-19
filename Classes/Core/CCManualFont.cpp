#include "CCManualFont.h"

using namespace cocos2d;
CCManualFont::CCManualFont(CCNode *parent, const char* font_plist, const char *font_texture)
    : _font(GraphicsManager::getLoaderFor(parent, font_plist, font_texture))
{
    _font->inject(true);
}
CCManualFont* CCManualFont::create(CCNode* parent,
                                   const char* font_plist,
                                   const char *font_texture)
{
    CCAssert(parent!=0, "Parent should be defined");
    CCManualFont* obj = new CCManualFont(parent, font_plist, font_texture);
    obj->autorelease();
    return obj;

}
CCSprite* CCManualFont::getChar(const char ch)
{
    //TODO: this looks stupid
    switch(ch)
    {
    case '0': return _font->loadSprite("_0000s_0000_0_48.png");
    case '1': return _font->loadSprite("_0000s_0001_1_49.png");
    case '2': return _font->loadSprite("_0000s_0002_2_50.png");
    case '3': return _font->loadSprite("_0000s_0003_3_51.png");
    case '4': return _font->loadSprite("_0000s_0004_4_52.png");
    case '5': return _font->loadSprite("_0000s_0005_5_53.png");
    case '6': return _font->loadSprite("_0000s_0006_6_54.png");
    case '7': return _font->loadSprite("_0000s_0007_7_55.png");
    case '8': return _font->loadSprite("_0000s_0008_8_56.png");
    case '9': return _font->loadSprite("_0000s_0009_9_57.png");
    case '/': return _font->loadSprite("divide_47.png");
    case ' ': return _font->loadSprite("space_32.png");
    }
    return 0;
}
#include "cocos2d-A.h"
CCNode* createLabel(const std::string& text,
                 const char* font_plist,
                 const char* font_texture,
                 const cocos2d::ccColor3B& color)
{
    CCNode* node = CCNode::create();


    if(text.size()>0)
    {
        CCManualFont* font = CCManualFont::create(node,
                                                  font_plist,
                                                font_texture);
        CCSprite* ch = font->getChar(text[0]);
        CCSize ch_size = ch->getContentSize();
        ch->removeFromParent();

        float padding = 0;
        ccColor3B label_color(color);
        for(unsigned int i=0; i<text.size(); ++i)
        {
            addCharToLabel(font,padding,text[i], label_color);
        }
        node->setContentSize(CCSize(padding, ch_size.height));
    }
    return node;

}
