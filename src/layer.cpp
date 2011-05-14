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

#include "layer.h"
#include "utility.h"

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <algorithm>
#include <set>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cougar;

Layer::Layer(double z, double pitch)
{
    z_ = z;
    pitch_ = pitch;
}

bool Layer::setLines(const vector<Line>& lines)
{
    lines_ = lines;
    bool ok = createLoops();
    if(!ok) {
        return false;
    }

    calcDimension();
    createScanlines();
    createChunks();
    return true;
}

bool Layer::createLoops()
{
    loops_.clear();
    while(!lines_.empty()) {
        Line line = lines_.back();
        lines_.pop_back();

        vector<Line> loop;
        loop.push_back(line);

        Point start = line.point1_;
        Point p2 = line.point2_;
        Point p1;

        while(true) {
            bool found = false;
            vector<Line>::iterator it;
            for(it = lines_.begin(); it != lines_.end(); it++) {
                if(p2 == it->point1_) {
                    p1 = it->point1_;
                    p2 = it->point2_;
                    found = true;
                    break;
                } else if(p2 == it->point2_) {
                    p1 = it->point2_;
                    p2 = it->point1_;
                    found = true;
                    break;
                }
            }

            if(found) {
                lines_.erase(it); 
                loop.push_back(Line(p1, p2));
                if(p2 == start) {
                    break;
                }
            } else {
                return false;
            }
        }

        moveLines(loop);
        mergeLines(loop);
        loops_.push_back(loop);
    }
    return true;
}

void Layer::moveLines(std::vector<Line>& loop)
{
    Line tail = loop.back(); 
    double k1 = tail.slope();

    Line head = loop.front();
    double k2 = head.slope();
    
    if(equal(k1, k2)) {
        int n = 0;
        for(vector<Line>::iterator it = loop.begin(); it != loop.end(); it++) {
            double k = it->slope(); 
            if(equal(k, k1)) {
                n++; 
            } else {
                break;
            }
        }    

        //
        if(n > 0) {
            loop.insert(loop.end(), loop.begin(), loop.begin() + n);
            loop.erase(loop.begin(), loop.begin() + n);
        }
    }

    k1 = loop.front().slope();
    k2 = loop.back().slope();
    wxASSERT(k1 != k2);
}

void Layer::mergeLines(vector<Line>& loop)
{
    vector<Line> nloop; 
    while(!loop.empty()) {
        Line line = loop.front(); 
        double k1 = line.slope();
        Point p1 = line.point1_;
        Point p2 = line.point2_;

        loop.erase(loop.begin());
        int n = 0;
        for(vector<Line>::iterator it = loop.begin(); it != loop.end(); it++) {
            double k2 = it->slope(); 
            if(equal(k1, k2)) {
                p2 = it->point2_;
                n++;
            } else {
                p2 = it->point1_;
                break;
            }
        }

        //
        loop.erase(loop.begin(), loop.begin() + n);
        nloop.push_back(Line(p1, p2));
    }
    loop = nloop;
}

int Layer::createGLList()
{
    srand(time(NULL));
    int id = 1001;

    glNewList(id, GL_COMPILE);
    glBegin(GL_LINES);
    
    // chunks
    for(vector<vector<Line> > ::iterator it = chunks_.begin(); it != chunks_.end(); it++) {
        vector<Line>& chunk = *it;
        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;
        glColor3ub(r, g, b);
        for(vector<Line>::iterator lit = chunk.begin(); lit != chunk.end(); lit++) {
            Line& line = *lit;      
            Point& p1 = line.point1_;
            glVertex3f(p1.x, p1.y, p1.z);
            Point& p2 = line.point2_;
            glVertex3f(p2.x, p2.y, p2.z);
        }
    }    
    
    glColor3f(1, 1, 1);
    // loops
    for(vector<vector<Line> >::iterator it = loops_.begin(); it != loops_.end(); it++) {
        vector<Line>& loop = *it;
        for(vector<Line>::iterator lit = loop.begin(); lit != loop.end(); lit++) {
            Line& line = *lit;
            Point& p1 = line.point1_;
            glVertex3f(p1.x, p1.y, p1.z);
            Point& p2 = line.point2_;
            glVertex3f(p2.x, p2.y, p2.z);
        }
    }

    glEnd();
    glEndList();
    return id;
}

