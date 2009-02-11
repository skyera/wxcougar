#ifndef cougarframe_h
#define cougarframe_h

#include <wx/wx.h>
#include <wx/splitter.h>
#include <map>
#include "cadmodel.h"

class Modelcanvas;
class ControlPanel;

class CougarFrame: public wxFrame
{
public:
    CougarFrame(const wxString& title);

private:
    void OnOpen(wxCommandEvent& event);
    void OnSlice(wxCommandEvent& event);
    void createMenu();
    void createControls();
    wxSplitterWindow* createSplitter();
    void createToolbar();
    
    DECLARE_EVENT_TABLE()
    
    ControlPanel *m_controlPanel;
    //
    Cadmodel m_cadmodel;
    Modelcanvas* m_modelCanvas;
    std::map<wxString, wxString> m_paraMap;
};

#endif
