#include "controlpanel.h"
#include <wx/stattext.h>

using namespace std;

ControlPanel::ControlPanel(wxWindow* parent):wxPanel(parent)
{
    createControls();
}

void ControlPanel::createControls()
{
    wxBoxSizer *sizer1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizer2 = new wxBoxSizer(wxVERTICAL);
    sizer1->Add(sizer2, 1, wxALL|wxEXPAND, 10);
    SetSizer(sizer1);
    
    wxSizer *sizer3 = createDimensions();
    sizer2->Add(sizer3, 0, wxEXPAND|wxALIGN_CENTER);

}

wxSizer* ControlPanel::createDimensions()
{
    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, wxT("Dimension"));
    wxStaticBoxSizer *sizer = new wxStaticBoxSizer(box, wxHORIZONTAL);
    
    wxString caption(wxT("Original"));
    vector<pair<wxString, wxString> > items;
    items.push_back(make_pair(wxT("X"), wxT("oldx")));
    items.push_back(make_pair(wxT("Y"), wxT("oldy")));
    items.push_back(make_pair(wxT("Z"), wxT("oldz")));
    wxSizer *sizer1 = createOneDimension(caption, items);
    sizer->Add(sizer1, 1, wxEXPAND|wxALL, 2);
    
    caption = wxT("Scaled");
    items.clear();
    items.push_back(make_pair(wxT("X"), wxT("newx")));
    items.push_back(make_pair(wxT("Y"), wxT("newy")));
    items.push_back(make_pair(wxT("Z"), wxT("newz")));
    wxSizer *sizer2 = createOneDimension(caption, items);
    sizer->Add(sizer2, 1, wxEXPAND|wxALL, 2);
    return sizer;
}


wxSizer* ControlPanel::createOneDimension(const wxString& caption, const vector<pair<wxString, wxString> > &items) 
{
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticText *label = new wxStaticText(this, wxID_ANY, caption);
    sizer->Add(label, 0, wxALIGN_CENTER);
    
    wxFlexGridSizer *flex = new wxFlexGridSizer(items.size(), 2, 2, 2);
    vector<pair<wxString, wxString> >::const_iterator it;
    for(it = items.begin(); it != items.end(); it++) {
         wxString label = it->first;
         wxString key = it->second;

         wxStaticText *lblCtrl = new wxStaticText(this, wxID_ANY, label);
         wxTextCtrl *txtCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxTE_READONLY); 

        flex->Add(lblCtrl);
        flex->Add(txtCtrl, 0, wxEXPAND);
    }
    flex->AddGrowableCol(1, 1);
    sizer->Add(flex, 0, wxEXPAND);
    return sizer;
    

    return 0;
}
