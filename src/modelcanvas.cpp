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

#include "modelcanvas.h"
#include "cadmodel.h"
#include <iostream>

using namespace std;

BEGIN_EVENT_TABLE(Modelcanvas, wxGLCanvas)
    EVT_PAINT(Modelcanvas::OnPaint)
    EVT_SIZE(Modelcanvas::OnSize)
    EVT_LEFT_DOWN(Modelcanvas::OnLeftDown)
    EVT_MOUSE_EVENTS(Modelcanvas::OnMouseEvent)
    EVT_ERASE_BACKGROUND(Modelcanvas::OnEraseBackground)
END_EVENT_TABLE()

Modelcanvas::Modelcanvas(wxWindow* parent, Cadmodel *cadmodel):wxGLCanvas(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    cadmodel_ = cadmodel;
    xangle_ = 0;
    yangle_ = 0;
}

void Modelcanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    if(!GetContext()) {
        return;
    }
    SetCurrent();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);    

    static bool init = false;
    if(!init) {
        setupGLContext();
        init = true;
    }
    showModel();
    showPath();
    glFlush();
    SwapBuffers();
}

void Modelcanvas::OnSize(wxSizeEvent& event)
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

void Modelcanvas::setupProjection()
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
    glOrtho(left, right, bottom, top, 0.0, diameter * 4);
}

void Modelcanvas::showModel()
{
    if(!cadmodel_->isLoaded()) {
        return;
    }     
    
    setupProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -cadmodel_->getDiameter());
    glRotatef(xangle_, 1, 0, 0);
    glRotatef(yangle_, 0, 1, 1);

    glTranslatef(-cadmodel_->getXcenter(), -cadmodel_->getYcenter(), -cadmodel_->getZcenter());
    int gid = cadmodel_->createGLModellist();
    glCallList(gid);
}

void Modelcanvas::OnLeftDown(wxMouseEvent& event)
{
}

void Modelcanvas::OnMouseEvent(wxMouseEvent& event)
{
    static int dragging = 0;
    static float last_x, last_y;

    if(event.LeftIsDown()) {
        if(!dragging) {
            dragging = 1;
        } else {
            yangle_ += (event.GetX() - last_x) * 1.0;
            xangle_ += (event.GetY() - last_y) * 1.0;
            Refresh(false);
        }

        last_x = event.GetX();
        last_y = event.GetY();
    } else {
        dragging = 0;
    }
}

void Modelcanvas::setupGLContext()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat diffuseLight[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat specularLight[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat position[] = {-15.0, 30.0, -40.0, 1.0};
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    GLfloat p[] = {0.2, 0.2, 0.2, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, p);

    GLfloat mcolor[] = {0.0, 0.0, 0.4, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_BACK, GL_LINE);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //glMaterial(GL_FRONT, GL_SHININESS, 50);
}

void Modelcanvas::showPath()
{
    if(!cadmodel_->isSliced()) {
        return;
    }

    int id = cadmodel_->getCurrLayerGLList();
    glCallList(id);
}

void Modelcanvas::OnEraseBackground(wxEraseEvent& event)
{
    //event.Skip();
}
