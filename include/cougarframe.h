#ifndef cougarframe_h
#define cougarframe_h

#include <wx/wx.h>
#include <wx/splitter.h>
#include "cadmodel.h"

class CougarFrame: public wxFrame
{
public:
    CougarFrame(const wxString& title);

private:
    void OnOpen(wxCommandEvent& event);
    void createMenu();
    void createControls();
    wxSplitterWindow* createSplitter();
    
    DECLARE_EVENT_TABLE()

    //
    Cadmodel m_cadmodel;
};

#endif
