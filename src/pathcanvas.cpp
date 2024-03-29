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

#include "pathcanvas.h"
#include "cadmodel.h"

BEGIN_EVENT_TABLE(Pathcanvas, wxGLCanvas)
    EVT_PAINT(Pathcanvas::OnPaint)
    EVT_SIZE(Pathcanvas::OnSize)
    EVT_ERASE_BACKGROUND(Pathcanvas::OnEraseBackground)
END_EVENT_TABLE()

Pathcanvas::Pathcanvas(wxWindow* parent, Cadmodel *cadmodel):
    wxGLCanvas(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    cadmodel_ = cadmodel;
}

void Pathcanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    if(!GetContext()) {
        return;
    }

    SetCurrent();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    showPath();
    SwapBuffers();
}

void Pathcanvas::OnSize(wxSizeEvent& event)
{
    wxGLCanvas::OnSize(event);
    int w, h;
    GetClientSize(&w, &h);
    if(GetContext()) {
        SetCurrent();
        glViewport(0, 0, w, h);
    }
    Refresh(false);
}

void Pathcanvas::showPath()
{
    if(!cadmodel_->isSliced()) {
        return;
    }

    setupProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    double z = cadmodel_->getCurrLayerHeight();
    glTranslatef(-cadmodel_->getXcenter(), -cadmodel_->getYcenter(), -z);
    int id = cadmodel_->getCurrLayerGLList();
    glCallList(id);

}

void Pathcanvas::setupProjection()
{
    double diameter = cadmodel_->getDiameter();
    int w, h;
    GetClientSize(&w, &h);
    double half = diameter / 2;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double factor = 1;
    double left, right, bottom, top;
    if(w <= h) {
        factor = (double) h / w;
        left = -half;
        right = half;
        bottom = -half * factor;
        top = -bottom;
    } else{
        factor = (double) w / h;
        left = -half * factor;
        right = -left;
        bottom = -half;
        top = half;
    } 
    
    //double near = 0.0;
    //double far = diameter * 4;
    //glOrtho(left, right, bottom, top, near, far);
    glOrtho(left, right, bottom, top, 0.0, diameter * 2);
}

void Pathcanvas::OnEraseBackground(wxEraseEvent& event)
{
    //event.Skip();
}
