#include "AnimatedMenuItem.h"

//
//CCAnimatedMenuItem
//
const unsigned int    kNormalTag = 0x1;
const unsigned int    kSelectedTag = 0x2;
const unsigned int    kDisableTag = 0x3;
CCNode * AnimatedMenuItem::getNormalImage()
{
    return m_pNormalImage;
}
AnimatedMenuItem::AnimatedMenuItem()
    :m_pNormalImage(NULL), _base_scale(1), _nephews(NULL)
{

}

//CCAction* AnimatedMenuItem::_up_action = AnimatedMenuItem::createUpAction();
//CCAction* AnimatedMenuItem::_down_action = AnimatedMenuItem::createDownAction();
CCAction* AnimatedMenuItem::createUpAction()
{
    return CCScaleTo::create(0.05f, _base_scale*1.0f);
}

CCAction* AnimatedMenuItem::createDownAction()
{
    return CCScaleTo::create(0.05f, _base_scale*1.10f);
}

float AnimatedMenuItem::getBaseScale()
{
    return _base_scale;
}

void AnimatedMenuItem::addNephew(CCNode* node)
{
    CCAssert(node, "");
    if(_nephews == NULL)
    {
        _nephews = CCArray::createWithObject(node);
        _nephews->retain();
    }
    else
        _nephews->addObject(node);
    node->setScaleX(node->getScaleX() * this->getScaleX());
    node->setScaleY(node->getScaleY() * this->getScaleY());
    node->retain();
}

void AnimatedMenuItem::setPosition(const CCPoint& newPosition)
{
    if(this->getNormalImage())
        this->getNormalImage()->setPosition(newPosition);
    CCNode::setPosition(newPosition);


}
void AnimatedMenuItem::setVisible(bool visible)
{
    this->getNormalImage()->setVisible(visible);
    CCNode::setVisible(visible);
}
void AnimatedMenuItem::setScale(float scale)
{
    float scale_diff = scale / CCNode::getScale();
    this->getNormalImage()->setScale(this->getNormalImage()->getScale() * scale_diff);
    //this->getNormalImage()->setScale(scale);
    _base_scale = 1;
    CCNode::setScale(scale);


}
void AnimatedMenuItem::setScaleX(float scale)
{
    float scale_diff = scale / CCNode::getScaleX();


    if(_nephews)
    {


        CCObject* obj=0;
        CCARRAY_FOREACH(_nephews, obj)
        {
            CCNode* node = dynamic_cast<CCNode*>(obj);
            if(node)
                node->setScaleX(node->getScaleX() * scale_diff);
        }
    }

    this->getNormalImage()->setScaleX(scale);
    CCNode::setScaleX(scale);

}
void AnimatedMenuItem::setBaseScale(float scale)
{
    _base_scale = scale;
    CCNode::setScale(scale);
    if(m_pNormalImage)
        m_pNormalImage->setScale(scale);
}

void AnimatedMenuItem::setScaleY(float scale)
{
    float scale_diff = scale / CCNode::getScaleY();
    if(_nephews)
    {


        CCObject* obj=0;
        CCARRAY_FOREACH(_nephews, obj)
        {
            CCNode* node = dynamic_cast<CCNode*>(obj);
            if(node)
                node->setScaleY(node->getScaleY() * scale_diff);
        }
    }

    this->getNormalImage()->setScaleY(scale);
    CCNode::setScaleY(scale);
}

void AnimatedMenuItem::setNormalImage(CCNode* pImage)
{
    if (pImage != m_pNormalImage)
    {
        if (pImage)
        {
            //addChild(pImage, 0, kNormalTag);
            pImage->retain();
            //pImage->setAnchorPoint(ccp(0, 0));
        }

        if (m_pNormalImage)
        {
            m_pNormalImage->release();
            if(pImage)
                pImage->setScale(m_pNormalImage->getScale());
            m_pNormalImage = NULL;
            //removeChild(m_pNormalImage, true);
        }
        else
        {
            pImage->setScale(_base_scale);
        }

        m_pNormalImage = pImage;
        this->setContentSize(m_pNormalImage->getContentSize());
        this->updateImagesVisibility();
    }
    if(m_pNormalImage)
        m_pNormalImage->setPosition(this->getPosition());
}

AnimatedMenuItem::~AnimatedMenuItem()
{
    if (m_pNormalImage)
    {
        m_pNormalImage->release();
        m_pNormalImage = NULL;
        //removeChild(m_pNormalImage, true);
    }

    if(_nephews)
    {
        CCObject* obj=0;
        CCARRAY_FOREACH(_nephews, obj)
        {
            obj->retain();
        }
        _nephews->release();
    }
}


