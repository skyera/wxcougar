#ifndef layer_h
#define layer_h

#include <vector>
#include <utility>
#include <fstream>
#include "line.h"

class Cadmodel;

class Layer
{
    friend class Cadmodel;
public:
    Layer(double z = 0.0, double pitch = 0.0);
    bool setLines(const std::vector<Line>& lines); 
    void save(std::ofstream& f);

private:
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
    void createChunks();
    std::pair<bool, Line> getOverlapLine(const Line& line, std::vector<Line>& scanline);
    
    // data
    double z_;
    double pitch_;
    std::vector<Line> lines_;
    std::vector<std::vector<Line> > loops_;
    std::vector<std::vector<Line> > scanlines_;
    std::vector<std::vector<Line> > chunks_;
    double miny_;
    double maxy_;
    int id_;
};

#endif
