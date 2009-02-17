#ifndef facet_h
#define facet_h

#include <wx/wx.h>
#include <vector>
#include <utility>

#include "line.h"
#include "point.h"

class Cadmodel;

class Facet
{
    friend class Cadmodel;
public:
    Facet(const Point& normal, const std::vector<Point> &pts);
    Facet(const Facet& rhs);
    
    void scale(double factor);
    void changeDirection(const wxString& direction);
    std::pair<int, Line> intersect(double z);
   
    static bool isIntersect(const Point& p1, const Point& p2, double z);
    static Point calcIntersect(const Point& p1, const Point& p2, double z);
    static double intersect(double x1, double y1, double x2, double y2, double x);

private:
    Line intersect_0(double z); 
    //
    Point normal;
    std::vector<Point> points;
};

#endif
