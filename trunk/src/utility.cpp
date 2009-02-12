#include "utility.h"
#include <cmath>

using namespace std;

namespace cougar
{
bool equal(double d1, double d2)
{
    const double LIMIT = 1e-8;
    double diff = fabs(d1 - d2);
    return diff <= LIMIT;
}

}
