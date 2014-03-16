/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2010 Sangwoo Im
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CCScrollView.h"
#include "cocos2d.h"
#ifdef CC_WIN8_METRO
#include "CCEGLView.h"
#include "DirectXRender.h"
using namespace DirectX;
#endif
using namespace cocos2d;

#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.15f
#define INSET_RATIO          0.2f


CCScrollView::CCScrollView()
: m_fZoomScale(0.0f)
, m_fMinZoomScale(0.0f)
, m_fMaxZoomScale(0.0f)
, m_pDelegate(NULL)
, m_bDragging(false)
, m_bBounceable(false)
, m_eDirection(kCCScrollViewDirectionBoth)
, m_bClippingToBounds(false)
, m_pContainer(NULL)
, m_bTouchMoved(false)
, m_fTouchLength(0.0f)
, m_pTouches(NULL)
, m_pRedirectToMenu(NULL)
, m_fMinScale(0.0f)
, m_fMaxScale(0.0f)
, _menu(0)
{

}

CCScrollView::~CCScrollView()
{
    m_pTouches->release();
    m_pRedirectToMenu->release();
}

CCScrollView* CCScrollView::viewWithViewSize(CCSize size, CCNode* container/* = NULL*/)
{
    return CCScrollView::create(size, container);
}

CCScrollView* CCScrollView::create(CCSize size, CCNode* container/* = NULL*/)
{
    CCScrollView* pRet = new CCScrollView();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCScrollView* CCScrollView::node()
{
    return CCScrollView::create();
}

CCScrollView* CCScrollView::create()
{
    CCScrollView* pRet = new CCScrollView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}


bool CCScrollView::initWithViewSize(CCSize size, CCNode *container/* = NULL*/)
{
    if (CCLayer::init())
    {
        m_pContainer = container;
        
        if (!this->m_pContainer)
        {
            m_pContainer = CCLayer::create();
            this->m_pContainer->ignoreAnchorPointForPosition(false);
            this->m_pContainer->setAnchorPoint(ccp(0.0f, 0.0f));
        }

        this->setViewSize(size);

        setTouchEnabled(true);
        m_pTouches = new CCArray();
        m_pRedirectToMenu = new CCArray();
        m_pDelegate = NULL;
        m_bBounceable = true;
        m_bClippingToBounds = true;
        //m_pContainer->setContentSize(CCSizeZero);
        m_eDirection  = kCCScrollViewDirectionBoth;
        m_pContainer->setPosition(ccp(0.0f, 0.0f));
        m_fTouchLength = 0.0f;
        
        this->addChild(m_pContainer);
        m_fMinScale = m_fMaxScale = 1.0f;
        return true;
    }
    return false;
}

bool CCScrollView::init()
{
    return this->initWithViewSize(CCSizeMake(200, 200), NULL);
}

void CCScrollView::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
}

bool CCScrollView::isNodeVisible(CCNode* node)
{
    const CCPoint offset = this->getContentOffset();
    const CCSize  size   = this->getViewSize();
    const float   scale  = this->getZoomScale();
    
    CCRect viewRect;
    
    viewRect = CCRectMake(-offset.x/scale, -offset.y/scale, size.width/scale, size.height/scale); 
    
    return viewRect.intersectsRect(node->boundingBox());
}

void CCScrollView::pause(CCObject*)
{
    m_pContainer->pauseSchedulerAndActions();

    CCObject* pObj = NULL;
    CCArray* pChildren = m_pContainer->getChildren();

    CCARRAY_FOREACH(pChildren, pObj)
    {
        CCNode* pChild = (CCNode*)pObj;
        pChild->pauseSchedulerAndActions();
    }
}

void CCScrollView::resume(CCObject*)
{
    CCObject* pObj = NULL;
    CCArray* pChildren = m_pContainer->getChildren();

    CCARRAY_FOREACH(pChildren, pObj)
    {
        CCNode* pChild = (CCNode*)pObj;
        pChild->resumeSchedulerAndActions();
    }

    m_pContainer->resumeSchedulerAndActions();
}

void CCScrollView::setTouchEnabled(bool e)
{
    CCLayer::setTouchEnabled(e);
    if (!e)
    {
        m_bDragging = false;
        m_bTouchMoved = false;
        m_pTouches->removeAllObjects();
        m_pRedirectToMenu->removeAllObjects();
    }
}

