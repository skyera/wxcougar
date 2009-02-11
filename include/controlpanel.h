#ifndef controlpanel_h
#define controlpanel_h

#include <wx/wx.h>
#include <vector>
#include <utility>
#include <map>

class ControlPanel: public wxPanel
{
public:
    ControlPanel(wxWindow *parent);
    void setDimension(std::map<wxString, double>& dmap);
private:
    void createControls();
    wxSizer* createDimensions();
    wxSizer* createOneDimension(const wxString& caption, const std::vector<std::pair<wxString, wxString> > &items);

    std::map<wxString, wxTextCtrl*> m_txtMap;
};

#endif
