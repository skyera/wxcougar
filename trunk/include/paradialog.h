#ifndef paradialog_h
#define paradialog_h

#include <wx/wx.h>
#include <map>

class wxChoice;
class ParaDialog: public wxDialog
{
public:
    ParaDialog(wxWindow *parent, wxWindowID id, const wxString& title, std::map<wxString, wxString>& paraMap);
    wxString getDirection();
private:
    void createControls();
    std::map<wxString, wxString> &m_paraMap;
    wxChoice *m_choice;
};

#endif
