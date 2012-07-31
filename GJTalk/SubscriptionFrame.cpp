#include "stdafx.h"
#include "SubscriptionFrame.h"


CSubscriptionFrame::CSubscriptionFrame(GJContext *context)
	:CGJContextWnd(context),m_pLblTip(NULL),m_pBtnAccept(NULL),m_pBtnRefuse(NULL)
{
	this->Create(NULL,_T("��������"),UI_WNDSTYLE_DIALOG,0L);
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

	CGJContextWnd::Notify(msg);
}

void CSubscriptionFrame::SetSubscription( const Subscription& subscription )
{
	m_from = subscription.from();
	if(m_pLblTip)
	{
		CString strTip;

		strTip.Format(_T("�û� %s ���������Ϊ���ѡ�"),utf8dec(m_from.username()));
		if(subscription.status().length()>0)
			strTip.AppendFormat(_T("\r\n��֤��Ϣ��%s"),utf8dec(subscription.status()));
		m_pLblTip->SetText(strTip);
	}
}
