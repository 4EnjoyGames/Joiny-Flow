#include "FlowGame.h"
#include <algorithm>
#include "Logic/Tutorial.h"
#include "Core/MusicSettings.h"

using namespace cocos2d;

FlowGame::FlowGame(const FlowTable &table, DelegatePtr delegate)
    : _is_tracking_touch(false),
      _renderer(FlowRenderer::create(table)),
      _table(0), _is_touch_active(false),
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
    startTrackingTouch();
    CCLog("Game start output");
    traceDebug();
}
void FlowGame::startTrackingTouch()
{
    if(!_is_tracking_touch)
    {
        _is_tracking_touch = true;
        CCDirector* pDirector = CCDirector::sharedDirector();
        pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);

    }
}

void FlowGame::stopTrackingTouch()
{
    if(_is_tracking_touch)
    {
        _is_tracking_touch = false;
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    }
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

void FlowGame::ccTouchMoved(cocos2d::CCTouch *pTouch,
                            cocos2d::CCEvent *)
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

void FlowGame::ccTouchEnded(cocos2d::CCTouch *pTouch,
                            cocos2d::CCEvent *)
{
    if(_is_touch_active && pTouch->getID()==_active_touch_id && this->isVisible())
    {
        touchEnded(touchToCellCordinates(touchToLocalCords(pTouch)));
        _renderer->hideActiveCircle();
    }

    //if the trace is that we would like the user touched
    if(Tutorial::getInstance()->isActive() &&
            hasUserThisPath(Tutorial::getInstance()->getCurrentTutorialHintPath()))
    {
        if(Tutorial::getInstance()->hasTutorial())
            Tutorial::getInstance()->showTutorial();
    }
}

void FlowGame::ccTouchCancelled(cocos2d::CCTouch *pTouch,
                                cocos2d::CCEvent *)
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
    if(local_cords.x < 0 || local_cords.y < 0)
        return FlowPoint::UNDEFINED;

    CCSize cell_size = _renderer->getNodeSize();

    int x = static_cast<int>(local_cords.x / cell_size.width);
    int y = static_cast<int>(local_cords.y / cell_size.height);

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
    stopTrackingTouch();
}
//void FlowGame::playMusic(const FlowPoint& p)
//{
//    FlowColor color = getCellColor(p)+1;
//    std::stringstream file_name;
//    file_name << "music/flow_game/note" << color << ".wav";
//    MusicSettings::playSoundEffect(file_name.str().c_str());
//}

