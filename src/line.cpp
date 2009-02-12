#include "line.h"
#include <limits>
#include <cmath>
#include "utility.h"

using namespace std;
using namespace cougar;

Line::Line(const Point& p1, const Point& p2):m_p1(p1), m_p2(p2)
{

}

double Line::slope()
{
    double diffy = m_p2.y - m_p1.y;
    double diffx = m_p2.x - m_p1.x;
    
    if(equal(diffx, 0.0)) {
        return numeric_limits<double>::max();
    } else {
        double k = diffy / diffx;
        return k;
    }
}
