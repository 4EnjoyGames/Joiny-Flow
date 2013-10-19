#ifndef FGENERATORNODE_H
#define FGENERATORNODE_H

#include "../FlowPoint.h"
#include <vector>
#include <map>
#define USE_TWO_SHUFFLE
//#define THREE_BLOCK_SHUFFLE

typedef std::vector<FlowPoint> FlowStartEnd;
#include "Binary/Binary.h"

inline InputBinaryStream& operator>>(InputBinaryStream& is, FlowStartEnd& s)
{
    FlowPoint a(0,0);
    FlowPoint b(0,0);
    is >> a >> b;
    s.resize(0, FlowPoint(0,0));
    s.reserve(2);
    s.push_back(a);
    s.push_back(b);
    return is;
}

inline OutputBinaryStream& operator<<(OutputBinaryStream& os, const FlowStartEnd& s)
{
    os << s[0] << s[1];
    return os;
}


typedef std::vector<FlowStartEnd> FlowTask;


FlowTask generate(const Cordinate width, const Cordinate height);

typedef int TraceID;
typedef std::map<TraceID, FlowStartEnd> FlowTaskMap;
typedef std::vector<TraceID> TRow;
typedef std::vector<TRow> Table;

#endif // FGENERATORNODE_H