void CCScrollView::setContentOffset(CCPoint offset, bool animated/* = false*/)
{
    if (animated)
    { //animate scrolling
        this->setContentOffsetInDuration(offset, BOUNCE_DURATION);
    } 
    else
    { //set the container position directly
        if (!m_bBounceable)
        {
            const CCPoint minOffset = this->minContainerOffset();
            const CCPoint maxOffset = this->maxContainerOffset();
            
            offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
            offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
        }

        m_pContainer->setPosition(offset);

        if (m_pDelegate != NULL)
        {
            m_pDelegate->scrollViewDidScroll(this);   
        }
    }
}

void CCScrollView::setContentOffsetInDuration(CCPoint offset, float dt)
{
    CCFiniteTimeAction *scroll, *expire;
    
    scroll = CCMoveTo::create(dt, offset);
    expire = CCCallFuncN::create(this, callfuncN_selector(CCScrollView::stoppedAnimatedScroll));
    m_pContainer->runAction(CCSequence::create(scroll, expire, NULL));
    this->schedule(schedule_selector(CCScrollView::performedAnimatedScroll));
}

CCPoint CCScrollView::getContentOffset()
{
    return m_pContainer->getPosition();
}

void CCScrollView::setZoomScale(float s)
{
    if (m_pContainer->getScale() != s)
    {
        CCPoint oldCenter, newCenter;
        CCPoint center;
        
        if (m_fTouchLength == 0.0f) 
        {
            center = ccp(m_tViewSize.width*0.5f, m_tViewSize.height*0.5f);
            center = this->convertToWorldSpace(center);
        }
        else
        {
            center = m_tTouchPoint;
        }
        
        oldCenter = m_pContainer->convertToNodeSpace(center);
        m_pContainer->setScale(MAX(m_fMinScale, MIN(m_fMaxScale, s)));
        newCenter = m_pContainer->convertToWorldSpace(oldCenter);
        
        const CCPoint offset = ccpSub(center, newCenter);
        if (m_pDelegate != NULL)
        {
            m_pDelegate->scrollViewDidZoom(this);
        }
        this->setContentOffset(ccpAdd(m_pContainer->getPosition(),offset));
    }
}

float CCScrollView::getZoomScale()
{
    return m_pContainer->getScale();
}

void CCScrollView::setZoomScale(float s, bool animated)
{
    if (animated)
    {
        this->setZoomScaleInDuration(s, BOUNCE_DURATION);
    }
    else
    {
        this->setZoomScale(s);
    }
}

void CCScrollView::setZoomScaleInDuration(float s, float dt)
{
#ifndef CC_WIN8_METRO
    if (dt > 0)
    {
        if (m_pContainer->getScale() != s)
        {
            CCActionTween *scaleAction;
            scaleAction = CCActionTween::create(dt, "zoomScale", m_pContainer->getScale(), s);
            this->runAction(scaleAction);
        }
    }
    else
    {
        this->setZoomScale(s);
    }
#endif
}

void CCScrollView::setViewSize(CCSize size)
{
    m_tViewSize = size;
    CCLayer::setContentSize(size);
}

CCNode * CCScrollView::getContainer()
{
    return this->m_pContainer;
}

void CCScrollView::setContainer(CCNode * pContainer)
{
    this->removeAllChildrenWithCleanup(true);

    if (!pContainer) return;

    this->m_pContainer = pContainer;

    this->m_pContainer->ignoreAnchorPointForPosition(false);
    this->m_pContainer->setAnchorPoint(ccp(0.0f, 0.0f));

    this->addChild(this->m_pContainer);

    this->setViewSize(this->m_tViewSize);
}

void CCScrollView::relocateContainer(bool animated)
{
    CCPoint oldPoint, min, max;
    float newX, newY;
    
    min = this->minContainerOffset();
    max = this->maxContainerOffset();
    
    oldPoint = m_pContainer->getPosition();

    newX     = oldPoint.x;
    newY     = oldPoint.y;
    if (m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionHorizontal)
    {
        newX     = MIN(newX, max.x);
        newX     = MAX(newX, min.x);
    }

    if (m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionVertical)
    {
        newY     = MIN(newY, max.y);
        newY     = MAX(newY, min.y);
    }

    if (newY != oldPoint.y || newX != oldPoint.x)
    {
        this->setContentOffset(ccp(newX, newY), animated);
    }
}

CCPoint CCScrollView::maxContainerOffset()
{
    return ccp(0.0f, 0.0f);
}

CCPoint CCScrollView::minContainerOffset()
{
    return ccp(m_tViewSize.width - m_pContainer->getContentSize().width*m_pContainer->getScaleX(), 
               m_tViewSize.height - m_pContainer->getContentSize().height*m_pContainer->getScaleY());
}

