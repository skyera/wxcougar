#include "layer.h"
#include <wx/wx.h>
#include <iostream>
#include <wx/glcanvas.h>

using namespace std;

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
        m_loops.push_back(loop);
    }
    cout << "no of loops:" << m_loops.size() << endl;
    return true;
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
    glEnd();
    glEndList();
    return id;
}
