// Zhigang Liu
// Feb 2009
#include "cougarapp.h"
#include "cougarframe.h"

bool CougarApp::OnInit()
{
    CougarFrame *frame = new CougarFrame(wxT("STL CAD slicer"));
    frame->Show(true);
    return true;
}

IMPLEMENT_APP(CougarApp)
