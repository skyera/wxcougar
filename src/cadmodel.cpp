// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "cadmodel.h"
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <iostream>
#include <exception>
#include <algorithm>
#include <fstream>

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
    m_scale = 1.0;
}

bool Cadmodel::open(const wxString& filename)
{
    wxTextFile file;
    if(file.Open(filename)) {
        clearFacets(m_facets);
        clearFacets(m_oldfacets);
        try {
            getSolidline(file);
            while(true) {
                Facet *facet = getOneFacet(file); 
                m_facets.push_back(facet);
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
            
            clearFacets(m_oldfacets);
            for(vector<Facet*>::iterator it = m_facets.begin(); it != m_facets.end(); it++) {
                Facet *facet = *it; 
                Facet *nfacet = new Facet(*facet);
                m_oldfacets.push_back(nfacet);
            }
            m_sliced = false;
            return true;
        }
    } 
    return false;
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
    if(n >= 1 && n <= 2) {
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

void Cadmodel::clearFacets(vector<Facet*> & facets)
{
    for(vector<Facet*>::iterator it = facets.begin(); it != facets.end(); it++) {
        delete *it;
    }
    facets.clear();
}

Cadmodel::~Cadmodel()
{
    clearFacets(m_facets);
    clearFacets(m_oldfacets);
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
    m_minx = min = *min_element(xlist.begin(), xlist.end());
    m_maxx = max = *max_element(xlist.begin(), xlist.end());
    m_xsize = max - min;
    m_xcenter = (min + max) / 2;
    
    //
    m_miny = min = *min_element(ylist.begin(), ylist.end());
    m_maxy = max = *max_element(ylist.begin(), ylist.end());
    m_ysize = max - min;
    m_ycenter = (min + max) / 2;
    
    //
    m_minz = min = *min_element(zlist.begin(), zlist.end());
    m_maxz = max = *max_element(zlist.begin(), zlist.end());
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
    m_height = height;
    m_pitch = pitch;
    m_speed = speed;
    m_direction = direction;
    m_scale = scale;

    scaleModel(m_scale);
    calcDimension();
    createLayers();
    if(!m_layers.empty()) {
        m_sliced = true;
        m_currLayer = 0;
    }
    return true;
}

void Cadmodel::scaleModel(double scale)
{
    clearFacets(m_facets);
    for(vector<Facet*>::iterator it = m_oldfacets.begin(); it != m_oldfacets.end(); it++) {
        Facet *facet = *it;
        Facet *nfacet = new Facet(*facet);
        nfacet->scale(scale);
        nfacet->changeDirection(m_direction);
        m_facets.push_back(nfacet);
    }
}

void Cadmodel::createLayers()
{
    m_layers.clear();
    double z = m_minz + m_height; 
    double lastz = m_minz;
    int count = 0;
    while(z < m_maxz) {
        pair<int, Layer> ret = createOnelayer(z); 
        int cod  = ret.first;
        if(cod == 1) {
            count++;
            ret.second.m_id = count;
            m_layers.push_back(ret.second); 
            lastz = z;
            z += m_height;
        }else if(cod == 2) {
            break;
        } else if(cod == 3) {
            z = z - m_height * 0.01; 
            if(z < lastz) {
                break;
            }
            cout << "recreate layer\n";
        } else {
            lastz = z;
            z += m_height;
        }
    }
    cout << "no of layers:" << m_layers.size() << endl;
}

// code 0 - empty
//      1 - new layer
//      2 - error
//      3 - redo
pair<int, Layer> Cadmodel::createOnelayer(double z)
{
    pair<int, Layer> ret;
    vector<Line> lines;
    for(vector<Facet*>::iterator it = m_facets.begin(); it != m_facets.end(); it++) {
        Facet *facet = *it;
        pair<int, Line> p = facet->intersect(z);
        int code = p.first;
        if(code == -1) {
            ret.first = 3;
            return ret;
        } else if(code == 1) {
            lines.push_back(p.second); 
        }
    } 
    
    if(!lines.empty()) {
        Layer layer(z, m_pitch);
        bool ok = layer.setLines(lines);
        if(ok) {
            ret.first = 1;
            ret.second = layer;
        } else {
            ret.first = 2;
        }
    } else {
        ret.first = 0;
    }

    return ret;
}

int Cadmodel::getCurrLayerGLList()
{
    if(m_sliced) {
        return m_layers[m_currLayer].createGLList();
    } else {
        return 0;
    }
}

double Cadmodel::getCurrLayerHeight()
{
    return m_layers[m_currLayer].m_z;
}

void Cadmodel::nextLayer()
{
    if(m_sliced) {
        m_currLayer = (m_currLayer + 1) % m_layers.size();
    }
}

void Cadmodel::prevLayer()
{
    if(m_sliced) {
        m_currLayer--;
        if(m_currLayer < 0) {
            m_currLayer = m_layers.size() - 1;
        }
    }
}

int Cadmodel::getNoLayers()
{
    return m_layers.size();
}

int Cadmodel::getCurrLayerIndex()
{
    return m_currLayer;
}

void Cadmodel::save(const wxString& filename)
{
    ofstream f(filename.mb_str());    

    f << "<slice>\n"
      << "  <dimension>\n"
      << "    <x>" << m_xsize << "</x>\n"
      << "    <y>" << m_ysize << "</y>\n"
      << "    <z>" << m_zsize << "</z>\n"
      << "  </dimension>\n"
      << "  <para>\n"
      << "     <layerheight>" << m_height << "</layerheight>\n"
      << "     <layerpitch>" << m_pitch << "</layerpitch>\n"
      << "     <speed>" << m_speed << "</speed>\n"
      << "  </para>\n"
      << "  <layers num=\"" << m_layers.size() << "\">\n"; 
    
    for(vector<Layer>::iterator it = m_layers.begin(); it != m_layers.end(); it++) {
        it->save(f); 
    }

    f << "  </layers>\n"
      << "</slice>\n";
}

double Cadmodel::getXsize()
{
    return m_xsize;
}

double Cadmodel::getYsize()
{
    return m_ysize;
}

double Cadmodel::getZsize()
{
    return m_zsize;
}

bool Cadmodel::isLoaded()
{
    return m_loaded;
}

bool Cadmodel::isSliced()
{
    return m_sliced;
}

double Cadmodel::getDiameter()
{
    return m_diameter;
}

double Cadmodel::getXcenter()
{
    return m_xcenter;
}

double Cadmodel::getYcenter()
{
    return m_ycenter;
}

double Cadmodel::getZcenter()
{
    return m_zcenter;
}
