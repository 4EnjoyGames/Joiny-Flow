#include "FlowPointState.h"
const FlowPoint FlowPoint::UNDEFINED(100,100);
FlowPointState::FlowPointState()
    : _line_color(0),
      _state(Free),
      _highlighted(false),
      _has_next(false),
      _has_previous(false),
      _my_cordinates(0,0),
      _next_cordinates(0,0),
      _previous_cordinates(0,0),
      _trace_id(FlowPoint::UNDEFINED),
      _hint_has_next(false),
      _hint_has_previous(false),
      _hint_next_cordinates(0,0),
      _hint_previous_cordinates(0,0)

{}

void FlowPointState::setNodeType(const NodeType s)
{
    _state = s;
}

void FlowPointState::setLineColor(const FlowColor c)
{
    _line_color = c;
}
void FlowPointState::setHighlightColor(const FlowColor c)
{
    _highlight_color = c;
}
void FlowPointState::setTraceId(const FlowPoint p)
{
    _trace_id = p;
}

const FlowPoint FlowPointState::getTraceId() const
{
    return _trace_id;
}
void FlowPointState::setHighlighted(const bool h)
{
    _highlighted = h;
}
void FlowPointState::setNextCordinates(const FlowPoint p)
{
    _next_cordinates = p;
    _has_next = true;
}


void FlowPointState::setCordinates(const FlowPoint p)
{
    _my_cordinates = p;
}

const FlowPointState::NodeType FlowPointState::getNodeType() const
{
    return _state;
}
const FlowColor FlowPointState::getLineColor() const
{
    return _line_color;
}
const FlowColor FlowPointState::getHighlightColor() const
{
    return _highlight_color;
}
const bool FlowPointState::isHighlighted() const
{
    return _highlighted;
}
const FlowPoint FlowPointState::getNextCordinates() const
{
    return _next_cordinates;
}
bool FlowPointState::hasNext() const
{
    return _has_next;
}

void FlowPointState::removeNext()
{
    _has_next = false;
}


void FlowPointState::setPreviousCordinates(const FlowPoint p)
{
    _previous_cordinates = p;
    _has_previous = true;
}

void FlowPointState::removePrevious()
{
    _has_previous = false;
}
bool FlowPointState::hasPrevious() const
{
    return _has_previous;
}

const FlowPoint FlowPointState::getCordinates() const
{
    return _my_cordinates;
}

const FlowPoint FlowPointState::getPreviousCordinates() const
{
    return _previous_cordinates;
}

bool FlowPointState::isFree() const
{
    return !hasPrevious() && !hasNext();
}
void FlowPointState::setHintNextCordinate(const FlowPoint p)
{
    _hint_next_cordinates = p;
    _hint_has_next = true;
}
bool FlowPointState::hasHintNext() const
{
    return _hint_has_next;
}

void FlowPointState::removeHintNext()
{
     _hint_has_next = false;
}

void FlowPointState::setHintPreviousCordinate(const FlowPoint p)
{
    _hint_previous_cordinates = p;
    _hint_has_previous = true;
}
bool FlowPointState::hasHintPrevious() const
{
    return _hint_has_previous;
}

void FlowPointState::removeHintPrevious()
{
     _hint_has_previous = false;
}

const FlowPoint FlowPointState::getHintNextCordinate() const
{
    return _hint_next_cordinates;
}
const FlowPoint FlowPointState::getHintPreviousCordinate() const
{
    return _hint_previous_cordinates;
}
