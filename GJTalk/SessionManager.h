#pragma once
#include "ChatFrame.h"
#include "UIBuddyList.h"
#include "GJContext.h" 
#include <map>
#include <stack>
#include <list>
#include "..\xmpp\jid.h"

using namespace std;

typedef map<JID,CChatFrame*> session_map;
class CSessionMessage;
typedef struct
{
	CBuddyListItem* buddy;
	list<CSessionMessage> messages;
} SessionItem;



class CSessionMessage
{
private:
	CString m_sSubject;
	JID m_From;
	JID m_To;
	CString m_sBody;
public: 
	CSessionMessage(const Message& msg);
	CString Subjiect() const;
	CString Body() const;
	JID From() const;
	JID To() const;
};
class CSessionManager
{
private:
	session_map m_sessions;
	GJContext *m_pContext;
	map<JID,SessionItem> m_cachedSessoins;

public:
	void SetContext( GJContext* pContext);
	CSessionManager();	
	CSessionManager( GJContext *pContext);
	~CSessionManager(void);

	void HandleMessage(const Message& msg);

	void OnChatFrameClose(const CChatFrame& frame);
	bool IsChatFrameOpened(const JID& jid) const;
	CChatFrame& OpenChatFrame(const JID& jid);
	CChatFrame& GetChatFrame( const JID& jid);
};

