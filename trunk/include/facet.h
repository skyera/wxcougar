#ifndef facet_h
#define facet_h

#include <vector>
#include "point.h"

class Facet
{
public:
    Facet(const Point& normal, const std::vector<Point> &pts);
    
    Point normal;
    std::vector<Point> points;

};

#endif
