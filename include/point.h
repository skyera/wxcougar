#ifndef point_h
#define point_h

struct Point
{
    Point(double x = 0, double y = 0, double z = 0);
    Point(const Point& rhs);

    bool operator==(const Point& rhs) const;

    double x;
    double y;
    double z;
};

#endif
