#ifndef controlpanel_h
#define controlpanel_h

#include <wx/wx.h>
#include <vector>
#include <utility>

class ControlPanel: public wxPanel
{
public:
    ControlPanel(wxWindow *parent);

private:
    void createControls();
    wxSizer* createDimensions();
    wxSizer* createOneDimension(const wxString& caption, const std::vector<std::pair<wxString, wxString> > &items);

};

#endif
