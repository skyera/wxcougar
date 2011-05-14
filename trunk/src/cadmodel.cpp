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

#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <iostream>
#include <exception>
#include <algorithm>
#include <fstream>

#include "cadmodel.h"
#include "utility.h"

using namespace std;
using namespace cougar;

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
    FormatError(const string& msg=""):msg_(msg)
    {
         
    }

    virtual const char* what() const throw()
    {
        string s = "FormatError: " + msg_;
        return s.c_str();
    }
    virtual ~FormatError() throw()
    {
    
    }

private:
    string msg_;
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

Cadmodel::Cadmodel():loaded_(false)
{
    sliced_ = false;    
    scale_ = 1.0;
}

bool Cadmodel::open(const wxString& filename)
{
    wxTextFile file;
    if(file.Open(filename)) {
        clearFacets(facets_);
        clearFacets(oldfacets_);
        try {
            getSolidline(file);
            while(true) {
                Facet *facet = getOneFacet(file); 
                facets_.push_back(facet);
            }
        }
        catch(EofError& err) {
            cerr << err.what() << endl; 
        }
        catch(FormatError& err) {
            cerr << err.what() << endl; 
        }

        if(loaded_) {
            cout << "no of facets:" << facets_.size() << endl;
            calcDimension();
            
            clearFacets(oldfacets_);
            for(vector<Facet*>::iterator it = facets_.begin(); it != facets_.end(); it++) {
                Facet *facet = *it; 
                Facet *nfacet = new Facet(*facet);
                oldfacets_.push_back(nfacet);
            }
            sliced_ = false;
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
        line_ = line;
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
            loaded_ = true;
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
        string msg = genMsg("normal", line_);
        throw FormatError(msg);
    }
}

void Cadmodel::getOuterloop(wxTextFile& file)
{
    wxArrayString tokens = getLine(file); 
    int num_tokens = tokens.Count();
    if(num_tokens == 2) {
        
    } else {
        string msg = genMsg("outerloop", line_);
        throw FormatError(msg);
    }
}

Point Cadmodel::getVertex(wxTextFile& file)
{
    wxArrayString tokens = getLine(file);
    int num_tokens = tokens.Count();
    if(num_tokens == 4) {
        wxString sx = tokens[1];
        wxString sy = tokens[2];
        wxString sz = tokens[3];

        double x, y, z;
        sx.ToDouble(&x);
        sy.ToDouble(&y);
        sz.ToDouble(&z);
        return Point(x, y, z);
    } else {
        string msg = genMsg("vertex", line_);
        throw FormatError(msg);
    }
}

void Cadmodel::getEndloop(wxTextFile& file)
{
    wxArrayString tokens = getLine(file);
    int num_tokens = tokens.Count();
    if(num_tokens == 1) {
    
    } else {
        string msg = genMsg("endloop", line_);
        throw FormatError(msg);
    }
}

void Cadmodel::getEndfacet(wxTextFile& file)
{
    wxArrayString tokens = getLine(file); 
}

void Cadmodel::clearFacets(vector<Facet*> & facets)
{
    for(vector<Facet*>::iterator it = facets.begin(); it != facets.end(); ++it) {
        delete *it;
    }
    facets.clear();
}

Cadmodel::~Cadmodel()
{
    clearFacets(facets_);
    clearFacets(oldfacets_);
}

void Cadmodel::calcDimension()
{
    vector<double> xlist, ylist, zlist;
    for(vector<Facet*>::iterator it = facets_.begin(); it != facets_.end(); it++) {
        Facet *facet = *it;
        const vector<Point> &points = facet->points_; 
        for(vector<Point>::const_iterator pit = points.begin(); pit != points.end(); pit++) {
            const Point &p = *pit;
            xlist.push_back(p.x);
            ylist.push_back(p.y);
            zlist.push_back(p.z);
        }
    }
    double min, max;
    minx_ = min = *min_element(xlist.begin(), xlist.end());
    maxx_ = max = *max_element(xlist.begin(), xlist.end());
    xsize_ = max - min;
    xcenter_ = (min + max) / 2;
    
    //
    miny_ = min = *min_element(ylist.begin(), ylist.end());
    maxy_ = max = *max_element(ylist.begin(), ylist.end());
    ysize_ = max - min;
    ycenter_ = (min + max) / 2;
    
    //
    minz_ = min = *min_element(zlist.begin(), zlist.end());
    maxz_ = max = *max_element(zlist.begin(), zlist.end());
    zsize_ = max - min;
    zcenter_ = (min + max) / 2;

    diameter_ = sqrt(xsize_ * xsize_ + ysize_ * ysize_ + zsize_ * zsize_);

    cout << "xsize: " << xsize_ << endl
         << "ysize: " << ysize_ << endl
         << "zsize: " << zsize_ << endl;
}

int Cadmodel::createGLModellist()
{
    int id = 1000;
    glNewList(id, GL_COMPILE);
    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    for(vector<Facet*>::iterator it = facets_.begin(); it != facets_.end(); it++) {
        Facet *facet = *it;
        Point normal = facet->normal_;
        glNormal3f(normal.x, normal.y, normal.z);
        vector<Point>& points = facet->points_;
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
    sliced_ = false;
    height_ = height;
    pitch_ = pitch;
    speed_ = speed;
    direction_ = direction;
    scale_ = scale;

    scaleModel(scale_);
    calcDimension();
    createLayers();
    if(!layers_.empty()) {
        sliced_ = true;
        curr_layer_ = 0;
    }
    return true;
}

void Cadmodel::scaleModel(double scale)
{
    clearFacets(facets_);
    for(vector<Facet*>::iterator it = oldfacets_.begin(); it != oldfacets_.end(); it++) {
        Facet *facet = *it;
        Facet *nfacet = new Facet(*facet);
        nfacet->scale(scale);
        nfacet->changeDirection(direction_);
        facets_.push_back(nfacet);
    }
}

void Cadmodel::createLayers()
{
    layers_.clear();
    double z = minz_ + height_; 
    double lastz = minz_;
    int count = 0;
    while(z < maxz_) {
        pair<int, Layer> ret = createOnelayer(z); 
        int cod  = ret.first;
        if(cod == LAYER) {
            count++;
            ret.second.id_ = count;
            layers_.push_back(ret.second); 
            lastz = z;
            z += height_;
        }else if(cod == ERR) {
            break;
        } else if(cod == REDO) {
            z = z - height_ * 0.01; 
            if(z < lastz) {
                break;
            }
            cout << "recreate layer\n";
        } else { // NOT_LAYER
            lastz = z;
            z += height_;
        }
    }
    cout << "no of layers:" << layers_.size() << endl;
}

pair<int, Layer> Cadmodel::createOnelayer(double z)
{
    pair<int, Layer> ret;
    vector<Line> lines;
    for(vector<Facet*>::iterator it = facets_.begin(); it != facets_.end(); it++) {
        Facet *facet = *it;
        pair<int, Line> p = facet->intersect(z);
        int code = p.first;
        if(code == REDO) {
            ret.first = REDO;
            return ret;
        } else if(code == INTERSECTED) {
            lines.push_back(p.second); 
        }
    } 
    
    if(!lines.empty()) {
        Layer layer(z, pitch_);
        bool ok = layer.setLines(lines);
        if(ok) {
            ret.first = LAYER;
            ret.second = layer;
        } else {
            ret.first = ERR;
        }
    } else {
        ret.first = NOT_LAYER;
    }

    return ret;
}

int Cadmodel::getCurrLayerGLList()
{
    if(sliced_) {
        return layers_[curr_layer_].createGLList();
    } else {
        return 0;
    }
}

double Cadmodel::getCurrLayerHeight()
{
    return layers_[curr_layer_].z_;
}

void Cadmodel::nextLayer()
{
    if(sliced_) {
        curr_layer_ = (curr_layer_ + 1) % layers_.size();
    }
}

void Cadmodel::prevLayer()
{
    if(sliced_) {
        curr_layer_--;
        if(curr_layer_ < 0) {
            curr_layer_ = layers_.size() - 1;
        }
    }
}

int Cadmodel::getNoLayers()
{
    return layers_.size();
}

int Cadmodel::getCurrLayerIndex()
{
    return curr_layer_;
}

void Cadmodel::save(const wxString& filename)
{
    ofstream f(filename.mb_str());    

    f << "<slice>\n"
      << "  <dimension>\n"
      << "    <x>" << xsize_ << "</x>\n"
      << "    <y>" << ysize_ << "</y>\n"
      << "    <z>" << zsize_ << "</z>\n"
      << "  </dimension>\n"
      << "  <para>\n"
      << "     <layerheight>" << height_ << "</layerheight>\n"
      << "     <layerpitch>" << pitch_ << "</layerpitch>\n"
      << "     <speed>" << speed_ << "</speed>\n"
      << "  </para>\n"
      << "  <layers num=\"" << layers_.size() << "\">\n"; 
    
    for(vector<Layer>::iterator it = layers_.begin(); it != layers_.end(); it++) {
        it->save(f); 
    }

    f << "  </layers>\n"
      << "</slice>\n";
}

double Cadmodel::getXsize()
{
    return xsize_;
}

double Cadmodel::getYsize()
{
    return ysize_;
}

double Cadmodel::getZsize()
{
    return zsize_;
}

bool Cadmodel::isLoaded()
{
    return loaded_;
}

bool Cadmodel::isSliced()
{
    return sliced_;
}

double Cadmodel::getDiameter()
{
    return diameter_;
}

double Cadmodel::getXcenter()
{
    return xcenter_;
}

double Cadmodel::getYcenter()
{
    return ycenter_;
}

double Cadmodel::getZcenter()
{
    return zcenter_;
}
