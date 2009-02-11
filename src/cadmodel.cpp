#include "cadmodel.h"
#include <wx/textfile.h>
#include <iostream>
#include <exception>
#include <wx/tokenzr.h>
#include <algorithm>

using namespace std;

class EofError: public exception
{
public:
    virtual const char* what() const throw()
    {
        return "End of file";
    }
};

class FormatError: public exception
{
public:
    FormatError(const string& msg=""):m_msg(msg)
    {
         
    }

    virtual const char* what() const throw()
    {
        string s = "FormatError: " + m_msg;
        return s.c_str();
    }
    virtual ~FormatError() throw()
    {
    
    }

private:
    string m_msg;
};

wxArrayString tokenize(const wxString& string)
{
    wxArrayString tokens;
    wxStringTokenizer tokenizer(string);
    while(tokenizer.HasMoreTokens()) {
        wxString tok = tokenizer.GetNextToken();
        tokens.Add(tok);
    }
    return tokens;
}

string genMsg(const string& info, const wxString& line)
{
    return info + string(line.mb_str());
}

Cadmodel::Cadmodel():m_loaded(false)
{
    m_sliced = false;    
}

bool Cadmodel::open(const wxString& filename)
{
    wxTextFile file;
    if(file.Open(filename)) {
        clearFacets();
        try {
            getSolidline(file);
            int c = 0;
            while(true) {
                Facet *facet = getOneFacet(file); 
                m_facets.push_back(facet);
                c++;
            }
        }
        catch(EofError& err) {
            cerr << err.what() << endl; 
        }
        catch(FormatError& err) {
            cerr << err.what() << endl; 
        }

        if(m_loaded) {
            cout << "no of facets:" << m_facets.size() << endl;
            calcDimension();
        }
    } else {
        
    }
    return true;
}

bool Cadmodel::getSolidline(wxTextFile& file)
{
    wxString line = file.GetFirstLine();
    wxArrayString tokens = tokenize(line);
    int n = tokens.Count();
    if(n >= 2) {
        wxString solid = tokens[0];
        if(solid != wxT("solid")) {
            throw FormatError(string(solid.mb_str()));
        }
    } else {
        throw FormatError("first line");
    }
    return true;
}

wxArrayString Cadmodel::getLine(wxTextFile& file)
{
    wxArrayString tokens;
    if(!file.Eof()) {
        wxString line = file.GetNextLine();
        m_line = line;
        wxStringTokenizer tokenizer(line); 
        
        while(tokenizer.HasMoreTokens()) {
            wxString token = tokenizer.GetNextToken();
            tokens.Add(token);
        }
    } else {
        throw EofError(); 
    }
    return tokens;
}

Facet* Cadmodel::getOneFacet(wxTextFile& file)
{
    Point normal = getNormal(file);        
    getOuterloop(file);

    vector<Point> pts;
    for(int i = 0; i < 3; i++) {
        Point p = getVertex(file);        
        pts.push_back(p);
    }

    getEndloop(file);
    getEndfacet(file);
    Facet *facet = new Facet(normal, pts);
    return facet;
}

Point Cadmodel::getNormal(wxTextFile& file)
{
    wxArrayString tokens = getLine(file);
    int n = tokens.Count();
    if(n == 2) {
        wxString t1 = tokens[0];        
        if(t1 == wxT("endsolid")) {
            m_loaded = true;
            throw EofError();
        }
    } else if(n == 5) {
        wxString t1 = tokens[0];
        wxString t2 = tokens[1];
        
        double x, y, z;
        wxString sx = tokens[2];
        wxString sy = tokens[3];
        wxString sz = tokens[4];
        sx.ToDouble(&x);
        sy.ToDouble(&y);
        sz.ToDouble(&z);

        return Point(x, y, z);
    } else {
        string msg = genMsg("normal", m_line);
        throw FormatError(msg);
    }
}

void Cadmodel::getOuterloop(wxTextFile& file)
{
    wxArrayString tokens = getLine(file); 
    int n = tokens.Count();
    if(n == 2) {
        
    } else {
        string msg = genMsg("outerloop", m_line);
        throw FormatError(msg);
    }
}

Point Cadmodel::getVertex(wxTextFile& file)
{
    wxArrayString tokens = getLine(file);
    int n = tokens.Count();
    if(n == 4) {
        wxString sx = tokens[1];
        wxString sy = tokens[2];
        wxString sz = tokens[3];

        double x, y, z;
        sx.ToDouble(&x);
        sy.ToDouble(&y);
        sz.ToDouble(&z);
        return Point(x, y, z);
    } else {
        string msg = genMsg("vertex", m_line);
        throw FormatError(msg);
    }
}

void Cadmodel::getEndloop(wxTextFile& file)
{
    wxArrayString tokens = getLine(file);
    int n = tokens.Count();
    if(n == 1) {
    
    } else {
        string msg = genMsg("endloop", m_line);
        throw FormatError(msg);
    }
}

void Cadmodel::getEndfacet(wxTextFile& file)
{
    wxArrayString tokens = getLine(file); 
}

void Cadmodel::clearFacets()
{
    for(vector<Facet*>::iterator it = m_facets.begin(); it != m_facets.end(); it++) {
        delete *it;
    }
    m_facets.clear();
}

Cadmodel::~Cadmodel()
{
    clearFacets();
}

void Cadmodel::calcDimension()
{
    vector<double> xlist, ylist, zlist;
    for(vector<Facet*>::iterator it = m_facets.begin(); it != m_facets.end(); it++) {
        Facet *facet = *it;
        const vector<Point> &points = facet->points; 
        for(vector<Point>::const_iterator pit = points.begin(); pit != points.end(); pit++) {
            const Point &p = *pit;
            xlist.push_back(p.x);
            ylist.push_back(p.y);
            zlist.push_back(p.z);
        }
    }
    double min, max;
    min = *min_element(xlist.begin(), xlist.end());
    max = *max_element(xlist.begin(), xlist.end());
    m_xsize = max - min;
    m_xcenter = (min + max) / 2;
    
    //
    min = *min_element(ylist.begin(), ylist.end());
    max = *max_element(ylist.begin(), ylist.end());
    m_ysize = max - min;
    m_ycenter = (min + max) / 2;
    
    //
    min = *min_element(zlist.begin(), zlist.end());
    max = *max_element(zlist.begin(), zlist.end());
    m_zsize = max - min;
    m_zcenter = (min + max) / 2;

    m_diameter = sqrt(m_xsize * m_xsize + m_ysize * m_ysize + m_zsize * m_zsize);

    cout << "xsize: " << m_xsize << endl
         << "ysize: " << m_ysize << endl
         << "zsize: " << m_zsize << endl;
}

int Cadmodel::createGLModellist()
{
    int id = 1000;
    glNewList(id, GL_COMPILE);
    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    for(vector<Facet*>::iterator it = m_facets.begin(); it != m_facets.end(); it++) {
        Facet *facet = *it;
        Point normal = facet->normal;
        glNormal3f(normal.x, normal.y, normal.z);
        vector<Point>& points = facet->points;
        for(vector<Point>::iterator pit = points.begin(); pit != points.end(); pit++) {
            Point p = *pit;
            glVertex3f(p.x, p.y, p.z);
        }
    }
    glEnd();
    glEndList();
    return id;
}

bool Cadmodel::slice(double height, double pitch, double speed, const wxString& direction, double scale)
{
    m_sliced = false;
    return true;
}

void Cadmodel::scaleModel(double scale)
{

}
