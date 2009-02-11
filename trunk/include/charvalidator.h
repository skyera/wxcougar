#ifndef charvalidator_h
#define charvalidator_h

#include <wx/wx.h>
#include <map>

class CharValidator: public wxValidator
{
public:
    CharValidator(const wxString& key, std::map<wxString, wxString>& paraMap);
    wxObject* Clone() const;
    bool Validate(wxWindow* parent);
    bool TransferFromWindow();
    bool TransferToWindow();
private:
    wxString m_key;
    std::map<wxString, wxString>& m_paraMap;
};

#endif