void CCScrollView::deaccelerateScrolling(float)
{
    if (m_bDragging)
    {
        this->unschedule(schedule_selector(CCScrollView::deaccelerateScrolling));
        return;
    }
    
    float newX, newY;
    CCPoint maxInset, minInset;
    
    m_pContainer->setPosition(ccpAdd(m_pContainer->getPosition(), m_tScrollDistance));
    
    if (m_bBounceable)
    {
        maxInset = m_fMaxInset;
        minInset = m_fMinInset;
    }
    else
    {
        maxInset = this->maxContainerOffset();
        minInset = this->minContainerOffset();
    }
    
    //check to see if offset lies within the inset bounds
    newX     = MIN(m_pContainer->getPosition().x, maxInset.x);
    newX     = MAX(newX, minInset.x);
    newY     = MIN(m_pContainer->getPosition().y, maxInset.y);
    newY     = MAX(newY, minInset.y);
    
    m_tScrollDistance     = ccpSub(m_tScrollDistance, ccp(newX - m_pContainer->getPosition().x, newY - m_pContainer->getPosition().y));
    m_tScrollDistance     = ccpMult(m_tScrollDistance, SCROLL_DEACCEL_RATE);
    this->setContentOffset(ccp(newX,newY));
    
    if ((fabsf(m_tScrollDistance.x) <= SCROLL_DEACCEL_DIST &&
         fabsf(m_tScrollDistance.y) <= SCROLL_DEACCEL_DIST) ||
        newX == maxInset.x || newX == minInset.x ||
        newY == maxInset.y || newY == minInset.y)
    {
        this->unschedule(schedule_selector(CCScrollView::deaccelerateScrolling));
        this->relocateContainer(true);
    }
}

void CCScrollView::stoppedAnimatedScroll(CCNode *)
{
    this->unschedule(schedule_selector(CCScrollView::performedAnimatedScroll));
}

void CCScrollView::performedAnimatedScroll(float)
{
    if (m_bDragging)
    {
        this->unschedule(schedule_selector(CCScrollView::performedAnimatedScroll));
        return;
    }

    if (m_pDelegate != NULL)
    {
        m_pDelegate->scrollViewDidScroll(this);
    }
}


CCSize CCScrollView::getContentSize()
{
	return m_pContainer->getContentSize();
}

void CCScrollView::setContentSize(const CCSize & size)
{
    if (this->getContainer() != NULL)
    {
        this->getContainer()->setContentSize(size);
		this->updateInset();
    }
}

void CCScrollView::updateInset()
{
	if (this->getContainer() != NULL)
	{
		m_fMaxInset = this->maxContainerOffset();
		m_fMaxInset = ccp(m_fMaxInset.x + m_tViewSize.width * INSET_RATIO,
			m_fMaxInset.y + m_tViewSize.height * INSET_RATIO);
		m_fMinInset = this->minContainerOffset();
		m_fMinInset = ccp(m_fMinInset.x - m_tViewSize.width * INSET_RATIO,
			m_fMinInset.y - m_tViewSize.height * INSET_RATIO);
	}
}

/**
 * make sure all children go to the container
 */
void CCScrollView::addChild(CCNode * child, int zOrder, int tag)
{
    child->ignoreAnchorPointForPosition(false);
    child->setAnchorPoint(ccp(0.0f, 0.0f));
    if (m_pContainer != child) {
        m_pContainer->addChild(child, zOrder, tag);
    } else {
        CCLayer::addChild(child, zOrder, tag);
    }
}

void CCScrollView::addChild(CCNode * child, int zOrder)
{
    this->addChild(child, zOrder, child->getTag());
}

void CCScrollView::addChild(CCNode * child)
{
    this->addChild(child, child->getZOrder(), child->getTag());
}

/**
 * clip this view so that outside of the visible bounds can be hidden.
 */


void CCScrollView::beforeDraw()
{
    if (m_bClippingToBounds)
    {
		// TODO: This scrollview should respect parents' positions
		CCPoint screenPos = this->getParent()->convertToWorldSpace(this->getPosition());
        glEnable(GL_SCISSOR_TEST);
        float s = this->getScale();

//        CCDirector *director = CCDirector::sharedDirector();
//        s *= director->getContentScaleFactor();
        float x = CCEGLView::sharedOpenGLView()->getScaleX();
        float y = CCEGLView::sharedOpenGLView()->getScaleY();
        CCPoint origin = CCEGLView::sharedOpenGLView()->getViewPortRect().origin;
        //CCEGLView::sharedOpenGLView()->setScissorInPoints(screenPos.x*s, screenPos.y*s, m_tViewSize.width*s, m_tViewSize.height*s);
        glScissor((GLint)(screenPos.x*x + origin.x), (GLint)(screenPos.y*y + origin.y),
                  (GLsizei)(m_tViewSize.width*s*x), (GLsizei)(m_tViewSize.height*s*y));

    }
}

