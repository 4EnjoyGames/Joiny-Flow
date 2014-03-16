#ifndef ANIMATEDMENUITEM_H
#define ANIMATEDMENUITEM_H

#include "cocos2d.h"
using namespace cocos2d;
/** @brief CCMenuItemSprite accepts CCNode<CCRGBAProtocol> objects as items.
 The images has 3 different states:
 - unselected image
 - selected image
 - disabled image

 @since v0.8.0
 */
class AnimatedMenuItem : public CCMenuItem
{
    /** the image used when the item is not selected */
    CC_PROPERTY(CCNode*, m_pNormalImage, NormalImage);
public:
    AnimatedMenuItem();
    ~AnimatedMenuItem();


    /** creates a menu item with a normal, selected and disabled image*/
    static AnimatedMenuItem * create(CCNode* normalSprite);
    /** creates a menu item with a normal and selected image with target/selector */
    static AnimatedMenuItem * create(CCNode* normalSprite, CCObject* target, SEL_MenuHandler selector);

    /** initializes a menu item with a normal, selected  and disabled image with target/selector */
    bool initWithNormalSprite(CCNode* normalSprite, CCObject* target, SEL_MenuHandler selector);
    // super methods
    virtual void setColor(const ccColor3B& color);
    virtual const ccColor3B& getColor();
    virtual void setOpacity(GLubyte opacity);
    virtual GLubyte getOpacity();
    virtual void addNephew(CCNode *node);
    /**
     @since v0.99.5
     */
    void setPosition(const CCPoint& newPosition);
    void setVisible(bool visible);
    void setScale(float scale);

    virtual void activate();
    virtual void selected();
    virtual void unselected();
    virtual void setEnabled(bool bEnabled);
    virtual void setBaseScale(float scale);
    virtual float getBaseScale();

    virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);}
    virtual bool isOpacityModifyRGB(void) { return false;}

    void updateDisplayedOpacity(GLubyte parentOpacity)
    {
        this->setOpacity(parentOpacity);
    }

protected:
    virtual void updateImagesVisibility();
private:
    void setScaleX(float scale);
    void setScaleY(float scale);

    void setOpacityToAllChildren(CCNode *target, GLubyte opacity);
    //static CCAction* _up_action;
    //static CCAction* _down_action;
    float _base_scale;
    CCArray* _nephews;
    CCAction* createUpAction();
    CCAction* createDownAction();
};
#endif // ANIMATEDMENUITEM_H
