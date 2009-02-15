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

#include "line.h"
#include "utility.h"

#include <limits>
#include <cmath>

using namespace std;
using namespace cougar;

Line::Line(const Point& p1, const Point& p2):m_p1(p1), m_p2(p2)
{

}

double Line::slope()
{
    double diffy = m_p2.y - m_p1.y;
    double diffx = m_p2.x - m_p1.x;
    
    if(equal(diffx, 0.0)) {
        return numeric_limits<double>::max();
    } else {
        double k = diffy / diffx;
        return k;
    }
}