/**
 * retract what's done in beforeDraw so that there's no side effect to
 * other nodes.
 */
void CCScrollView::afterDraw()
{
#ifndef CC_WIN8_METRO
    if (m_bClippingToBounds)
    {
        glDisable(GL_SCISSOR_TEST);
    }
#else
	if (m_bClippingToBounds)
    {
		//DirectX impl
		ID3D11DeviceContext* cd3d = CCEGLView::sharedOpenGLView()->GetDeviceContext();
		ID3D11Device* d3d = CCEGLView::sharedOpenGLView()->GetDevice();
		// variables to hold the current rasterizer state and its description
		  ID3D11RasterizerState * rState=0 ;
		  D3D11_RASTERIZER_DESC rDesc ;

		  // cd3d is the ID3D11DeviceContext  
		  cd3d->RSGetState( &rState ) ; // retrieve the current state
		  rState->GetDesc( &rDesc ) ;    // get the desc of the state
		  rDesc.ScissorEnable = FALSE ; // change the ONE setting

		  // create a whole new rasterizer state
		  // d3d is the ID3D11Device
		  d3d->CreateRasterizerState( &rDesc, &rState ) ;

		  cd3d->RSSetState( rState );    // set the new rasterizer state
	}
#endif
}

void CCScrollView::visit()
{
	// quick return if not visible
	if (!isVisible())
    {
		return;
    }

	kmGLPushMatrix();
	
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        this->transformAncestors();
    }

	this->transform();
    this->beforeDraw();

	if(m_pChildren)
    {
		ccArray *arrayData = m_pChildren->data;
		unsigned int i=0;
		
		// draw children zOrder < 0
		for( ; i < arrayData->num; i++ )
        {
			CCNode *child =  (CCNode*)arrayData->arr[i];
			if ( child->getZOrder() < 0 )
            {
				child->visit();
			}
            else
            {
				break;
            }
		}
		
		// this draw
		this->draw();
		
		// draw children zOrder >= 0
		for( ; i < arrayData->num; i++ )
        {
			CCNode* child = (CCNode*)arrayData->arr[i];
			child->visit();
		}
        
	}
    else
    {
		this->draw();
    }

    this->afterDraw();

	if ( m_pGrid && m_pGrid->isActive())
    {
		m_pGrid->afterDraw(this);
    }

	kmGLPopMatrix();

}
void CCScrollView::addHighPriorityTouchListener(CCTouchDelegate* d)
{
    _high_priority_touch_listeners.push_back(d);
}

bool CCScrollView::ccTouchBegan(CCTouch* touch, CCEvent* ev)
{
    if (!this->isVisible())
    {
        return false;
    }

    CCRect frame;
    CCPoint frameOriginal = this->getParent()->convertToWorldSpace(this->getPosition());
    frame = CCRectMake(frameOriginal.x, frameOriginal.y, m_tViewSize.width, m_tViewSize.height);

    for(CCTouchDelegate* delegate : _high_priority_touch_listeners)
    {
        bool res = delegate->ccTouchBegan(touch, ev);
        if(res)
        {
            _high_priority_tracking[touch] = delegate;
            return true;
        }
    }

    //dispatcher does not know about clipping. reject touches outside visible bounds.
    CCPoint global_point = m_pContainer->convertToWorldSpace(m_pContainer->convertTouchToNodeSpace(touch));
    if (m_pTouches->count() > 2 ||
        m_bTouchMoved          ||
        !frame.containsPoint(global_point))
    {
        if(_touch_eat_zone.containsPoint(global_point))
            return true;
        return false;
    }

    if(_menu && !m_pRedirectToMenu->containsObject(touch))
    {
        pressPoint = touch->getLocationInView();
        bool res = _menu->ccTouchBegan(touch, ev);
        if(res)
            m_pRedirectToMenu->addObject(touch);
    }


    if (!m_pTouches->containsObject(touch))
    {
        m_pTouches->addObject(touch);

    }

    if (m_pTouches->count() == 1)
    { // scrolling
        m_tTouchPoint     = this->convertTouchToNodeSpace(touch);
        m_bTouchMoved     = false;
        m_bDragging     = true; //dragging started
        m_tScrollDistance = ccp(0.0f, 0.0f);
        m_fTouchLength    = 0.0f;
    }
//    else if (m_pTouches->count() == 2)
//    {
//        m_tTouchPoint  = ccpMidpoint(this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
//                                   this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
//        m_fTouchLength = ccpDistance(m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
//                                   m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
//        m_bDragging  = false;
//    }


    return true;
}

