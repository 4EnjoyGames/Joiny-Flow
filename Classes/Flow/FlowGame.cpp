#include "FlowGame.h"
#include <algorithm>
using namespace cocos2d;
FlowGame::FlowGame(const FlowTable &table, DelegatePtr delegate)
    : _renderer(FlowRenderer::create(table)), _table(0), _is_touch_active(false),
      _active_touch_id(-1), _active_traces(), _submitted_traces(),
      _working_color(-1), _finish_lock(false), _delegate(delegate),
      _last_point(0,0), _working_trace_id(0,0), _score(0)
{
    _table = &_renderer->getTable();
    this->setContentSize(_renderer->getContentSize());
    this->addChild(_renderer);
    _renderer->setAnchorPoint(ccp(0,0));

    //    for(unsigned int i=0; i<table.getColorsNumber(); ++i)
    //    {
    //        _active_traces[i].clear();
    //    }
    _submitted_traces = _active_traces;

    //Start listening for touch
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
    CCLog("Game start output");
    traceDebug();
}


FlowGame* FlowGame::create(const FlowTable& table, DelegatePtr delegate)
{
    FlowGame* g = new FlowGame(table, delegate);
    if(g != 0)
        g->autorelease();
    return g;
}

bool FlowGame::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *)
{
    if(!_is_touch_active && this->isVisible())
    {
        CCPoint local = touchToLocalCords(pTouch);
        bool res = touchStart(touchToCellCordinates(local));
        if(res)
        {
            _active_touch_id = pTouch->getID();
            _is_touch_active = true;

            updateActiveCircle(local);
        }
        return res;
    }
    return false;
}

void FlowGame::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *)
{
    if(_is_touch_active && pTouch->getID()==_active_touch_id && this->isVisible())
    {
        CCPoint local = touchToLocalCords(pTouch);
        touchMoved(touchToCellCordinates(local));

        updateActiveCircle(local);
    }
}

void FlowGame::updateActiveCircle(cocos2d::CCPoint local)
{
    if(local.x <= this->getContentSize().width && local.x >= 0)
        _renderer->showActiveCircle(local, _working_color);
    else
        _renderer->hideActiveCircle();
}

void FlowGame::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *)
{
    if(_is_touch_active && pTouch->getID()==_active_touch_id && this->isVisible())
    {
        touchEnded(touchToCellCordinates(touchToLocalCords(pTouch)));
        _renderer->hideActiveCircle();
    }
}

void FlowGame::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *)
{
    if(_is_touch_active && pTouch->getID()==_active_touch_id && this->isVisible())
    {
        touchEnded(touchToCellCordinates(touchToLocalCords(pTouch)));
        _renderer->hideActiveCircle();
    }
}

cocos2d::CCPoint FlowGame::touchToLocalCords(cocos2d::CCTouch* touch)
{
    //Get touch point in local cordinates
    CCPoint touchLocation = touch->getLocation();
    CCPoint local_cords = this->convertToNodeSpace(touchLocation);
    return local_cords;
}

FlowPoint FlowGame::touchToCellCordinates(cocos2d::CCPoint local_cords)
{


    CCSize cell_size = _renderer->getNodeSize();

    int x = static_cast<int>(local_cords.x / cell_size.width);
    int y = static_cast<int>(local_cords.y / cell_size.height);

    if(x < 0)
        x = _table->getWidth();
    if(y < 0)
        y = _table->getHeight();

    return FlowPoint(x, y);
}

bool FlowGame::touchStart(const FlowPoint& p)
{
    if(p.x() >= _table->getWidth() || p.y() >= _table->getHeight())
        return false;
    bool res = false;
    FlowColor color = getPointColor(p);
    FlowPointState st = _table->get(p);

    if(color >= 0)
    {
        //The point should be initial or a part of other line
        //if(st.getNodeType() == FlowPointState::Circle ||
        //        st.hasPrevious())
        if(st.getNodeType() == FlowPointState::Circle)
        {
            Trace& a1 = _active_traces[st.getTraceId()];
            if(a1.size() > 0)
                cutTheTrace(a1[0], st.getTraceId(), true);
            a1.clear();


            _working_color = color;
            _working_trace_id = p;
            _last_point = p;

            st=_table->get(st.getCordinates());
            st.setTraceId(p);
            _renderer->updateCell(st.getCordinates(), st);
            res = true;
        }
        else if(st.hasNext() || st.hasPrevious())
        {
            res = true;
            _working_color = color;
            _working_trace_id = st.getTraceId();
            _last_point = p;
            cutTheTrace(p, _working_trace_id, true);

        }

        //If we start from circle on oposit side
        updateScore();
    }

    traceDebug();
    return res;
}



