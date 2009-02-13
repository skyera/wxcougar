#ifndef cougarframe_h
#define cougarframe_h

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/statusbr.h>
#include <map>
#include "cadmodel.h"

class Modelcanvas;
class ControlPanel;
class Pathcanvas;

class CougarFrame: public wxFrame
{
public:
    CougarFrame(const wxString& title);

private:
    void OnOpen(wxCommandEvent& event);
    void OnSlice(wxCommandEvent& event);
    void OnNextLayer(wxCommandEvent& event);
    void OnPrevLayer(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void createMenu();
    void createControls();
    wxSplitterWindow* createSplitter();
    void createToolbar();
    
    DECLARE_EVENT_TABLE()
    
    ControlPanel *m_controlPanel;
    //
    Cadmodel m_cadmodel;
    Modelcanvas* m_modelCanvas;
    Pathcanvas* m_pathCanvas;
    std::map<wxString, wxString> m_paraMap;
    wxString m_filename;
    wxStatusBar *m_statusbar;
};

#endif
