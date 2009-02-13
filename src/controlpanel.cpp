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
    sizer2->Add(sizer3, 0, wxALL|wxEXPAND|wxALIGN_CENTER, 2);
    sizer2->AddStretchSpacer(); 
    wxSizer *sizer4 = createSliceInfo();
    sizer2->Add(sizer4, 0, wxALL|wxEXPAND|wxALIGN_CENTER, 2);
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
         m_txtMap[key] = txtCtrl;

        flex->Add(lblCtrl);
        flex->Add(txtCtrl, 0, wxEXPAND);
    }
    flex->AddGrowableCol(1, 1);
    sizer->Add(flex, 0, wxEXPAND);
    return sizer;
    

    return 0;
}

void ControlPanel::setDimension(map<wxString, wxString> & dmap)
{
    for(map<wxString, wxString>::iterator it = dmap.begin(); it != dmap.end(); it++) {
        wxString key = it->first;
        wxString val = it->second;
        wxTextCtrl* txtCtrl = m_txtMap[key];
        txtCtrl->SetValue(val);
    }
}

wxSizer* ControlPanel::createSliceInfo()
{
    wxStaticBoxSizer *sizer = new wxStaticBoxSizer(new wxStaticBox(this, -1, wxT("Slice Info")), wxVERTICAL); 
    wxFlexGridSizer *flex = new wxFlexGridSizer(6, 2, 2, 2);
    sizer->Add(flex, 1, wxEXPAND);
    
    wxArrayString items;
    items.Add(wxT("height"));
    items.Add(wxT("pitch"));
    items.Add(wxT("speed"));
    items.Add(wxT("direction"));
    items.Add(wxT("scale"));
    items.Add(wxT("No layer"));
    items.Add(wxT("curr layer"));

    for(int i = 0;  i < items.Count(); i++) {
        flex->Add(new wxStaticText(this, -1, items[i]));
        wxTextCtrl *txtCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxTE_READONLY); 
        flex->Add(txtCtrl, 0, wxEXPAND);
        m_txtMap[items[i]] = txtCtrl;
    } 
    flex->AddGrowableCol(1, 1);
    return sizer;
}

void ControlPanel::setSliceInfo(const map<wxString, wxString>& paraMap)
{
    for(map<wxString, wxString>::const_iterator it = paraMap.begin(); it != paraMap.end(); it++) {
        wxString key = it->first;
        m_txtMap[key]->SetValue(it->second);
    }
}

void ControlPanel::setNoLayer(int no)
{
    m_txtMap[wxT("No layer")]->SetValue(wxString::Format(wxT("%d"), no));
}

void ControlPanel::setCurrLayer(int layer)
{
    m_txtMap[wxT("curr layer")]->SetValue(wxString::Format(wxT("%d"), layer));
}