void FlowGame::touchMoved(const FlowPoint& p)
{
    if(p.x() >= _table->getWidth() || p.y() >= _table->getHeight())
        return;
    if(_last_point != p)
    {
        FlowPointState st = _table->get(p);
        FlowPointState cur_state = _table->get(_last_point);
        if(st.getTraceId() == _working_trace_id && st.hasNext())
        {
            //We have an recursion

            //We should just clean the trace
            cutTheTrace(p, _working_trace_id, false);
            _last_point = p;
            _finish_lock = false;

            restoreTraces();
        }
        else if(isPointNextTo(_last_point, p))
        {


            if(st.getLineColor() == _working_color &&
                    st.getNodeType() == FlowPointState::Circle && !_finish_lock)
            {
                //We are at finish (or at start)

                //If the line do not start from this circle we are at finish
                if(st.getCordinates() != _working_trace_id)
                {


                    //Ending there
                    Trace& a2 = _active_traces[st.getTraceId()];
                    if(a2.size() > 0)
                    {
                        if(st.hasPrevious() && a2.size() > 1)
                            cutTheTrace(a2[a2.size() - 2], st.getTraceId(), false);
                        else
                        {
                            cutTheTrace(a2[0], st.getTraceId(), false);
                            a2.clear();
                        }
                    }

                    connectPoints(cur_state, st);
                    Trace& t = _active_traces[_working_trace_id];
                    if(t.size() == 0)
                        t.push_back(_last_point);
                    t.push_back(p);
                    _last_point = p;

                    _finish_lock = true;



                    restoreTraces();

                }
            }
            else if(!_finish_lock && st.getNodeType() != FlowPointState::Circle)
            {
                //We are at other cell

                //If next cell is a part of other line we need to clean up that line
                if(st.hasPrevious())
                {
                    FlowPoint next_trace_id = _table->get(st.getPreviousCordinates()).getTraceId();

                    Trace& t = _active_traces[next_trace_id];
                    //FlowColor next_color = getPointColor(st.getPreviousCordinates());
                    if(t.size() > 0)
                    {
                        cutTheTrace(st.getPreviousCordinates(), next_trace_id, false);
                    }
                }

                //Add new point to current trace
                connectPoints(cur_state, st);
                Trace& t = _active_traces[_working_trace_id];
                if(t.size() == 0)
                    t.push_back(_last_point);
                t.push_back(p);
                _last_point = p;
                restoreTraces();

            }
        }
        updateScore();
    }

}
void FlowGame::endGame()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void FlowGame::touchEnded(const FlowPoint& p)
{

    touchMoved(p);
    _is_touch_active = false;
    _submitted_traces = _active_traces;
    _finish_lock = false;
    updateHighlighted();

    updateScore();
    if(checkWinCondition())
    {
        //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        if(_delegate.get() != 0)
        {
            _delegate->onWin();
        }
    }

}
void FlowGame::updateScore()
{
    FlowScore score = getScore();
    if(score != _score)
    {
        _score = score;
        if(_delegate.get() != 0)
            _delegate->onScoreChanged(_score);

    }
}

void FlowGame::cutTheTrace(const FlowPoint& start, const FlowPoint trace_id,
                           const bool remove_highlight)
{
    FlowPointState st = _table->get(start);
    while(st.hasNext())
    {
        FlowPointState next_st = _table->get(st.getNextCordinates());

        if(remove_highlight)
        {
            removeHighlight(st);
            removeHighlight(next_st);
        }

        disconnectPoints(st, next_st);



        st = next_st;
    }

    Trace& t = _active_traces[trace_id];
    Trace::iterator i = std::find(t.begin(), t.end(), start);
    if(i != t.end())
    {
        t.erase(i+1, t.end());
    }
}

void FlowGame::removeHighlight(FlowPointState& st)
{
    if(st.isHighlighted())
    {
        FlowPointState nst(st);
        nst.setHighlighted(false);
        _renderer->updateCell(nst.getCordinates(), nst);
    }
}

void FlowGame::connectPoints(FlowPointState& a_st, FlowPointState& b_st)
{
    //FlowPointState a_st = _table->get(a);
    //FlowPointState b_st = _table->get(b);

    a_st.setNextCordinates(b_st.getCordinates());
    b_st.setPreviousCordinates(a_st.getCordinates());
    b_st.removeNext();
    b_st.setLineColor(a_st.getLineColor());
    b_st.setTraceId(a_st.getTraceId());

    _renderer->updateCell(a_st.getCordinates(), a_st);
    _renderer->updateCell(b_st.getCordinates(), b_st);
}
void FlowGame::disconnectPoints(FlowPointState &a_st, FlowPointState &b_st)
{
    //FlowPointState a_st = _table->get(a);
    //FlowPointState b_st = _table->get(b);

    a_st.removeNext();
    b_st.removePrevious();

    b_st.setTraceId(FlowPoint::UNDEFINED);
    _renderer->updateCell(a_st.getCordinates(), a_st);
    _renderer->updateCell(b_st.getCordinates(), b_st);
}