void CCScrollView::ccTouchMoved(CCTouch* touch, CCEvent* ev)
{
    if (!this->isVisible())
    {
        return;
    }

    auto high_priority = _high_priority_tracking.find(touch);
    if(high_priority != _high_priority_tracking.end())
    {
        high_priority->second->ccTouchMoved(touch, ev);
    }

    if(_menu && m_pRedirectToMenu->containsObject(touch))
    {
        _menu->ccTouchMoved(touch, ev);
    }
    if (m_pTouches->containsObject(touch))
    {
        if (m_pTouches->count() == 1 && m_bDragging)
        { // scrolling
            CCPoint moveDistance, newPoint, maxInset, minInset;
            CCRect  frame;
            float newX, newY;
            
            m_bTouchMoved  = true;
            CCPoint frameOriginal = this->getParent()->convertToWorldSpace(this->getPosition());
            frame = CCRectMake(frameOriginal.x, frameOriginal.y, m_tViewSize.width, m_tViewSize.height);

            newPoint     = this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0));
            moveDistance = ccpSub(newPoint, m_tTouchPoint);
            m_tTouchPoint  = newPoint;
            
            if (frame.containsPoint(this->convertToWorldSpace(newPoint)))
            {
                switch (m_eDirection)
                {
                    case kCCScrollViewDirectionVertical:
                        moveDistance = ccp(0.0f, moveDistance.y);
                        break;
                    case kCCScrollViewDirectionHorizontal:
                        moveDistance = ccp(moveDistance.x, 0.0f);
                        break;
                    default:
                        break;
                }

                m_pContainer->setPosition(ccpAdd(m_pContainer->getPosition(), moveDistance));
                
                maxInset = m_fMaxInset;
                minInset = m_fMinInset;
                
                
                //check to see if offset lies within the inset bounds
                newX     = MIN(m_pContainer->getPosition().x, maxInset.x);
                newX     = MAX(newX, minInset.x);
                newY     = MIN(m_pContainer->getPosition().y, maxInset.y);
                newY     = MAX(newY, minInset.y);
                
                m_tScrollDistance     = ccpSub(moveDistance, ccp(newX - m_pContainer->getPosition().x, newY - m_pContainer->getPosition().y));
                this->setContentOffset(ccp(newX, newY));
            }
        }
//        else if (m_pTouches->count() == 2 && !m_bDragging)
//        {
//            const float len = ccpDistance(m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
//                                            m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
//            this->setZoomScale(this->getZoomScale()*len/m_fTouchLength);
//        }
    }


}

void CCScrollView::ccTouchEnded(CCTouch* touch, CCEvent* ev)
{
    if (!this->isVisible())
    {
        return;
    }

    auto high_priority = _high_priority_tracking.find(touch);
    if(high_priority != _high_priority_tracking.end())
    {
        high_priority->second->ccTouchEnded(touch, ev);
        _high_priority_tracking.erase(high_priority);
    }

    if (m_pTouches->containsObject(touch))
    {
        if (m_pTouches->count() == 1 && m_bTouchMoved)
        {
            this->schedule(schedule_selector(CCScrollView::deaccelerateScrolling));
        }
        m_pTouches->removeObject(touch);


    } 
    if(_menu && m_pRedirectToMenu->containsObject(touch))
    {
        CCPoint endPoint = touch->getLocationInView();
        float x_diff = (endPoint.x - pressPoint.x);
        float y_diff = (endPoint.y - pressPoint.y);
        float manh_distance = x_diff * x_diff + y_diff * y_diff;

        if(manh_distance < 10000)
        {
            _menu->ccTouchEnded(touch, ev);
        }
        else
        {
            _menu->ccTouchCancelled(touch, ev);
        }
        m_pRedirectToMenu->removeObject(touch);
    }
    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;    
        m_bTouchMoved = false;
    }


}

void CCScrollView::ccTouchCancelled(CCTouch* touch, CCEvent* ev)
{
    if (!this->isVisible())
    {
        return;
    }
    auto high_priority = _high_priority_tracking.find(touch);
    if(high_priority != _high_priority_tracking.end())
    {
        high_priority->second->ccTouchCancelled(touch, ev);
        _high_priority_tracking.erase(high_priority);
    }

    if(_menu && m_pRedirectToMenu->containsObject(touch))
    {
        _menu->ccTouchMoved(touch, ev);
        m_pRedirectToMenu->removeObject(touch);
    }
    m_pTouches->removeObject(touch); 
    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;    
        m_bTouchMoved = false;
    }
}


