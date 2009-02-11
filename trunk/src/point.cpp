#include "point.h"

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
