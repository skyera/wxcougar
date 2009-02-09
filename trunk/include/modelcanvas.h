#ifndef modelcanvas_h
#define modelcanvas_h

#include <wx/wx.h>
#include <wx/glcanvas.h>

class Cadmodel;

class Modelcanvas: public wxGLCanvas
{
public:
    Modelcanvas(wxWindow* parent, Cadmodel *cadmodel);
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnMouseEvent(wxMouseEvent& event);
    DECLARE_EVENT_TABLE()

private:
    void setupProjection();
    void showModel();
    void setupGLContext();

    Cadmodel *m_cadmodel;
    int m_xangle;
    int m_yangle;

};

#endif
