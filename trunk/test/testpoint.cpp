#include <tut/tut.hpp>
#include "point.h"

namespace tut
{
struct point_data
{
};

typedef test_group<point_data> factory;
typedef factory::object object;

}

namespace
{
tut::factory tf("point");
}

namespace tut
{

// test == operator
template<>
template<>
void object::test<1>()
{
    Point p1(1.1, 2.2, 3.3);
    Point p2(1.1, 2.2, 3.3);

    bool ok = (p1 == p2);
    ensure(ok);
}

}
