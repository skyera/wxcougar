#ifndef line_h
#define line_h

#include "point.h"

class Line
{
public:
    Line(const Point& p1=Point(), const Point& p2=Point());
    
    double slope();
    Point point1_;
    Point point2_;
};

#endif
