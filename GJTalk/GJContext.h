#pragma once

#include "GJTalk.h"
#include "GJUser.h"
#include "MainFrame.h"
#include <string>
#include "../xmpp/client.h"
#include "../xmpp/connectionlistener.h"
#include "../xmpp/loghandler.h" 
#include "../xmpp/tlshandler.h"
#include "../xmpp/message.h"
#include "../xmpp/messagesessionhandler.h"
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
class GJContext:public ConnectionListener, LogHandler,
	MessageSessionHandler, TLSHandler
{
public:

	 
	int MainFrameCloseReason;
private:
	JID *m_pSelf; 
	Client *m_pClient;
	CMainFrame *m_pMainFrame;
	string m_Server;
	int m_Port;
public: 
	GJContext(void);
	void setMainFrame(CMainFrame* frame);
	bool init(const string& sever,int port=-1);
	bool signIn(const string& username,const string& password);
	bool isSignedIn() const;
	JID *getSelf() const;

	void onConnect() ;
	void onDisconnect( ConnectionError e );
	bool onTLSConnect( const CertInfo& info );
	void handleEncryptedData( const TLSBase* /*base*/, const std::string& data );
	void handleDecryptedData( const TLSBase* /*base*/, const std::string& data );
	void handleHandshakeResult( const TLSBase* /*base*/, bool success, CertInfo& /*certinfo*/ );
	void handleMessageSession( MessageSession *session );
	void handleLog( LogLevel level, LogArea area, const std::string& message );


	~GJContext(void);
};

