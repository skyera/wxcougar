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


#include <iostream>
#include <string>

#include <wx/splitter.h>
#include <wx/artprov.h>
#include <wx/aboutdlg.h>
#include <wx/filename.h>
#include <wx/busyinfo.h>

#include "cougarframe.h"
#include "modelcanvas.h"
#include "paradialog.h"
#include "pathcanvas.h"
#include "controlpanel.h"

using namespace std;

enum { ID_SLICE = 2000, ID_NEXT, ID_PREV};

BEGIN_EVENT_TABLE(CougarFrame, wxFrame)
    EVT_MENU(wxID_OPEN, CougarFrame::OnOpen)
    EVT_MENU(ID_SLICE, CougarFrame::OnSlice)
    EVT_MENU(ID_NEXT, CougarFrame::OnNextLayer)
    EVT_MENU(ID_PREV, CougarFrame::OnPrevLayer)
    EVT_MENU(wxID_EXIT, CougarFrame::OnExit)
    EVT_MENU(wxID_ABOUT, CougarFrame::OnAbout)
    EVT_MENU(wxID_SAVE, CougarFrame::OnSave)
END_EVENT_TABLE()

CougarFrame::CougarFrame(const wxString& title):
    wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
    createMenu();
    createToolbar();
    createControls();
    statusbar_ = CreateStatusBar();
    statusbar_->SetStatusText(wxT("Welcome"), 0);
    Centre();

    //
    param_map_.insert(make_pair(wxT("height"), wxT("1.0")));
    param_map_.insert(make_pair(wxT("pitch"), wxT("1.0")));
    param_map_.insert(make_pair(wxT("speed"), wxT("10.0")));
    param_map_.insert(make_pair(wxT("direction"), wxT("+Z")));
    param_map_.insert(make_pair(wxT("scale"), wxT("1.0")));

}

void CougarFrame::createMenu()
{
    wxMenuBar *menubar = new wxMenuBar();
    SetMenuBar(menubar);
    
    // File
    wxMenu *filemenu = new wxMenu();
    filemenu->Append(wxID_OPEN, wxT("&Open\tCtrl+O"), wxT("Open file"));
    filemenu->Append(ID_SLICE, wxT("S&lice\tCtrl+L"), wxT("Slice file"));
    filemenu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save slice info"));
    filemenu->AppendSeparator();
    filemenu->Append(wxID_EXIT, wxT("E&xit\tCtrl+Q"), wxT("Quit application"));
    menubar->Append(filemenu, wxT("&File"));
    
    // Edit
    wxMenu *editmenu = new wxMenu();
    editmenu->Append(ID_NEXT, wxT("Next Layer\tpgdn"), wxT("Next layer"));
    editmenu->Append(ID_PREV, wxT("Prev layer\tpgup"), wxT("Previous layer"));
    menubar->Append(editmenu, wxT("&Edit"));

    // Help
    wxMenu *helpmenu = new wxMenu();
    helpmenu->Append(wxID_ABOUT, wxT("About"), wxT("About"));
    menubar->Append(helpmenu, wxT("&Help"));
}

void CougarFrame::OnOpen(wxCommandEvent& event)
{
    wxString caption = wxT("Open a STL cad file");
    wxString wildcard = wxT("STL files (*.stl)|*.stl|All files(*.*)|*.*");
    wxFileDialog dlg(this, caption, wxGetCwd(), wxEmptyString, wildcard, wxOPEN); 
    if(dlg.ShowModal() == wxID_OK) {
        wxString filename = dlg.GetPath();    
        bool ok = cadmodel_.open(filename);
        if(!ok) {
            wxMessageBox(wxT("cannot open ") + filename, wxT("Error"), wxOK|wxICON_ERROR);
            return;
        }
        
        filename_ = wxFileName(filename).GetName();
        map<wxString, wxString> dmap;
        
        dmap[wxT("oldx")] = wxString::Format(wxT("%f"), cadmodel_.getXsize());
        dmap[wxT("oldy")] = wxString::Format(wxT("%f"), cadmodel_.getYsize());;
        dmap[wxT("oldz")] = wxString::Format(wxT("%f"), cadmodel_.getZsize());
        dmap[wxT("newx")] = wxT("");
        dmap[wxT("newy")] = wxT("");
        dmap[wxT("newz")] = wxT("");
        control_panel_->setDimension(dmap);
        model_canvas_->Refresh();
        path_canvas_->Refresh();
        statusbar_->SetStatusText(wxT("open ") + filename, 0);
    }
}

void CougarFrame::createControls()
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    
    // control panel
    control_panel_ = new ControlPanel(this);
    sizer->Add(control_panel_, 0, wxEXPAND);
    
    // splitter
    wxSplitterWindow *splitter = createSplitter();
    sizer->Add(splitter, 1, wxEXPAND);
    SetSizer(sizer);
}

wxSplitterWindow* CougarFrame::createSplitter()
{
    wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxPoint(0, 0), wxSize(400, 400), wxSP_3D);
    
    // model canvas
    wxPanel *panel1 = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
    model_canvas_ = new Modelcanvas(panel1, &cadmodel_);
    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
    box->Add(model_canvas_, 1, wxEXPAND);
    panel1->SetSizer(box);

    wxPanel *panel2 = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
    path_canvas_ = new Pathcanvas(panel2, &cadmodel_);
    wxBoxSizer *box2 = new wxBoxSizer(wxVERTICAL);
    box2->Add(path_canvas_, 1, wxEXPAND);
    panel2->SetSizer(box2);

    splitter->Initialize(panel1);
    splitter->SplitVertically(panel1, panel2, 300);
    splitter->SetMinimumPaneSize(10);
    return splitter;
}

