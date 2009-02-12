#include "layer.h"

using namespace std;

Layer::Layer(double z, double pitch)
{
    m_z = z;
    m_pitch = pitch;
}

bool Layer::setLines(const vector<Line>& lines)
{
    m_lines = lines;
    createLoops();
    return true;
}

bool Layer::createLoops()
{
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
    }
    return true;
}
