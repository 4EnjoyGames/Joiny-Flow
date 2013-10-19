#ifndef CCMANUALFONT_H
#define CCMANUALFONT_H
#include "cocos2d.h"
#include "GraphicsManager.h"
class CCManualFont : public cocos2d::CCObject
{
public:
    static CCManualFont* create(cocos2d::CCNode* parent,
                                const char* font_plist,
                                const char* font_texture);
    cocos2d::CCSprite* getChar(const char ch);
private:
    CCManualFont(cocos2d::CCNode* parent,
                 const char* font_plist,
                 const char* font_texture);
    SpritesLoader _font;
};

inline void addCharToLabel(CCManualFont* font, float& left_padding, char ch, const cocos2d::ccColor3B& color)
{
    cocos2d::CCSprite* symbol = font->getChar(ch);
    symbol->setAnchorPoint(ccp(0,0));
    symbol->setPositionX(left_padding);
    symbol->setColor(color);
    left_padding += symbol->getContentSize().width;
}
cocos2d::CCNode *createLabel(const std::string& text,
                 const char* font_plist,
                 const char* font_texture,
                 const cocos2d::ccColor3B& color);

#endif // CCMANUALFONT_H
