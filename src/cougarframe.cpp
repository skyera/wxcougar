#include "cougarframe.h"
#include "controlpanel.h"
#include <iostream>
#include <string>
#include <wx/splitter.h>
#include "modelcanvas.h"
#include <wx/artprov.h>
#include "paradialog.h"
#include "pathcanvas.h"
#include <wx/aboutdlg.h>
#include <wx/filename.h>

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
    m_statusbar = CreateStatusBar();
    Centre();

    //
    m_paraMap.insert(make_pair(wxT("height"), wxT("1.0")));
    m_paraMap.insert(make_pair(wxT("pitch"), wxT("1.0")));
    m_paraMap.insert(make_pair(wxT("speed"), wxT("10.0")));
    m_paraMap.insert(make_pair(wxT("direction"), wxT("+Z")));
    m_paraMap.insert(make_pair(wxT("scale"), wxT("1.0")));

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
    wxFileDialog dlg(this, caption, wxT("."), wxEmptyString, wildcard, wxOPEN); 
    if(dlg.ShowModal() == wxID_OK) {
        wxString filename = dlg.GetPath();    
        bool ok = m_cadmodel.open(filename);
        if(!ok) {
            wxMessageBox(wxT("cannot open ") + filename, wxT("Error"), wxOK|wxICON_ERROR);
            return;
        }
        
        m_filename = wxFileName(filename).GetName();
        map<wxString, wxString> dmap;
        
        dmap[wxT("oldx")] = wxString::Format(wxT("%f"), m_cadmodel.m_xsize);
        dmap[wxT("oldy")] = wxString::Format(wxT("%f"), m_cadmodel.m_ysize);
        dmap[wxT("oldz")] = wxString::Format(wxT("%f"), m_cadmodel.m_zsize);
        dmap[wxT("newx")] = wxT("");
        dmap[wxT("newy")] = wxT("");
        dmap[wxT("newz")] = wxT("");
        m_controlPanel->setDimension(dmap);
        m_modelCanvas->Refresh();
        m_pathCanvas->Refresh();
    }
}

void CougarFrame::createControls()
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    
    // control panel
    m_controlPanel = new ControlPanel(this);
    sizer->Add(m_controlPanel, 0, wxEXPAND);
    
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
    m_modelCanvas = new Modelcanvas(panel1, &m_cadmodel);
    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
    box->Add(m_modelCanvas, 1, wxEXPAND);
    panel1->SetSizer(box);

    wxPanel *panel2 = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
    m_pathCanvas = new Pathcanvas(panel2, &m_cadmodel);
    wxBoxSizer *box2 = new wxBoxSizer(wxVERTICAL);
    box2->Add(m_pathCanvas, 1, wxEXPAND);
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
    if(!m_cadmodel.m_loaded) {
        return;
    }

    ParaDialog dlg(this, -1, wxT("slice parameters"), m_paraMap);
    if(dlg.ShowModal() == wxID_OK) {
        wxString dir = dlg.getDirection();
        m_paraMap[wxT("direction")] = dir;
        
        wxString s_height = m_paraMap[wxT("height")];
        wxString s_pitch = m_paraMap[wxT("pitch")];
        wxString s_speed = m_paraMap[wxT("speed")];
        wxString s_direction = m_paraMap[wxT("direction")];
        wxString s_scale = m_paraMap[wxT("scale")];

        double height, pitch, speed, scale;
        s_height.ToDouble(&height);
        s_pitch.ToDouble(&pitch);
        s_speed.ToDouble(&speed);
        s_scale.ToDouble(&scale);

        m_cadmodel.slice(height, pitch, speed, s_direction, scale);
        
        map<wxString, wxString> dmap;
        dmap[wxT("newx")] = wxString::Format(wxT("%f"), m_cadmodel.m_xsize);
        dmap[wxT("newy")] = wxString::Format(wxT("%f"), m_cadmodel.m_ysize);
        dmap[wxT("newz")] = wxString::Format(wxT("%f"), m_cadmodel.m_zsize);
        m_controlPanel->setDimension(dmap);
        m_modelCanvas->Refresh();
        m_pathCanvas->Refresh();
        m_controlPanel->setSliceInfo(m_paraMap);
        if(m_cadmodel.m_sliced) {
            m_controlPanel->setNoLayer(m_cadmodel.getNoLayers());
            m_controlPanel->setCurrLayer(m_cadmodel.getCurrLayerIndex());
        }
    }
}

void CougarFrame::OnNextLayer(wxCommandEvent& event)
{
    m_cadmodel.nextLayer();
    m_controlPanel->setCurrLayer(m_cadmodel.getCurrLayerIndex());
    m_modelCanvas->Refresh(false);
    m_pathCanvas->Refresh(false);
}

void CougarFrame::OnPrevLayer(wxCommandEvent& event)
{
    m_cadmodel.prevLayer();
    m_controlPanel->setCurrLayer(m_cadmodel.getCurrLayerIndex());
    m_modelCanvas->Refresh(false);
    m_pathCanvas->Refresh(false);
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
    if(!m_cadmodel.m_sliced) {
        return;
    }
    
    wxString caption = wxT("Save slice info");
    wxString wildcard = wxT("xml files (*.xml)|*.xml|All files(*.*)|*.*");
    wxFileDialog dlg(this, caption, wxT("."), m_filename, wildcard, wxSAVE);
    if(dlg.ShowModal() == wxID_OK) {
        wxString filename = dlg.GetPath();    
        bool ext = wxFileName(filename).HasExt();
        if(!ext) {
            filename += wxT(".xml");
        } 

        m_cadmodel.save(filename);
        m_statusbar->SetStatusText(wxT("slice info is saved in ") + filename, 0);
    }
}
