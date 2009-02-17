#include <tut/tut.hpp>
#include "line.h"
#include "utility.h"

namespace tut
{
struct line_data
{

};

typedef test_group<line_data> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("line");
}

namespace tut
{

template<>
template<>
void object::test<1>()
{
    Point p1(1, 1, 1);
    Point p2(4, 4, 1);

    Line line(p1, p2);

    double k = line.slope();
    bool ok = cougar::equal(k, 1.0);
    ensure(ok);
}

}
