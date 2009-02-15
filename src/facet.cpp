// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "facet.h"
#include "utility.h"

using namespace std;
using namespace cougar;

namespace {

bool isIntersect(const Point& p1, const Point& p2, double z)
{
    double val = (p1.z - z) * (p2.z - z);
    if(val <= 0.0) {
        return true;
    } else {
        return false;
    }
}

double intersect(double x1, double y1, double x2, double y2, double x)
{
    double y = (y2 - y1) / (x2 - x1) * (x - x1) + y1;
    return y;
}

Point calcIntersect(const Point& p1, const Point& p2, double z)
{
    double x1 = p1.x;
    double y1 = p1.y;
    double z1 = p1.z;

    double x2 = p2.x;
    double y2 = p2.y;
    double z2 = p2.z;

    double x = intersect(z1, x1, z2, x2, z);
    double y = intersect(z1, y1, z2, y2, z);
    
    Point p(x, y, z);
    return p;
}

}

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
            swap(point.y, point.z);
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

// 
// return -1: error
//         0: no intersection
//         1: intersected
pair<int, Line> Facet::intersect(double z)
{
    pair<int, Line> p;

    int c1 = 0;
    int c2 = 0;
    for(vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
        if(it->z > z) {
            c1++;
        }
        if(it->z < z) {
            c2++;
        }
    }
    
    if(c1 == 3 || c2 == 3) {
        p.first = 0;
        return p;
    } 
    
    //
    vector<Point> pv;
    Point pe;
    int n = 0;
    for(vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
        if(equal(it->z, z)) {
            n++;
            pe = *it;
        } else {
            pv.push_back(*it);
        }             
    }

    if(n == 0) {
        Line line = intersect_0(z); 
        p.second = line;
        p.first = 1;
    } else if(n == 1) {
        Point p1 = pv[0];
        Point p2 = pv[1];
        if(isIntersect(p1, p2, z)) {
            Point ap = calcIntersect(p1, p2, z);     
            Line line(pe, ap);
            p.second = line;
            p.first = 1;
        } else {
             p.first = 0; 
        }
    } else if(n == 2 || n == 3){
        p.first = -1; 
    } 

    return p;
}

Line Facet::intersect_0(double z)
{
    vector<Point> pv;
    int n = points.size();
    wxASSERT(n == 3);
    for(int i = 0; i < n; i++) {
        int next = (i + 1) % n;    
        Point p1 = points[i];
        Point p2 = points[next];
        if(isIntersect(p1, p2, z)) {
            Point p = calcIntersect(p1, p2, z); 
            pv.push_back(p);
        }
    } 

    Line line(pv[0], pv[1]);
    return line;
}
