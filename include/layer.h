#ifndef layer_h
#define layer_h

#include <vector>
#include "line.h"

class Layer
{
public:
    Layer(double z, double pitch);
    bool setLines(const std::vector<Line>& lines); 
    bool createLoops();

    double m_z;
    double m_pitch;
    std::vector<Line> m_lines;
};

#endif
