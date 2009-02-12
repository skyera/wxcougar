#include "point.h"
#include <cmath>

using namespace std;

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

namespace {
const double LIMIT = 1e-8;
bool equal(double d1, double d2)
{
    double diff = fabs(d1 - d2);
    if(diff <= LIMIT) {
        return true;
    } else {
        return false;
    }
}

}

bool Point::operator==(const Point& rhs)
{
    if(equal(x, rhs.x) && equal(y, rhs.y) && equal(z, rhs.z)) {
        return true;
    } else {
        return false;
    }   
}
