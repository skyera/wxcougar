#ifndef layer_h
#define layer_h

#include <vector>
#include "line.h"

class Layer
{
public:
    Layer(double z = 0.0, double pitch = 0.0);
    bool setLines(const std::vector<Line>& lines); 
    bool createLoops();
    int createGLList();

    double m_z;
    double m_pitch;
    std::vector<Line> m_lines;
    std::vector<std::vector<Line> > m_loops;
};

#endif
