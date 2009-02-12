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
    m_cadmodel = cadmodel;
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
    SetCurrent();
    glViewport(0, 0, w, h);
    Refresh(false);
}

void Pathcanvas::showPath()
{
    if(!m_cadmodel->m_sliced) {
        return;
    }

    setupProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    double z = m_cadmodel->getCurrLayerHeight();
    glTranslatef(-m_cadmodel->m_xcenter, -m_cadmodel->m_ycenter, -z);
    int id = m_cadmodel->getCurrLayerGLList();
    glCallList(id);

}

void Pathcanvas::setupProjection()
{
    double diameter = m_cadmodel->m_diameter;
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
