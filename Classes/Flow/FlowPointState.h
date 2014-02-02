#ifndef FLOWPOINTSTATE_H
#define FLOWPOINTSTATE_H
#include "FlowColor.h"
#include "FlowPoint.h"
class FlowPointState
{
public:
    enum NodeType
    {
        Free,
        Circle
    };

    FlowPointState();

    void setNodeType(const NodeType s);
    const NodeType getNodeType() const;

    void setLineColor(const FlowColor c);
    const FlowColor getLineColor() const;

    void setHighlightColor(const FlowColor c);
    const FlowColor getHighlightColor() const;

    void setHighlighted(const bool h);
    const bool isHighlighted() const;

    void setNextCordinates(const FlowPoint p);
    void removeNext();
    bool hasNext() const;

    void setPreviousCordinates(const FlowPoint p);
    void removePrevious();
    bool hasPrevious() const;
    bool isFree() const;

    const FlowPoint getNextCordinates() const;
    const FlowPoint getPreviousCordinates() const;

    void setTraceId(const FlowPoint p);
    const FlowPoint getTraceId() const;

    void setCordinates(const FlowPoint p);
    const FlowPoint getCordinates() const;

    //hints
    void setHintColorColor(const FlowColor c);
    const FlowColor getHintColor() const;

    void setHintNextCordinate(const FlowPoint p);
    bool hasHintNext() const;
    void removeHintNext();


    const FlowPoint getHintNextCordinate() const;

private:
    FlowColor _line_color;
    FlowColor _highlight_color;
    bool _highlighted;

    FlowPoint _my_cordinates;

    FlowPoint _trace_id;

    FlowPoint _next_cordinates;
    bool _has_next;

    FlowPoint _previous_cordinates;
    bool _has_previous;

    //hints
    FlowColor _hint_color;
    FlowPoint _hint_next_cordinates;
    bool _hint_has_next;

    NodeType _state;

};

#endif // FLOWPOINTSTATE_H
