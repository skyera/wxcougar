#include <tut/tut.hpp>
#include "facet.h"
#include "utility.h"

namespace tut
{

struct facet_data
{

};

typedef test_group<facet_data> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("facet");    
}

namespace tut
{

template<>
template<>
void object::test<1>()
{
    double x1 = 0;
    double y1 = 0;
    double x2 = 4;
    double y2 = 4;
    double x = 2;
    double y = Facet::intersect(x1, y1, x2, y2, x);
    bool ok  = cougar::equal(y, 2.0);
    ensure(ok);
}

template<>
template<>
void object::test<2>()
{
    
    Point p1(0, 0, 1);
    Point p2(0, 0, 4);
    
    Point p = Facet::calcIntersect(p1, p2, 1);
    bool ok = (p == Point(0, 0, 1));
    ensure(ok);
}

}
