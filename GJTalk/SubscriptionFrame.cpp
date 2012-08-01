#include "stdafx.h"
#include "SubscriptionFrame.h"
#include "AcceptSubscriptionFrame.h"


CSubscriptionFrame::CSubscriptionFrame(GJContext *context)
	:CGJContextWnd(context),m_pLblTip(NULL),m_pBtnAccept(NULL),m_pBtnRefuse(NULL)
{
	this->Create(NULL,_T("�����������"),UI_WNDSTYLE_FRAME^WS_THICKFRAME,UI_WNDSTYLE_EX_FRAME);
}


CSubscriptionFrame::~CSubscriptionFrame(void)
{
}

void CSubscriptionFrame::OnPostCreate()
{
	ASSERT(InitFromXmlFile(_T("SubscriptionFrame.xml")));
	m_pLblTip = FindControl<CLabelUI>(_T("lblTip"));
	m_pBtnAccept = FindControl<CButtonUI>(_T("btnAccept"));
	m_pBtnRefuse = FindControl<CButtonUI>(_T("btnRefuse"));

	ASSERT(m_pLblTip&&m_pBtnRefuse&&m_pBtnAccept);

}

void CSubscriptionFrame::OnFinalMessage( HWND )
{
	//delete this;
}

void CSubscriptionFrame::Notify( TNotifyUI& msg )
{
	if(msg.sType==_T("click"))
	{
		if(msg.pSender==m_pBtnAccept)
		{
			CAcceptSubscriptionFrame frame(this->GetContext());
			frame.CenterWindow();
			bool bOk=frame.ShowModal(m_hWnd)==0;
			if(bOk)
			{
				GetContext()->GetClient()->rosterManager()->ackSubscriptionRequest(m_from,true);
				GetContext()->GetClient()->rosterManager()->subscribe(m_from,
					utf8enc(frame.Remark()), SingleStringList(utf8enc(frame.GroupName())));
			} 
		}
		else if(msg.pSender==m_pBtnRefuse)
		{
			GetContext()->GetClient()->rosterManager()->ackSubscriptionRequest(m_from,false);
		}
	}
	if(msg.sType==_T("windowinit"))
	{
		SetWindowText(m_hWnd,_T("�����������"));
	}
	CGJContextWnd::Notify(msg);
}

void CSubscriptionFrame::SetSubscription( const SubscriptionRequest& subscription )
{
	m_from = subscription.from.bare();
	if(m_pLblTip)
	{
		CString strTip;

		strTip.Format(_T("�û� %s ���������Ϊ���ѡ�"),utf8dec(m_from.username()));
		if(!subscription.msg.IsEmpty())
			strTip.AppendFormat(_T("\r\n��֤��Ϣ��%s"),subscription.msg);
		m_pLblTip->SetText(strTip); 
	}
}
