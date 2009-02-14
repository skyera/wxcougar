#ifndef pathcanvas_h
#define pathcanvas_h

#include <wx/wx.h>
#include <wx/glcanvas.h>

class Cadmodel;

class Pathcanvas: public wxGLCanvas
{
public:
    Pathcanvas(wxWindow *parent, Cadmodel *cadmodel); 
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);

private:
    void showPath();
    void setupProjection();

    Cadmodel *m_cadmodel;
    DECLARE_EVENT_TABLE()
};

#endif
