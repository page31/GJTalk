#pragma once
#include "gjcontextwnd.h"
#include <map>
#include "..\xmpp\jid.h"
#include <vector>
class CAddBuddyFrame :
	public CGJContextWnd
{
private:
	static map<string,CAddBuddyFrame*> m_opened;

	vector<CString> m_groups;
	CComboBoxUI *m_pCBGroups;
	CEditUI *m_pEditRemark;
	CEditUI *m_pEditMsg;
	CButtonUI *m_pBtnAddGroup;
	JID m_jid;
public:
	CAddBuddyFrame(const JID &jid,GJContext *pContext);
	~CAddBuddyFrame(void);

	static void OpenFor(const JID &jid,GJContext *pContext);

	virtual void OnPostCreate();

	virtual void Notify( TNotifyUI& msg );

	virtual void OnFinalMessage( HWND );

};

