#include "paradialog.h"
#include "charvalidator.h"

#include <wx/choice.h>

using namespace std;

ParaDialog::ParaDialog(wxWindow *parent, wxWindowID id, const wxString& title, map<wxString, wxString>& paraMap)
    :wxDialog(parent, id, title),m_paraMap(paraMap)
{
    createControls();
}

void ParaDialog::createControls()
{
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer *flex = new wxFlexGridSizer(3, 2, 5, 5); 
    sizer->Add(flex, 0, wxEXPAND|wxALL, 10);
    flex->AddGrowableCol(1, 1);

    wxString lbls[] = {wxT("height"), wxT("pitch"), wxT("speed"), wxT("direction"), wxT("scale")};
    for(int i = 0; i < 5; i++) {
        wxString key = lbls[i];
        wxString val = m_paraMap[key];
        
        wxStaticText *label = new wxStaticText(this, -1, key);
        flex->Add(label);
        if(key == wxT("direction")) {
            wxArrayString strs;
            strs.Add(wxT("+X"));
            strs.Add(wxT("-X"));
            strs.Add(wxT("+Y"));
            strs.Add(wxT("-Y"));
            strs.Add(wxT("+Z"));
            strs.Add(wxT("-Z"));
            m_choice = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, strs);             
            m_choice->SetStringSelection(val);
            flex->Add(m_choice,0, wxEXPAND);
        } else {
            wxTextCtrl *txtCtrl = new wxTextCtrl(this, -1, val, wxDefaultPosition, wxSize(80, -1)); 
            flex->Add(txtCtrl, 0, wxEXPAND);
            
            CharValidator valid(key, m_paraMap);
            txtCtrl->SetValidator(valid);
        }
        
    }
    

    // ok, cancel
    wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(box, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton *ok = new wxButton(this, wxID_OK, wxT("&OK"));
    ok->SetDefault();
    box->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
    box->Add(cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    SetSizer(sizer);
    sizer->Fit(this);
}

wxString ParaDialog::getDirection()
{
    return m_choice->GetStringSelection();
}
