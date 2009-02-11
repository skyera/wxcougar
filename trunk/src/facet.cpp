#include "facet.h"

using namespace std;

Facet::Facet(const Point& normal, const std::vector<Point>& pts):normal(normal), points(pts)
{

}

Facet::Facet(const Facet& rhs)
{
    normal = rhs.normal;
    points = rhs.points;
}

void Facet::scale(double factor)
{
    for(vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
        it->x *= factor;
        it->y *= factor;
        it->z *= factor;
    }
}

void Facet::changeDirection(const wxString& direction)
{
    if(direction == wxT("+X")) {
        for(vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
            Point& point = *it;    
            swap(point.x, point.z);
        }
    } else if(direction == wxT("-X")) {
        
        for(vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
            Point& point = *it;    
            swap(point.x, point.z);
            point.z = -point.z;
        }
    } else if(direction == wxT("+Y")) {
        
        for(vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
            Point& point = *it;    
            swap(point.y, point.z);
        }
    } else if(direction == wxT("-Y")) {
    
        for(vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
            Point& point = *it;    
            swap(point.x, point.z);
            point.z = -point.z;
        }
    } else if(direction == wxT("+Z")) {
    
    } else if(direction == wxT("-Z")) {
        
        for(vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
            Point& point = *it;    
            point.z = -point.z;
        }
    } 
}