void CougarFrame::createToolbar()
{
    wxToolBar *toolbar = CreateToolBar();
    wxBitmap open_bmp = wxArtProvider::GetBitmap(wxART_FILE_OPEN);
    wxBitmap slice_bmp = wxArtProvider::GetBitmap(wxART_CDROM);
    wxBitmap save_bmp = wxArtProvider::GetBitmap(wxART_FILE_SAVE);
    wxBitmap next_bmp = wxArtProvider::GetBitmap(wxART_GO_DOWN);
    wxBitmap prev_bmp = wxArtProvider::GetBitmap(wxART_GO_UP);
    wxBitmap help_bmp = wxArtProvider::GetBitmap(wxART_HELP);
    wxBitmap quit_bmp = wxArtProvider::GetBitmap(wxART_QUIT);
    
    toolbar->AddTool(wxID_OPEN, wxT("open"), open_bmp, wxT("open"));
    toolbar->AddTool(ID_SLICE, wxT("slice"), slice_bmp, wxT("slice"));
    toolbar->AddTool(wxID_SAVE, wxT("save"), save_bmp, wxT("save"));
    toolbar->AddTool(ID_NEXT, wxT("next layer"), next_bmp, wxT("next layer"));
    toolbar->AddTool(ID_PREV, wxT("prev layer"), prev_bmp, wxT("prev layer"));
    toolbar->AddTool(wxID_ABOUT, wxT("about"), help_bmp, wxT("about"));
    toolbar->AddTool(wxID_EXIT, wxT("quit"), quit_bmp, wxT("exit"));

    toolbar->Realize();
}

void CougarFrame::OnSlice(wxCommandEvent& event)
{
    if(!cadmodel_.isLoaded()) {
        return;
    }

    ParaDialog dlg(this, -1, wxT("slice parameters"), param_map_);
    if(dlg.ShowModal() == wxID_OK) {
        wxString dir = dlg.getDirection();
        param_map_[wxT("direction")] = dir;
        
        wxString s_height = param_map_[wxT("height")];
        wxString s_pitch = param_map_[wxT("pitch")];
        wxString s_speed = param_map_[wxT("speed")];
        wxString s_direction = param_map_[wxT("direction")];
        wxString s_scale = param_map_[wxT("scale")];

        double height, pitch, speed, scale;
        s_height.ToDouble(&height);
        s_pitch.ToDouble(&pitch);
        s_speed.ToDouble(&speed);
        s_scale.ToDouble(&scale);
        
        wxWindowDisabler disableAll;
        wxBusyInfo info(wxT("Slicing, please wait..."), this);
        cadmodel_.slice(height, pitch, speed, s_direction, scale);
        
        map<wxString, wxString> dmap;
        dmap[wxT("newx")] = wxString::Format(wxT("%f"), cadmodel_.getXsize());
        dmap[wxT("newy")] = wxString::Format(wxT("%f"), cadmodel_.getYsize());
        dmap[wxT("newz")] = wxString::Format(wxT("%f"), cadmodel_.getZsize());
        control_panel_->setDimension(dmap);
        model_canvas_->Refresh();
        path_canvas_->Refresh();
        control_panel_->setSliceInfo(param_map_);
        if(cadmodel_.isSliced()) {
            control_panel_->setNoLayer(cadmodel_.getNoLayers());
            control_panel_->setCurrLayer(cadmodel_.getCurrLayerIndex());
            statusbar_->SetStatusText(wxT("sliced"), 0);
        }
    }
}

void CougarFrame::OnNextLayer(wxCommandEvent& event)
{
    cadmodel_.nextLayer();
    control_panel_->setCurrLayer(cadmodel_.getCurrLayerIndex());
    model_canvas_->Refresh(false);
    path_canvas_->Refresh(false);
}

void CougarFrame::OnPrevLayer(wxCommandEvent& event)
{
    cadmodel_.prevLayer();
    control_panel_->setCurrLayer(cadmodel_.getCurrLayerIndex());
    model_canvas_->Refresh(false);
    path_canvas_->Refresh(false);
}

void CougarFrame::OnExit(wxCommandEvent& event)
{
    Close();
}

void CougarFrame::OnAbout(wxCommandEvent& event)
{
    wxAboutDialogInfo info;
    
    info.SetName(wxT("wxCougar"));
    info.SetVersion(wxT("0.1"));
    info.SetDescription(wxT("Slice STL CAD file"));
    info.SetCopyright(wxT("2009"));
    info.AddDeveloper(wxT("Zhigang Liu"));
    info.SetLicense(wxT("GPL2"));
    wxAboutBox(info);
}

void CougarFrame::OnSave(wxCommandEvent& event)
{
    if(!cadmodel_.isSliced()) {
        return;
    }
    
    wxString caption = wxT("Save slice info");
    wxString wildcard = wxT("xml files (*.xml)|*.xml|All files(*.*)|*.*");
    wxFileDialog dlg(this, caption, wxT("."), filename_, wildcard, wxSAVE);
    if(dlg.ShowModal() == wxID_OK) {
        wxString filename = dlg.GetPath();    
        bool ext = wxFileName(filename).HasExt();
        if(!ext) {
            filename += wxT(".xml");
        } 

        cadmodel_.save(filename);
        statusbar_->SetStatusText(wxT("slice info is saved in ") + filename, 0);
    }
}
