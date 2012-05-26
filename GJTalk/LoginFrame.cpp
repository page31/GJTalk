#include "stdafx.h"
#include "LoginFrame.h"
#include "../xmpp/client.h"


CLoginFrame::CLoginFrame(GJContext &context)
	:CGJWnd(context)
{  
	 

}


void CLoginFrame::OnPostCreate()
{
	CDialogBuilder builder;
	CControlUI* pRoot=builder.Create(_T("LoginFrame.xml"),
		0,NULL,&m_pm);
	m_pm.AttachDialog(pRoot); 
}
CLoginFrame::~CLoginFrame(void)
{
}
