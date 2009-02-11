#ifndef facet_h
#define facet_h

#include <wx/wx.h>
#include <vector>
#include "point.h"

class Facet
{
public:
    Facet(const Point& normal, const std::vector<Point> &pts);
    Facet(const Facet& rhs);
    void scale(double factor);
    void changeDirection(const wxString& direction);
    //
    Point normal;
    std::vector<Point> points;

};

#endif
