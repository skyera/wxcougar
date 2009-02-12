#include "point.h"
#include <cmath>
#include "utility.h"

using namespace std;
using namespace cougar;

Point::Point(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::Point(const Point& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
}

bool Point::operator==(const Point& rhs) const
{
    if(equal(x, rhs.x) && equal(y, rhs.y) && equal(z, rhs.z)) {
        return true;
    } else {
        return false;
    }   
}
