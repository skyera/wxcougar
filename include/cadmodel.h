#ifndef cadmodel_h
#define cadmodel_h

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/glcanvas.h>
#include <vector>

#include "facet.h"
#include "point.h"
#include "layer.h"

class Cadmodel
{
public:
    Cadmodel();
    virtual ~Cadmodel();
    
    bool open(const wxString& filename);
    bool slice(double height, double pitch, double speed, const wxString& direction, double scale);
    void save(const wxString& filename);
    int createGLModellist();
    int getCurrLayerGLList();
    double getCurrLayerHeight();
    void nextLayer();
    void prevLayer();
    int getNoLayers();
    int getCurrLayerIndex();
    double getXsize();
    double getYsize();
    double getZsize();
    bool isLoaded();
    bool isSliced();
    double getDiameter();
    double getXcenter();
    double getYcenter();
    double getZcenter();
    
private:
    bool getSolidline(wxTextFile& file);
    wxArrayString getLine(wxTextFile& file);
    Facet* getOneFacet(wxTextFile& file);
    Point getNormal(wxTextFile& file);
    void getOuterloop(wxTextFile& file);
    Point getVertex(wxTextFile& file);
    void getEndloop(wxTextFile& file);
    void getEndfacet(wxTextFile& file);
    void clearFacets(std::vector<Facet*>& facets);
    void calcDimension();
    void scaleModel(double scale);
    void createLayers();
    std::pair<int, Layer> createOnelayer(double z);
    
    // data
    bool loaded_;
    bool sliced_;
    wxString line_;
    std::vector<Facet*> facets_;
    std::vector<Facet*> oldfacets_;
    std::vector<Layer> layers_;

    double xsize_;
    double ysize_;
    double zsize_;
    double xcenter_;
    double ycenter_;
    double zcenter_;
    double diameter_;
    
    // Slice parameters
    double scale_;
    double height_;
    double pitch_;
    double speed_;
    wxString direction_;

    double minx_;
    double maxx_;
    double miny_;
    double maxy_;
    double minz_;
    double maxz_;

    int curr_layer_;
};

#endif
