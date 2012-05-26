#include "stdafx.h"
#include "LoginFrame.h"
#include "../xmpp/client.h"


CLoginFrame::CLoginFrame(GJContext &context)
	:CGJWnd(context),m_pEditPassword(NULL),
	m_pEditUser(NULL)
{  


}


void CLoginFrame::OnPostCreate()
{
	CDialogBuilder builder;
	CControlUI* pRoot=builder.Create(_T("LoginFrame.xml"),
		0,NULL,&m_pm);
	m_pm.AttachDialog(pRoot); 
	m_pEditUser=static_cast<CEditUI*>(m_pm.FindControl(_T("editUser")));
	m_pEditPassword=static_cast<CEditUI*>(m_pm.FindControl(_T("editPassword")));
}

void CLoginFrame::Notify(TNotifyUI& msg)
{
	CGJWnd::Notify(msg);
	if(msg.sType==_T("click"))
	{ 
		CStdString strName=msg.pSender->GetName();
		if(strName==_T("btnLogin"))
		{
			CStdString strUsername=m_pEditUser->GetText();
			CStdString strPasswod=m_pEditPassword->GetText();
			//context.signIn(strUsername.GetData(),strPasswod.GetData());
		}
		else if(strName==_T("btnCancel"))
		{
			this->Close(); 
		}
	}
}

CLoginFrame::~CLoginFrame(void)
{
}
