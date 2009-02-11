#include "charvalidator.h"

using namespace std;

BEGIN_EVENT_TABLE(CharValidator, wxValidator)
    EVT_CHAR(CharValidator::OnChar)
END_EVENT_TABLE()

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
        wxMessageBox(wxT("This field must contain some text!"), wxT("Error"), wxOK|wxICON_ERROR);
        m_validatorWindow->SetBackgroundColour(wxT("pink"));
        m_validatorWindow->SetFocus();
        m_validatorWindow->Refresh();
        return false;
    } else {
        double val = 0;
        text.ToDouble(&val);
        if(val <= 0) {
            wxMessageBox(wxT("field <= 0"), wxT("Error"), wxOK|wxICON_ERROR); 
            m_validatorWindow->SetBackgroundColour(wxT("pink"));
            m_validatorWindow->SetFocus();
            m_validatorWindow->Refresh();
            return false;
        }
    } 
    
    return true;
}

bool CharValidator::TransferToWindow()
{
    return true;
}

bool CharValidator::TransferFromWindow()
{
    wxString value = ((wxTextCtrl*) m_validatorWindow)->GetValue();
    m_paraMap[m_key] = value;
    return true;
}

void CharValidator::OnChar(wxKeyEvent& event)
{
    int code = event.GetKeyCode(); 
    if(code == wxChar('-') || isalpha(code)) {
        return;
    }
    event.Skip();
}
