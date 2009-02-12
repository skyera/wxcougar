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
    void setDimension(std::map<wxString, wxString>& dmap);
    void setSliceInfo(const std::map<wxString, wxString>& paraMap);
    void setNoLayer(int no);
    void setCurrLayer(int layer);
private:
    void createControls();
    wxSizer* createDimensions();
    wxSizer* createOneDimension(const wxString& caption, const std::vector<std::pair<wxString, wxString> > &items);
    wxSizer* createSliceInfo();
    std::map<wxString, wxTextCtrl*> m_txtMap;
};

#endif
