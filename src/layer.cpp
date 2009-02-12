#include "layer.h"
#include <wx/wx.h>
#include <iostream>
#include <wx/glcanvas.h>
#include "utility.h"
#include <algorithm>
#include <set>

using namespace std;
using namespace cougar;

Layer::Layer(double z, double pitch)
{
    m_z = z;
    m_pitch = pitch;
}

bool Layer::setLines(const vector<Line>& lines)
{
    m_lines = lines;
    bool ok = createLoops();
    if(!ok) {
        return false;
    }

    calcDimension();
    createScanlines();
    return true;
}

bool Layer::createLoops()
{
    m_loops.clear();
    while(!m_lines.empty()) {
        Line line = m_lines.back();
        m_lines.pop_back();

        vector<Line> loop;
        loop.push_back(line);

        Point start = line.m_p1;
        Point p2 = line.m_p2;
        Point p1;

        while(true) {
            bool found = false;
            vector<Line>::iterator it;
            for(it = m_lines.begin(); it != m_lines.end(); it++) {
                if(p2 == it->m_p1) {
                    p1 = it->m_p1;
                    p2 = it->m_p2;
                    found = true;
                    break;
                } else if(p2 == it->m_p2) {
                    p1 = it->m_p2;
                    p2 = it->m_p1;
                    found = true;
                    break;
                }
            }

            if(found) {
                m_lines.erase(it); 
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
        m_loops.push_back(loop);
    }
    cout << "no of loops:" << m_loops.size() << endl;
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
        Point p1 = line.m_p1;
        Point p2 = line.m_p2;

        loop.erase(loop.begin());
        int n = 0;
        for(vector<Line>::iterator it = loop.begin(); it != loop.end(); it++) {
            double k2 = it->slope(); 
            if(equal(k1, k2)) {
                p2 = it->m_p2;
                n++;
            } else {
                p2 = it->m_p1;
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
    int id = 1001;

    glColor3f(1, 1, 1);
    glNewList(id, GL_COMPILE);
    glBegin(GL_LINES);
    for(vector<vector<Line> >::iterator it = m_loops.begin(); it != m_loops.end(); it++) {
        vector<Line>& loop = *it;
        for(vector<Line>::iterator lit = loop.begin(); lit != loop.end(); lit++) {
            Line& line = *lit;
            Point& p1 = line.m_p1;
            glVertex3f(p1.x, p1.y, p1.z);
            Point& p2 = line.m_p2;
            glVertex3f(p2.x, p2.y, p2.z);
        }
    }
    
    glColor3f(0, 0, 1);
    for(vector<vector<Line> > ::iterator it = m_scanlines.begin(); it != m_scanlines.end(); it++) {
        vector<Line>& scanlines = *it;
        for(vector<Line>::iterator lit = scanlines.begin(); lit != scanlines.end(); lit++) {
            Line& line = *lit;      
            Point& p1 = line.m_p1;
            glVertex3f(p1.x, p1.y, p1.z);
            Point& p2 = line.m_p2;
            glVertex3f(p2.x, p2.y, p2.z);

        }
    }
    glEnd();
    glEndList();
    return id;
}

void Layer::createScanlines()
{
    m_scanlines.clear();
    double y = m_miny + m_pitch;    
    double lasty = m_miny;
    while(y < m_maxy) {
        pair<int, vector<Line> > pair = createOneScanline(y); 
        int code = pair.first;
        if(code == 0) {
            lasty = y;
            y += m_pitch;
        } else if(code == 1) {
            m_scanlines.push_back(pair.second); 
            lasty = y;
            y += m_pitch;
        } else {
            y = y - m_pitch * 0.01;
            if(y < lasty) {
                break;
            }
            cout << "recreate scan line\n";
        }
    }
    cout << "no of scanlines:" << m_scanlines.size() << endl;
}


// code 0: no scan line
//      1: scan line
//      2: error (redo)
pair<int, vector<Line> > Layer::createOneScanline(double y)
{
    set<wxString> xset;
    pair<int, vector<Line> > ret;
    for(vector<vector<Line> >::iterator it = m_loops.begin(); it != m_loops.end(); it++) {
        vector<Line>& loop = *it;
        for(vector<Line>::iterator lit = loop.begin(); lit != loop.end(); lit++) {
            pair<int, double> pair = intersect(y, *lit, loop);
            int code = pair.first;
            if(code == 1) {
                double x = pair.second;
                wxString s = wxString::Format(wxT("%.6f"), x);
                xset.insert(s);
            } else if(code == 2) {
                ret.first = 2;
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
            Point p1(x1, y, m_z);
            Point p2(x2, y, m_z);
            Line line(p1, p2);
            lines.push_back(line);
        }
        ret.first = 1;
        ret.second = lines; 
    } else {
        ret.first = 0;
    }
    return ret;
}

// code: 0 -- no intersect
//       1 -- intersect
//       2 --- error(redo)
pair<int, double> Layer::intersect(double y, const Line& line, const vector<Line>& loop)
{
    pair<int, double> ret;

    double y1 = line.m_p1.y;
    double y2 = line.m_p2.y;
    Point p;
    if(isIntersect(y1, y2, y)) {
        int count = 0;
        if(equal(y, y1)) {
            count++;
            p = line.m_p1;
        }

        if(equal(y, y2)) {
            count++;
            p = line.m_p2;
        }

        if(count == 0) {
            ret.first =  1;
            double x = intersect_0(y, line); 
            ret.second = x;
        } else if(count == 1) {
            bool peak = isPeak(y, p, loop); 
            if(peak) {
                ret.first = 0;
            } else {
                ret.first = 1;
                ret.second = p.x;
            }
        } else {
            ret.first = 2; 
        }
    } else {
        ret.first = 0;
    }
    return ret;
}

double Layer::intersect_0(double y, const Line& line)
{
    double x1 = line.m_p1.x;
    double y1 = line.m_p1.y;
    double x2 = line.m_p2.x;
    double y2 = line.m_p2.y;

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
        if(point == it->m_p1) {
             pts.push_back(it->m_p2);       
        } else if(point == it->m_p2) {
            pts.push_back(it->m_p1);
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
    for(vector<vector<Line> >::iterator it = m_loops.begin(); it != m_loops.end(); it++) {
        vector<Line>& loop = *it;
        for(vector<Line>::iterator lit = loop.begin(); lit != loop.end(); lit++) {
            ylist.push_back(lit->m_p1.y);
            ylist.push_back(lit->m_p2.y);
        }
    } 

    m_miny = *min_element(ylist.begin(), ylist.end());
    m_maxy = *max_element(ylist.begin(), ylist.end());
}
