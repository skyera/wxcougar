#ifndef cadmodel_h
#define cadmodel_h

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/glcanvas.h>
#include <vector>

#include "facet.h"
#include "point.h"

class Cadmodel
{
public:
    Cadmodel();
    bool open(const wxString& filename);
    virtual ~Cadmodel();
    
    int createGLModellist();
    bool getSolidline(wxTextFile& file);
    wxArrayString getLine(wxTextFile& file);
    Facet* getOneFacet(wxTextFile& file);
    Point getNormal(wxTextFile& file);
    void getOuterloop(wxTextFile& file);
    Point getVertex(wxTextFile& file);
    void getEndloop(wxTextFile& file);
    void getEndfacet(wxTextFile& file);
    void clearFacets();
    void calcDimension();

    bool m_loaded;
    wxString m_line;
    std::vector<Facet*> m_facets;

    double m_xsize;
    double m_ysize;
    double m_zsize;
    double m_xcenter;
    double m_ycenter;
    double m_zcenter;
    double m_diameter;
};

#endif
