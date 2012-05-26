#include "stdafx.h"
#include "MainFrame.h"


CMainFrame::CMainFrame(GJContext &context)
	:CGJWnd(context)
{
	context.setMainFrame(this);
}

void CMainFrame::OnPostCreate()
{

}
CMainFrame::~CMainFrame(void)
{
}
