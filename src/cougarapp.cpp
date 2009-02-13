//----------------------------------------------------------------------------
// Author     : Zhigang Liu
// Date       : Feb 2009
// License    : GPL2
// Description: Slice STL CAD file layer by layer
//----------------------------------------------------------------------------
#include "cougarapp.h"
#include "cougarframe.h"

bool CougarApp::OnInit()
{
    CougarFrame *frame = new CougarFrame(wxT("STL CAD slicer"));
    frame->Show(true);
    return true;
}

IMPLEMENT_APP(CougarApp)