//
//CCAnimatedMenuItem - CCRGBAProtocol protocol
//
void AnimatedMenuItem::setOpacityToAllChildren(CCNode* target, GLubyte opacity)
{
    CCObject* obj=0;
    CCARRAY_FOREACH(target->getChildren(), obj)
    {
        CCRGBAProtocol* node = dynamic_cast<CCRGBAProtocol*>(obj);
        if(node)
            node->setOpacity(opacity);
        else
        {
            CCNode* node = dynamic_cast<CCNode*>(obj);
            if(node)
                setOpacityToAllChildren(node, opacity);
        }
    }
}

void AnimatedMenuItem::setOpacity(GLubyte opacity)
{
    dynamic_cast<CCRGBAProtocol*>(m_pNormalImage)->setOpacity(opacity);
    CCObject* obj=0;
    CCARRAY_FOREACH(_nephews, obj)
    {
        CCRGBAProtocol* node = dynamic_cast<CCRGBAProtocol*>(obj);
        if(node)
            node->setOpacity(opacity);
    }
    setOpacityToAllChildren(this, opacity);
}

void AnimatedMenuItem::setColor(const ccColor3B& color)
{
    dynamic_cast<CCRGBAProtocol*>(m_pNormalImage)->setColor(color);


}

GLubyte AnimatedMenuItem::getOpacity()
{
    return dynamic_cast<CCRGBAProtocol*>(m_pNormalImage)->getOpacity();
}

const ccColor3B &AnimatedMenuItem::getColor()
{
    return dynamic_cast<CCRGBAProtocol*>(m_pNormalImage)->getColor();
}



AnimatedMenuItem * AnimatedMenuItem::create(CCNode* normalSprite)
{
    return AnimatedMenuItem::create(normalSprite, NULL, NULL);
}


AnimatedMenuItem * AnimatedMenuItem::create(CCNode *normalSprite, CCObject *target, SEL_MenuHandler selector)
{
    AnimatedMenuItem *pRet = new AnimatedMenuItem();
    pRet->initWithNormalSprite(normalSprite, target, selector);
    pRet->autorelease();
    return pRet;
}

bool AnimatedMenuItem::initWithNormalSprite(CCNode* normalSprite, CCObject* target, SEL_MenuHandler selector)
{
    CCMenuItem::initWithTarget(target, selector);
    setNormalImage(normalSprite);

    if(m_pNormalImage)
    {
        this->setContentSize(m_pNormalImage->getContentSize());
    }
    return true;
}

/**
 @since v0.99.5
 */
void AnimatedMenuItem::activate()
{
    //TODO: place music effect on click
    //MusicSettings::playSoundEffect("music/choose.wav");
    CCMenuItem::activate();
}

void AnimatedMenuItem::selected()
{

    if (m_pNormalImage)
    {
        /*if (m_pDisabledImage)
        {
            m_pDisabledImage->setVisible(false);
        }

        if (m_pSelectedImage)
        {
            m_pNormalImage->setVisible(false);
            m_pSelectedImage->setVisible(true);
        }
        else
        {*/
        //m_pNormalImage->setVisible(true);
        /*if(m_pNormalImage->numberOfRunningActions() > 0)
            m_pNormalImage->stopAllActions();
        m_pNormalImage->runAction(createDownAction());*/
        if(this->numberOfRunningActions() > 0)
            this->stopAllActions();
        this->runAction(createDownAction());
        //}
    }
    CCMenuItem::selected();

}

void AnimatedMenuItem::unselected()
{
    if (m_pNormalImage)
    {
        //m_pNormalImage->setVisible(true);

        /*if (m_pSelectedImage)
        {
            m_pSelectedImage->setVisible(false);
        }

        if (m_pDisabledImage)
        {
            m_pDisabledImage->setVisible(false);
        }*/
        //        if(m_pNormalImage->numberOfRunningActions() > 0)
        //            m_pNormalImage->stopAllActions();
        //        m_pNormalImage->runAction(createUpAction());
        if(this->numberOfRunningActions() > 0)
            this->stopAllActions();
        this->runAction(createUpAction());
    }
    CCMenuItem::unselected();
}


void AnimatedMenuItem::setEnabled(bool bEnabled)
{
    if( m_bEnabled != bEnabled )
    {
        CCMenuItem::setEnabled(bEnabled);
        this->updateImagesVisibility();
    }
}

// Helper
void AnimatedMenuItem::updateImagesVisibility()
{
    if (m_bEnabled)
    {
        if (m_pNormalImage)   m_pNormalImage->setVisible(true);
        //if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
        //if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
    }
    /*else
    {
        if (m_pDisabledImage)
        {
            if (m_pNormalImage)   m_pNormalImage->setVisible(false);
            if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
            if (m_pDisabledImage) m_pDisabledImage->setVisible(true);
        }
        else
        {
            if (m_pNormalImage)   m_pNormalImage->setVisible(true);
            if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
            if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
        }
    }*/
}
