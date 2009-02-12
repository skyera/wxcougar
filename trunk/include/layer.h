#ifndef layer_h
#define layer_h

#include <vector>
#include <utility>
#include "line.h"

class Layer
{
public:
    Layer(double z = 0.0, double pitch = 0.0);
    bool setLines(const std::vector<Line>& lines); 
    bool createLoops();
    int createGLList();

    void moveLines(std::vector<Line>& loop);
    void mergeLines(std::vector<Line>& loop);
    void createScanlines();
    void calcDimension();
    std::pair<int, std::vector<Line> > createOneScanline(double y);
    std::pair<int, double> intersect(double y, const Line& line, const std::vector<Line>& loop);
    bool isIntersect(double y1, double y2, double y);
    double intersect_0(double y, const Line& line);
    bool isPeak(double y, const Point& point, const std::vector<Line>& loop);

    double m_z;
    double m_pitch;
    std::vector<Line> m_lines;
    std::vector<std::vector<Line> > m_loops;
    std::vector<std::vector<Line> > m_scanlines;
    double m_miny;
    double m_maxy;
};

#endif
