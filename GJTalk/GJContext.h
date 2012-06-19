#pragma once

#include "GJTalk.h"
#include "GJUser.h"
#include "MainFrame.h"
#include "LoginFrame.h"
#include <string>
#include "../xmpp/client.h"
#include "../xmpp/connectionlistener.h"
#include "../xmpp/loghandler.h" 
#include "../xmpp/tlshandler.h"
#include "../xmpp/messageeventhandler.h"
#include "../xmpp/messagehandler.h"
#include "../xmpp/messagesessionhandler.h"

#include "TrayIcon.h"
#include "UIBuddyList.h"
#include "SessionManager.h"

using namespace std;
using namespace gloox;


namespace  MainFrameCloseReasons
{
	enum
	{
		SignOut,
		Exit,
		SwitchUser
	};
};


class GJContext:
	public ConnectionListener,
	public LogHandler,
	public MessageSessionHandler,
	public MessageHandler,
	public TLSHandler,
	public ITrayIconListener,
	public IqHandler

{
public:
	CTrayIcon* m_pTrayIcon;
	int MainFrameCloseReason;
	CMainFrame *m_pMainFrame;
	CLoginFrame *m_pLoginFrame;
	CSessionManager m_SessionManager;

private:

	bool m_bRecvData;
	JID *m_pSelf; 
	Client *m_pClient;

	string m_Server;
	int m_Port;  
	CWinThread *m_pRecvThread;
public:  
	virtual void handleMessage( const Message& msg, MessageSession* session = 0 );
	void OnWindowDestroyed(const CGJContextWnd* pWnd);

	CBuddyListUI *GetBuddyList() const;
	CLoginFrame& GetLoginFrame();
	CMainFrame& GetMainFrame();

	CBuddyListItem *GetBuddyItemByJid(const JID& jid) const;



	CString &GetAppName() const;

	Client *GetClient() const;
	bool IsReceiving() const;
	void StartRecv();
	void StopRecv();
	bool init(const string& sever,int port=-1);
	bool SignIn(const string& username,const string& password,CLoginFrame *loginFrame);
	bool IsSignedIn() const;
	JID *GetSelf() const;

	void onConnect() ;
	void onDisconnect( ConnectionError e );
	bool onTLSConnect( const CertInfo& info );
	void handleEncryptedData( const TLSBase* /*base*/, const std::string& data );
	void handleDecryptedData( const TLSBase* /*base*/, const std::string& data );
	void handleHandshakeResult( const TLSBase* /*base*/, bool success, CertInfo& /*certinfo*/ );
	void handleMessageSession( MessageSession *session );
	void handleLog( LogLevel level, LogArea area, const std::string& message );
	virtual bool handleIq( const IQ& iq );
	virtual void handleIqID( const IQ& iq, int context );
	virtual void  OnTrayIconMessage(CTrayIconMessage &msg);

	GJContext(void);
	~GJContext(void);
};

