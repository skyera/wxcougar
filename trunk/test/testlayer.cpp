#include <tut/tut.hpp>
#include "layer.h"

namespace tut
{
struct layer_data
{

};

typedef test_group<layer_data> factory;
typedef factory::object object;

}

namespace
{
tut::factory tf("layer");
}

namespace tut
{

template<>
template<>
void object::test<1>()
{

}

}
