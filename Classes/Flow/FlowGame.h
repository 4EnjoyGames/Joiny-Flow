#ifndef FLOWGAME_H
#define FLOWGAME_H
#include "cocos2d.h"
#include "FlowRenderer.h"
#include <map>
typedef unsigned int FlowScore;

class FlowGame : public cocos2d::CCNode, public cocos2d::CCTouchDelegate
{
public:

    class Delegate
    {
    public:
        Delegate(){}
        virtual void onWin()
        {}

        virtual void onScoreChanged(const FlowScore)
        {}

        virtual ~Delegate()
        {}
    };
    typedef std::auto_ptr<Delegate> DelegatePtr;

    void endGame();

    FlowGame(const FlowTable& table, DelegatePtr delegate);
    static FlowGame* create(const FlowTable& table, DelegatePtr delegate);

    void connectHintPoints(const FlowPoint& a, const FlowPoint& b, const FlowColor color);
    void disconnectHintPoints(const FlowPoint& a, const FlowPoint& b);

    //verify has the user drow this path
    bool hasUserThisPath(const std::vector< FlowPoint>& path) const;

    //delete the hint path
    void deleteHintPath(const std::vector< FlowPoint>& path);

    //delete all connections which interfere to show hint path
    void deleteInterferePathes(const std::vector< FlowPoint>& path);

    void showPath(const std::vector< FlowPoint>& path);

    //return the cell color
    const FlowColor& getCellColor(const FlowPoint& point) const;

private:
    FlowRenderer* _renderer;
    const FlowTable* _table;

    FlowScore getScore();
    void updateScore();

    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    bool touchStart(const FlowPoint&);
    void touchMoved(const FlowPoint&);
    void touchEnded(const FlowPoint&);

    void cutTheTrace(const FlowPoint& start, const FlowPoint trace_id,
                     const bool remove_highlight);
    FlowColor getPointColor(const FlowPoint& p);
    void updateHighlighted();
    void removeHighlight(FlowPointState &st);

    void connectPoints(FlowPointState& a, FlowPointState& b);
    void disconnectPoints(FlowPointState& a, FlowPointState& b);


    bool checkWinCondition();
    void updateActiveCircle(cocos2d::CCPoint p);
    void restoreTraces();

    FlowPoint touchToCellCordinates(cocos2d::CCPoint p);
    cocos2d::CCPoint touchToLocalCords(cocos2d::CCTouch* touch);

    typedef std::vector<FlowPoint> Trace;
    bool isTraceFinished(const Trace& t);
    typedef std::map<FlowPoint, Trace> Traces;

    bool _is_touch_active;
    bool _finish_lock;
    int _active_touch_id;
    Traces _active_traces;
    Traces _submitted_traces;

    FlowPoint _last_point;
    FlowColor _working_color;
    FlowPoint _working_trace_id;
    DelegatePtr _delegate;
    FlowScore _score;


    void traceDebug() const;
    void traceDebug(std::ostream& os, const Traces& tr) const;
};

#endif // FLOWGAME_H
