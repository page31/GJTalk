#pragma once
#include "gjcontextwnd.h"
class CSubscriptionFrame :
	public CGJContextWnd
{
private:
	JID m_from;
	CLabelUI *m_pLblTip;
	CButtonUI *m_pBtnAccept;
	CButtonUI *m_pBtnRefuse;
public:
	CSubscriptionFrame(GJContext *context);
	~CSubscriptionFrame(void);

	void SetSubscription(const Subscription& subscription);

	virtual void OnPostCreate();

	virtual void OnFinalMessage( HWND );

	virtual void Notify( TNotifyUI& msg );

};

