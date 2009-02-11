#ifndef paradialog_h
#define paradialog_h

#include <wx/wx.h>
#include <map>

class ParaDialog: public wxDialog
{
public:
    ParaDialog(wxWindow *parent, wxWindowID id, const wxString& title, std::map<wxString, wxString>& paraMap);

private:
    void createControls();
    std::map<wxString, wxString> &m_paraMap;
};

#endif
