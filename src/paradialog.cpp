// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "paradialog.h"
#include "charvalidator.h"

#include <wx/choice.h>

using namespace std;

ParaDialog::ParaDialog(wxWindow *parent, wxWindowID id, const wxString& title, map<wxString, wxString>& param_map)
    :wxDialog(parent, id, title),param_map_(param_map)
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
        wxString val = param_map_[key];
        
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
            choice_ = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, strs);             
            choice_->SetStringSelection(val);
            flex->Add(choice_,0, wxEXPAND);
        } else {
            wxTextCtrl *txtCtrl = new wxTextCtrl(this, -1, val, wxDefaultPosition, wxSize(80, -1)); 
            flex->Add(txtCtrl, 0, wxEXPAND);
            
            CharValidator valid(key, param_map_);
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
    return choice_->GetStringSelection();
}
