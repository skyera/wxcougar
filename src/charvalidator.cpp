#include "charvalidator.h"

using namespace std;

CharValidator::CharValidator(const wxString& key, map<wxString, wxString>& paraMap):m_paraMap(paraMap), m_key(key)
{
    
}

wxObject* CharValidator::Clone() const
{
    return new CharValidator(m_key, m_paraMap);
}

bool CharValidator::Validate(wxWindow* parent)
{
    wxString text = ((wxTextCtrl*) m_validatorWindow)->GetValue();
    if(text == wxT("")) {
        wxMessageBox(wxT("empty field"), wxT("Error"), wxOK|wxICON_ERROR);
        return false;
    }
    return true;
}

bool CharValidator::TransferToWindow()
{
    return true;
}

bool CharValidator::TransferFromWindow()
{
    return true;
}
