// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "point.h"
#include "utility.h"

#include <cmath>

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
