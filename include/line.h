#ifndef line_h
#define line_h

#include "point.h"

class Line
{
public:
    Line(const Point& p1=Point(), const Point& p2=Point());

    Point m_p1;
    Point m_p2;
};

#endif
