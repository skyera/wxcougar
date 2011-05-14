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
    void OnChar(wxKeyEvent& event);

    DECLARE_EVENT_TABLE()
private:
    wxString key_;
    std::map<wxString, wxString>& param_map_;
};

#endif