void FlowGame::touchEnded(const FlowPoint& p)
{

    touchMoved(p);
    _is_touch_active = false;
    _submitted_traces = _active_traces;
    _finish_lock = false;
    updateHighlighted();

    //if the point is on the table
    //if((p.x() < _table->getWidth() && p.y() < _table->getHeight()))
    {
        //play sound effect
        MusicSettings::playSoundEffect("music/flow_click.wav");
    }

    updateScore();
    if(checkWinCondition())
    {
        //stopTrackingTouch();
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

void FlowGame::cutTheTrace(const FlowPoint& start,
                           const FlowPoint trace_id,
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
        st.setHighlighted(false);
        _renderer->updateCell(nst.getCordinates(), nst);
    }
}
void FlowGame::connectHintPoints(const FlowPoint &a_p, const FlowPoint &b_p, const FlowColor color)
{
    FlowPointState a = _table->get(a_p);
    a.setHintNextCordinate(b_p);
    a.setHintColorColor(color);
    _renderer->updateCell(a_p, a);
}

void FlowGame::disconnectHintPoints(const FlowPoint &a_p, const FlowPoint &b_p)
{
    FlowPointState a = _table->get(a_p);
    FlowPointState b = _table->get(b_p);

    if(a.getHintNextCordinate() == b_p)
    {
        a.removeHintNext();
        _renderer->updateCell(a_p, a);
    }
    else if(b.getHintNextCordinate() == a_p)
    {
        b.removeHintNext();
        _renderer->updateCell(b_p, b);
    }
    else
    {
        CCLog("ERROR: Points are not connected");
        assert(false);
    }

}
bool FlowGame::connectedFlowPoints(FlowPointState st_current,
                                   FlowPointState st_previous) const
{
    bool result = false;

    if(st_previous.getLineColor() == st_current.getLineColor())
    {
        if(st_current.hasPrevious())
        {
            if(st_current.getPreviousCordinates() == st_previous.getCordinates())
                result = true;
        }
        if(st_current.hasNext())
        {
            if(st_current.getNextCordinates() == st_previous.getCordinates())
                result = true;
        }
        if(st_previous.hasNext())
        {
            if(st_previous.getNextCordinates() == st_current.getCordinates())
                result = true;
        }
        if(st_previous.hasPrevious())
        {
            if(st_previous.getPreviousCordinates() == st_current.getCordinates())
                result = true;
        }
    }
    return result;
}

//verify has the user drow this path
bool FlowGame::hasUserThisPath(
        const std::vector< FlowPoint>& path) const
{

    bool result = true;
    FlowPointState st_current;
    FlowPointState st_previous;
    for(unsigned int i=0; i< path.size(); ++i)
    {
        st_previous = st_current;
        st_current = _table->get(path[i]);

        //if we have previous
        if(i!=0)
        {
            if(!connectedFlowPoints(st_current, st_previous))
            {
                result = false;
                break;
            }
        }
    }

    return result;
}

//delete the hint path
void FlowGame::deleteHintPath(
        const std::vector< FlowPoint>& path)
{

    for(unsigned int i=1; i<path.size(); ++i)
    {
        //CCLog("(%d, %d) -> (%d, %d)",path[i-1].x(),path[i-1].y(),
        //       path[i].x(),path[i].y());

        disconnectHintPoints(path[i-1], path[i]);
    }
}

//delete all connections which interfere to show hint path
void FlowGame::deleteInterferePathes(
        const std::vector< FlowPoint>& path)
{
    //Clean space
    for(unsigned int i=0; i< path.size(); ++i)
    {
        FlowPoint curr_p = path[i];

        //if current coordinate has connectin.png -> cut the trace
        FlowPointState st = _table->get(curr_p);
        //FlowColor color = getPointColor(curr_p);
        if(st.hasNext() || st.hasPrevious())
        {

            Trace& a1 = _submitted_traces[st.getTraceId()];
            Trace& a2 = _active_traces[st.getTraceId()];
            if(a1.size() > 0)
                cutTheTrace(a1[0], st.getTraceId(), true);
            a1.clear();
            a2.clear();

        }
    }
}
void FlowGame::showPath(const std::vector< FlowPoint>& path)
{
    if(path.size() > 0)
    {
        FlowPointState st = _table->get(path[0]);

        //Path should always start from circle
        assert(st.getNodeType() == FlowPointState::Circle);

        //Put new trace as submited
        Trace& a1 = _submitted_traces[path[0]];
        a1 = path;

        //Update starting point
        st = _table->get(path[0]);
        st.setTraceId(FlowPoint::UNDEFINED);
        _renderer->updateCell(st.getCordinates(), st);

        //Clear active trace starting from same point
        Trace& active = _active_traces[path[0]];
        active.clear();

        _working_trace_id = FlowPoint::UNDEFINED;

        //Use restore function to restore trace
        restoreTraces();
    }

    updateScore();
    if(checkWinCondition())
    {
         if(_delegate.get() != 0)
        {
            _delegate->onWin();
        }
    }
}

const FlowColor FlowGame::getCellColor(const FlowPoint& point) const
{
    FlowPointState st = _table->get(point);
    return st.getLineColor();
}
void FlowGame::connectPoints(FlowPointState& a_st, FlowPointState& b_st)
{
    //TODO: disconect hints

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
