#include "stdafx.h"
#include "SubscriptionFrame.h"


CSubscriptionFrame::CSubscriptionFrame(GJContext *context)
	:CGJContextWnd(context),m_pLblTip(NULL),m_pBtnAccept(NULL),m_pBtnRefuse(NULL)
{
	this->Create(NULL,_T("好友请求"),UI_WNDSTYLE_DIALOG,0L);
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

		strTip.Format(_T("用户 %s 请求添加您为好友。"),utf8dec(m_from.username()));
		if(subscription.status().length()>0)
			strTip.AppendFormat(_T("\r\n验证信息：%s"),utf8dec(subscription.status()));
		m_pLblTip->SetText(strTip);
	}
}
