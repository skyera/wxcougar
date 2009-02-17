#include <tut/tut.hpp>
#include "cadmodel.h"

namespace tut
{
struct cadmodel_data
{

};

typedef test_group<cadmodel_data> factory;
typedef factory::object object;

}

namespace 
{
tut::factory tf("cadmodel");
}

namespace tut
{

template<>
template<>
void object::test<1>()
{

}

}