void Layer::createScanlines()
{
    scanlines_.clear();
    double y = miny_ + pitch_;    
    double lasty = miny_;
    while(y < maxy_) {
        pair<int, vector<Line> > pair = createOneScanline(y); 
        int code = pair.first;
        if(code == NOT_SCANLINE) {
            lasty = y;
            y += pitch_;
        } else if(code == SCANLINE) {
            scanlines_.push_back(pair.second); 
            lasty = y;
            y += pitch_;
        } else {
            y = y - pitch_ * 0.01;
            if(y < lasty) {
                break;
            }
            cout << "recreate scan line\n";
        }
    }
}

pair<int, vector<Line> > Layer::createOneScanline(double y)
{
    set<wxString> xset;
    pair<int, vector<Line> > ret;
    for(vector<vector<Line> >::iterator it = loops_.begin(); it != loops_.end(); it++) {
        vector<Line>& loop = *it;
        for(vector<Line>::iterator lit = loop.begin(); lit != loop.end(); lit++) {
            pair<int, double> pair = intersect(y, *lit, loop);
            int code = pair.first;
            if(code == INTERSECTED) {
                double x = pair.second;
                wxString s = wxString::Format(wxT("%.6f"), x);
                xset.insert(s);
            } else if(code == REDO) {
                ret.first = REDO;
                return ret;
            } 
        }
    }
    
    if(!xset.empty()) {
        int n = xset.size();
        wxASSERT(n % 2 == 0);
        
        vector<double> V;
        vector<Line> lines;
        for(set<wxString>::iterator it = xset.begin(); it != xset.end(); it++) {
            double x;
            it->ToDouble(&x);
            V.push_back(x);
        }
        sort(V.begin(), V.end());

        for(int i = 0; i <= n - 2; i += 2) {
            double x1 = V[i];
            double x2 = V[i + 1];
            Point p1(x1, y, z_);
            Point p2(x2, y, z_);
            Line line(p1, p2);
            lines.push_back(line);
        }
        ret.first = SCANLINE;
        ret.second = lines; 
    } else {
        ret.first = NOT_SCANLINE;
    }
    return ret;
}

// code: 0 -- no intersect
//       1 -- intersect
//       2 --- error(redo)
pair<int, double> Layer::intersect(double y, const Line& line, const vector<Line>& loop)
{
    pair<int, double> ret;

    double y1 = line.point1_.y;
    double y2 = line.point2_.y;
    Point p;
    if(isIntersect(y1, y2, y)) {
        int count = 0;
        if(equal(y, y1)) {
            count++;
            p = line.point1_;
        }

        if(equal(y, y2)) {
            count++;
            p = line.point2_;
        }

        if(count == 0) {
            ret.first =  INTERSECTED;
            double x = intersect_0(y, line); 
            ret.second = x;
        } else if(count == 1) {
            bool peak = isPeak(y, p, loop); 
            if(peak) {
                ret.first = NOT_INTERSECTED;
            } else {
                ret.first = INTERSECTED;
                ret.second = p.x;
            }
        } else {
            ret.first = REDO; 
        }
    } else {
        ret.first = NOT_INTERSECTED;
    }
    return ret;
}

double Layer::intersect_0(double y, const Line& line)
{
    double x1 = line.point1_.x;
    double y1 = line.point1_.y;
    double x2 = line.point2_.x;
    double y2 = line.point2_.y;

    if(equal(x1, x2)) {
        return x1;
    } else {
        double x = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
        return x;
    }
}

bool Layer::isPeak(double y, const Point& point, const vector<Line>& loop)
{
    vector<Point> pts;
    for(vector<Line>::const_iterator it = loop.begin(); it != loop.end(); it++) {
        if(point == it->point1_) {
             pts.push_back(it->point2_);       
        } else if(point == it->point2_) {
            pts.push_back(it->point1_);
        }
    }

    double val = (pts[0].y - y) * (pts[1].y - y);
    bool peak = (val > 0.0); 
    if(peak) {
        return true;
    } else {
        return false;
    }

}

