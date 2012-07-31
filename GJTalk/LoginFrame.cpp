#include "stdafx.h"
#include "LoginFrame.h"
#include "../xmpp/client.h"


CLoginFrame::CLoginFrame(GJContext *context)
	:CGJContextWnd(context),m_pEditPassword(NULL),
	m_pEditUser(NULL)
{  
	context->m_pLoginFrame=this;
	m_pContext->m_pTrayIcon->ShowIcon();
	m_pContext->m_pTrayIcon->SetIcon(IDR_MAINFRAME);
	this->SetCaptionText(m_pContext->GetAppName());
	this->Create(NULL,m_pContext->GetAppName(),UI_WNDSTYLE_FRAME^WS_THICKFRAME,UI_WNDSTYLE_EX_FRAME); 
}


void CLoginFrame::OnPostCreate()
{
	//long style=::GetWindowLong(m_hWnd,GWL_STYLE);
	//style^=WS_THICKFRAME; 
	//::SetWindowLong(m_hWnd,GWL_STYLE,style);

	if(!this->InitFromXmlFile(_T("LoginFrame.xml")))
		throw CGJWndError::LOAD_ERROR;

	m_pEditUser=static_cast<CEditUI*>(m_pm.FindControl(_T("editUser")));
	m_pEditPassword=static_cast<CEditUI*>(m_pm.FindControl(_T("editPassword")));
	m_pLnkReg=static_cast<CLabelUI*>(m_pm.FindControl(_T("lnkReg")));
	m_pLnkForgotPwd=static_cast<CLabelUI*>(m_pm.FindControl(_T("lnkForgotPwd")));
	if(m_pLnkReg)
		m_pLnkReg->SetTextColor(m_pm.GetDefaultLinkFontColor());
	if(m_pLnkForgotPwd)
		m_pLnkForgotPwd->SetTextColor(m_pm.GetDefaultLinkFontColor());
}
void CLoginFrame::OnConnected()
{ 
	this->Close();
}
void CLoginFrame::OnDisconnected(ConnectionError error)
{
	switch (error)
	{	
	case gloox::ConnNoError:
		break; 
	case gloox::ConnProxyAuthRequired:
		MsgBox(_T("代理服务器需要认证信息。"),_T("登录失败"));
		break;
	case gloox::ConnProxyAuthFailed:

		MsgBox(_T("代理服务器认证失败。"),_T("登录失败"));
		break;
	case gloox::ConnProxyNoSupportedAuth:

		MsgBox(_T("代理服务器没有支持的认证方式。"),_T("登录失败"));
		break; 
	case gloox::ConnConnectionRefused:
		MsgBox(_T("连接服务器超时。"),_T("登录失败"));
		break;
	case gloox::ConnDnsError:
		MsgBox(_T("未能找到服务器。"),_T("登录失败"));
		break;
	case gloox::ConnOutOfMemory:
		break;
	case gloox::ConnNoSupportedAuth:
		break;
	case gloox::ConnTlsFailed:
		break;
	case gloox::ConnTlsNotAvailable:
		break;
	case gloox::ConnCompressionFailed:
		break;
	case gloox::ConnAuthenticationFailed:
		MsgBox(_T("用户名或密码错误。"),_T("登录失败"));
		break;
	case gloox::ConnUserDisconnected:
		break;
	case gloox::ConnNotConnected:
		break;
	default:
		MsgBox(_T("连接错误。"),_T("登录失败"));
		break;
	}
}

void CLoginFrame::Notify(TNotifyUI& msg)
{

	if(msg.pSender)
	{
		CGJWnd::Notify(msg);
		CStdString strName=msg.pSender->GetName();
		if(msg.pSender==m_pLnkReg||msg.pSender==m_pLnkForgotPwd)
		{
			CLabelUI* label= static_cast<CLabelUI*>(msg.pSender);
			if(msg.sType==_T("mouseenter"))
			{
				label->SetFont(2);
				label->SetTextColor(m_pm.GetDefaultLinkHoverFontColor()); 
			}
			else if(msg.sType==_T("mouseleave"))
			{
				label->SetFont(0);
				label->SetTextColor(m_pm.GetDefaultLinkFontColor()); 
			}
		}
		if(msg.sType==_T("click"))
		{  
			if(strName==_T("btnLogin"))
			{
				if(m_pEditUser->GetText().GetLength()==0)
				{
					m_pEditUser->SetFocus();
					return;
				}
				else if(m_pEditPassword->GetText().GetLength()==0)
				{
					m_pEditPassword->SetFocus();
					return;
				}
				else if(!GetContext()->SignIn(
					m_pEditUser->GetText(),
					m_pEditPassword->GetText(),
					this))
				{
					return;
				}

			}
			else if(strName==_T("btnCancel"))
			{
				this->Close(); 
			}
		}
		else if(msg.sType==_T("return"))
		{
			if(strName==_T("editUser"))
				m_pEditPassword->SetFocus();
			else if(strName==_T("editPassword"))
				GetContext()->SignIn(
				m_pEditUser->GetText(),
				m_pEditPassword->GetText(),
				this);

		}
	}
	else
	{   
		if(msg.sType==_T("close"))
		{
			if(!m_pContext->IsSignedIn())
				::PostQuitMessage(0);
		}
		else if(msg.sType==_T("final"))
		{
			m_pContext->OnWindowDestroyed(this);
		}
	}
}

CLoginFrame::~CLoginFrame(void)
{

}
