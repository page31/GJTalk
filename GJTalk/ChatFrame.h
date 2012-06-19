#pragma once
#include "GJContextWnd.h"
#include "UIBuddyList.h"
#include "../xmpp/message.h"
class CChatFrame :
	public CGJContextWnd
{

protected:
	CBuddyListItem *m_pBuddy;
	void OnPostCreate();
public:
	void OnReceiveMessage(const Message& msg);
	CBuddyListItem *GetTarget() const;
	void Notify(TNotifyUI& msg);
	CChatFrame(GJContext *context);
	void InitTarget(CBuddyListItem& buddy);
	~CChatFrame(void);
};