bool Layer::isIntersect(double y1, double y2, double y)
{
    double val = (y1 - y) * (y2 - y);
    if(val <= 0.0) {
        return true;
    } else {
        return false;
    }
}

void Layer::calcDimension()
{
    vector<double> ylist;
    for(vector<vector<Line> >::iterator it = loops_.begin(); it != loops_.end(); it++) {
        vector<Line>& loop = *it;
        for(vector<Line>::iterator lit = loop.begin(); lit != loop.end(); lit++) {
            ylist.push_back(lit->point1_.y);
            ylist.push_back(lit->point2_.y);
        }
    } 

    miny_ = *min_element(ylist.begin(), ylist.end());
    maxy_ = *max_element(ylist.begin(), ylist.end());
}

pair<bool, Line> Layer::getOverlapLine(const Line& line, std::vector<Line>& scanline)
{
    pair<bool, Line> ret;
    double y2 = scanline[0].point1_.y;
    double y1 = line.point1_.y;

    double distance = fabs(y2 - y1);
    if(equal(distance, pitch_) || distance < pitch_) {
        for(vector<Line>::iterator it = scanline.begin(); it != scanline.end(); it++) {
            if((it->point1_.x >= line.point2_.x) || (it->point2_.x <= line.point1_.x)) {
            
            } else {
                ret.first = true;
                ret.second = *it;
                scanline.erase(it);
                return ret;
            }
        } 
    }

    ret.first = false;
    return ret;
}

void Layer::createChunks()
{
    chunks_.clear();    
    while(!scanlines_.empty()) {
        vector<Line> chunk;
        vector<Line>& scanline = scanlines_[0];
        Line line = scanline.front();
        scanline.erase(scanline.begin());
        chunk.push_back(line);

        for(int i = 1; i < scanlines_.size(); i++) {
            vector<Line>& scanline = scanlines_[i];
            pair<bool, Line> pair = getOverlapLine(line, scanline); 
            if(pair.first) {
                chunk.push_back(pair.second);
                line = pair.second;
            } else {
                break;
            }
        }
        
        chunks_.push_back(chunk);
        
        vector<vector<Line> > scanlines;
        for(vector<vector<Line> >::iterator it = scanlines_.begin(); it != scanlines_.end(); it++) {
            if(!it->empty()) {
                scanlines.push_back(*it);
            }
        }
        scanlines_ = scanlines;
    }
}

void writeline(const Line& line, ofstream& f)
{
    Point pts[] = {line.point1_, line.point2_};
    f << "<line>";
    for(int i = 0; i < 2; i++) {
        Point& p = pts[i];
        f << "<point>"
          << "<x>" << p.x << "</x>"
          << "<y>" << p.y << "</y>"
          << "<z>" << p.z << "</z>"
          << "</point> ";
    }  
    f << "</line>\n";
}

void Layer::save(ofstream& f)
{
    f << "<layer id=\"" << id_ << "\" height=\"" << z_ << "\">\n";
    f << "<loops num=\"" << loops_.size() << "\">\n";
    int count = 0;
    for(vector<vector<Line> >::iterator it = loops_.begin(); it != loops_.end(); it++) {
        vector<Line>& loop = *it;
        count++;
        f << "<loop id=\"" << count << "\">\n";
        for(vector<Line>::iterator lit = loop.begin(); lit != loop.end(); lit++) {
            writeline(*lit, f); 
        }
        f << "</loop>\n";
    }
    f << "</loops>\n";
    
    f << "<chunks num=\"" << chunks_.size() << "\">\n";
    count = 0;
    for(vector<vector<Line> >::iterator it = chunks_.begin(); it != chunks_.end(); it++) {
        vector<Line>& chunk = *it;
        count++;
        f << "<chunk id=\"" << count << "\">\n";
        for(vector<Line>::iterator lit = chunk.begin(); lit != chunk.end(); lit++) {
            writeline(*lit, f); 
        }
        f << "</chunk>\n";
    }
    f << "</chunks>\n";


    f << "</layer>\n";  
}