void FlowGame::updateHighlighted()
{
    unsigned int width = _table->getWidth();
    unsigned int height = _table->getHeight();

    for(unsigned int x=0; x<width; ++x)
    {
        for(unsigned int y=0; y<height; ++y)
        {
            FlowPoint p(x,y);
            FlowPointState st = _table->get(p);
            if(st.isHighlighted())
            {
                st.setHighlighted(false);
                _renderer->updateCell(p, st);
            }
        }
    }

    for(Traces::iterator i=_active_traces.begin(); i!=_active_traces.end(); ++i)
    {
        Trace* it = &i->second;
        for(Trace::iterator j=it->begin(); j!=it->end(); ++j)
        {
            FlowPoint p = *j;
            FlowPointState st = _table->get(p);
            st.setHighlighted(true);
            st.setHighlightColor(st.getLineColor());
            _renderer->updateCell(p, st);

        }
    }
}

void FlowGame::restoreTraces()
{

    typedef Traces::iterator It;
    for(It c=_active_traces.begin(); c!=_active_traces.end(); ++c)
    {
        if(c->first != _working_trace_id)
        {
            Trace& updated = c->second;
            Trace& original = _submitted_traces[c->first];

            //if(updated.size() > 0)
            //{
                for(unsigned int i=updated.size(); i<original.size(); ++i)
                {
                    if(i != 0)
                    {

                        FlowPointState previous = _table->get(updated[i-1]);
                        FlowPointState next = _table->get(original[i]);

                        if(next.isFree()/* || (next.getNodeType() == FlowPointState::Circle
                                             && next.getLineColor() == previous.getLineColor())*/)
                        {
                            updated.push_back(original[i]);
                            connectPoints(previous, next);
                        }
                        else
                            break;
                    }
                    else
                    {
                        FlowPointState previous = _table->get(original[0]);
                        if(previous.getTraceId() != FlowPoint::UNDEFINED)
                            break;
                        else
                        {

                            updated.push_back(original[0]);
                            previous.setTraceId(previous.getCordinates());
                            _renderer->updateCell(previous.getCordinates(), previous);
                        }
                    }
                }
            //}

        }
    }
}

FlowColor FlowGame::getPointColor(const FlowPoint& p)
{
    FlowColor color = -1;

    const FlowPointState& state = _table->get(p);
    if(state.getNodeType() == FlowPointState::Circle)
    {
        color = state.getLineColor();
    }
    else
    {
        //If we have next node just see the color
        if(state.hasNext())
        {
            color = state.getLineColor();
        } //If there is no next but we have previuos then we can take its color
        else if(state.hasPrevious())
        {
            color = _table->get(state.getPreviousCordinates()).getLineColor();
        }
    }
    return color;
}
FlowScore FlowGame::getScore()
{
    FlowScore score = 0;
    for(Traces::const_iterator it = _active_traces.begin(); it!=_active_traces.end(); ++it)
    {
        const Trace& t = it->second;
        if(t.size()>0)
            score += 100;
        if(t.size()>1)
        {
            FlowPointState st = _table->get(t[t.size()-1]);
            if(st.getNodeType() == FlowPointState::Circle)
                score += 100;
        }
        if(t.size()>0)
        {
            unsigned int length = t.size() - 1;
            score += length * length * 100;
        }
    }
    return score;
}
bool FlowGame::isTraceFinished(const Trace& t)
{
    if(t.size() < 2)
    {
        return false;
    }

    FlowPoint start = t[0];
    FlowPoint end = t[t.size()-1];
    FlowPointState start_st = _table->get(start);
    FlowPointState end_st = _table->get(end);

    if(start_st.getNodeType() != FlowPointState::Circle ||
            end_st.getNodeType() != FlowPointState::Circle)
    {
        return false;
    }
    return true;
}

bool FlowGame::checkWinCondition()
{
    bool win = true;

    for(unsigned int x=0; x<_table->getWidth(); ++x)
    {
        for(unsigned int y=0; y<_table->getHeight(); ++y)
        {
            FlowPoint p(x,y);
            FlowPointState st = _table->get(p);
            if(st.getNodeType() == FlowPointState::Circle)
            {
                FlowPoint trace_id = st.getTraceId();
                if(trace_id == FlowPoint::UNDEFINED)
                    win = false;
                else if(!isTraceFinished(_active_traces[trace_id]))
                    win = false;
            }
        }
    }



    return win;
}
void FlowGame::cleanTheTrace()
{

}

void FlowGame::traceDebug(std::ostream& ss, const Traces& tr) const
{
    //    for(unsigned int c=0; c<_table->getColorsNumber(); ++c)
    //    {
    //        ss << c << ":\t";
    //        const Trace& t = tr[c];
    //        for(unsigned int i=0; i<t.size(); ++i)
    //        {
    //            FlowPoint p = t[i];
    //            if(i)
    //                ss << ", ";
    //            ss << "(" << p.x() << ";" << p.y() << ")";
    //        }
    //        ss << std::endl;
    //    }
}

void FlowGame::traceDebug() const
{
    //    std::stringstream ss;
    //    ss << "Active traces output" << std::endl;
    //    traceDebug(ss, _active_traces);
    //    ss << "Submited traces output" << std::endl;
    //    traceDebug(ss, _submitted_traces);
    //    ss << std::endl << std::endl;

    //    CCLog(ss.str().c_str());

}
