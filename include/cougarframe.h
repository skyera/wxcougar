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

    void OnOpen(wxCommandEvent& event);
    void OnSlice(wxCommandEvent& event);
    void OnNextLayer(wxCommandEvent& event);
    void OnPrevLayer(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);

private:
    void createMenu();
    void createControls();
    wxSplitterWindow* createSplitter();
    void createToolbar();
    
    // data
    ControlPanel *control_panel_;
    Cadmodel cadmodel_;
    Modelcanvas* model_canvas_;
    Pathcanvas* path_canvas_;
    std::map<wxString, wxString> param_map_;
    wxString filename_;
    wxStatusBar* statusbar_;
    
    DECLARE_EVENT_TABLE()
};

#endif
