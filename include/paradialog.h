#ifndef paradialog_h
#define paradialog_h

#include <wx/wx.h>

class ParaDialog: public wxDialog
{
public:
    ParaDialog(wxWindow *parent, wxWindowID id, const wxString& title);
};

#endif
