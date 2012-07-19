#pragma once
#include "GJContextWnd.h"
#include "UIBuddyList.h"
#include "../xmpp/message.h"
#include "SessionManager.h"
class CSessionMessage;
class CChatFrame :
	public CGJContextWnd
{

protected:
	JID m_jid;
	void OnPostCreate();
public:
	void OnReceiveMessage(const CSessionMessage& msg);
	const JID &GetTarget() const;
	void Notify(TNotifyUI& msg);
	CChatFrame(GJContext *context);
	void InitTarget(const JID& buddy);
	~CChatFrame(void);
};

