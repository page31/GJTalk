#include "stdafx.h" 
#include "GJContext.h" 
#include "../xmpp/md5.h"
#include "../xmpp/rostermanager.h"
#include "MenuWnd.h"
#include "SubscriptionFrame.h"
UINT RecvProc(LPVOID data)
{
	GJContext *context=static_cast<GJContext*>(data);
	while (context->IsReceiving())
	{
		if(context->GetClient())
			context->GetClient()->recv(100);
		else
			Sleep(100);
	} 
	return 0;
}

CLoginFrame& GJContext::GetLoginFrame()
{
	if(!m_pLoginFrame)
	{
		m_pLoginFrame=new CLoginFrame(this);

	}
	return *m_pLoginFrame;
}
CMainFrame& GJContext::GetMainFrame()
{
	if(!m_pMainFrame)
	{
		m_pMainFrame=new CMainFrame(this);
	}
	return *m_pMainFrame;
}
GJContext::GJContext(void)
	:m_pClient(NULL),m_pSelf(NULL),m_pMainFrame(NULL),
	m_pLoginFrame(NULL),m_bRecvData(false),m_pRecvThread(NULL),
	m_bConnected(false),m_pVCardMgr(NULL)
{

	m_SessionManager.SetContext(this);
	m_pTrayIcon=new CTrayIcon();
	m_pTrayIcon->AddListener(this);
	m_pTrayIcon->SetTooltipText(this->GetAppName()); 
	m_pSearchFrame=new CSearchFrame(this);
}

GJContext::~GJContext(void)
{
	StopRecv();
	if(m_pClient)
		m_pClient->disconnect();
	if(m_pTrayIcon)
		delete m_pTrayIcon;
	if(m_pLoginFrame)
		delete m_pLoginFrame;
	if(m_pMainFrame)
		delete m_pMainFrame;
	if(m_pSearchFrame)
		delete m_pSearchFrame;
}

void GJContext::OnWindowDestroyed(const CGJContextWnd* pWnd)
{
	if(pWnd==m_pLoginFrame)
	{ 
		m_pLoginFrame=NULL;
	}
	else if(pWnd==m_pMainFrame)
	{
		m_pMainFrame=NULL;
	}
	else
		return;
	delete pWnd;
}

CBuddyListUI *GJContext::GetBuddyList() const
{
	return m_pMainFrame?m_pMainFrame->m_pBuddyList:NULL;  
}
CBuddyListItem *GJContext::GetBuddyItemByJid(const JID& jid) const
{
	CBuddyListUI* pList=GetBuddyList();
	return pList?pList->FindBuddyItem(jid):NULL;
}
Client* GJContext::GetClient() const
{
	return m_pClient;
}
CSessionManager &GJContext::GetSessionManager()
{
	return	m_SessionManager;
}
bool GJContext::IsReceiving() const
{
	return m_bRecvData;
}
void GJContext::StartRecv()
{
	if(IsReceiving())
		StopRecv(); 
	m_bRecvData=true;
	m_pRecvThread=AfxBeginThread(RecvProc,this); 
}
void GJContext::StopRecv()
{
	if(!IsReceiving())
		return;
	m_bRecvData=false;
	if(m_pRecvThread)
		WaitForSingleObject(m_pRecvThread->m_hThread,INFINITE);
	m_pRecvThread=NULL;
}

bool GJContext::init(const string& server,int port)
{
	m_Server=server;
	m_Port=port;
	return true;
}
string encryptPassword(const string& password)
{
	string temp;
	gloox::MD5 md5;
	md5.reset();
	md5.feed(password); 
	md5.finalize();
	temp=md5.hex()+password;
	md5.reset();
	md5.feed(temp);
	md5.finalize();
	return md5.hex();
}


CString &GJContext::GetAppName() const
{
	TCHAR buffer[1024];
	::LoadString(AfxGetInstanceHandle(),IDS_APPNAME,buffer,1023);
	return *(new CString(buffer));
}

bool GJContext::SignIn(LPCTSTR username,LPCTSTR password,CLoginFrame *loginFrame)
{
	StopRecv();
	this->m_pLoginFrame=loginFrame;
	bool bOk=true;
	JID *nSelf=new JID();
	if(!nSelf)
	{
		bOk=false;
	}else if(!nSelf->setServer(m_Server)||!nSelf->setUsername(utf8enc(username)))
	{
		bOk=false;
	}else { 
		if(m_pClient) 
			m_pClient->disconnect();  
		m_pClient=new Client(*nSelf,encryptPassword(utf8enc(password)),m_Port);  
		m_pVCardMgr=new VCardManager(m_pClient);
		if(!m_pClient)
			bOk=false;
		else
		{
			m_pClient->registerConnectionListener(this);
			m_pClient->registerIqHandler(this,0);
			m_pClient->registerMessageHandler(this);
			m_pClient->registerSubscriptionHandler(this);
			bOk= m_pClient->connect(false);

		}
	}
	if(!bOk&&nSelf) 
		delete nSelf;
	else 
	{
		if(m_pSelf)
			delete m_pSelf;
		m_pSelf=nSelf; 
		StartRecv();
	}

	return bOk;
}
JID *GJContext::GetSelf() const
{
	if(!IsSignedIn())
		return NULL;
	return m_pSelf;
}
bool GJContext::IsSignedIn() const
{
	return m_pClient&&m_pClient->authed();
}




// handlers
void GJContext::onConnect()
{

	if(IsCrossThread())
	{
		AfxGetApp()->PostThreadMessageW(DM_CROSSTHREAD_NOTIFY,CT_CONNECT,NULL);
		return;
	}
	if(m_bConnected)
		return; 
	m_bConnected=true;
	m_pClient->setPresence(gloox::Presence::Available,0);
	GetVCardManager()->fetchVCard(m_pClient->jid(),this);
	if(m_pLoginFrame) 
		m_pLoginFrame->OnConnected();

}

void GJContext::onDisconnect( ConnectionError e )
{
	StopRecv();
	if(IsCrossThread())
	{
		AfxGetApp()->PostThreadMessageW(DM_CROSSTHREAD_NOTIFY,CT_DISCONNECT,e);
		return;
	} 
	m_bConnected=false;
	if(m_pLoginFrame)
		m_pLoginFrame->OnDisconnected(e);
	if(m_pVCardMgr)
	{
		delete m_pVCardMgr;
		m_pVCardMgr=NULL;
	}
}

bool GJContext::onTLSConnect( const CertInfo& info )
{

	return true;
}
void GJContext::handleEncryptedData( const TLSBase* base, const std::string& data )
{

}

void GJContext::handleDecryptedData( const TLSBase* base, const std::string& data )
{

}

void GJContext::handleHandshakeResult( const TLSBase* base, bool success, CertInfo& certinfo )
{

}
void GJContext::ShowMainPanel()
{
	if(IsSignedIn()&&m_pMainFrame)
		GetMainFrame().Appear();
	else if(!IsSignedIn()&&m_pLoginFrame)
		SetForegroundWindow(*m_pLoginFrame); 
}

void GJContext::ShowMainMenu()
{
	CMenuWnd *pMenu=new CMenuWnd();
	POINT mousePt;
	::GetCursorPos(&mousePt);
	HWND parent=NULL;
	CGJContextWnd *wnd=NULL;
	if(m_pMainFrame&&IsSignedIn())
		wnd=m_pMainFrame;
	else if(m_pLoginFrame)
		wnd=m_pLoginFrame; 
	if(wnd)
	{
		parent=wnd->m_pm.GetPaintWindow();
	}
	pMenu->Init(parent ,mousePt,_T("TrayIconMenu.xml"),this);
	pMenu->Show();
}



void GJContext::OnTrayIconMessage(CTrayIconMessage &msg)
{ 
	if(msg.sMsg==_T("ldown"))
		ShowMainPanel();
	else if(msg.sMsg==_T("rup"))
		ShowMainMenu();
}

void GJContext::handleMessageSession( MessageSession *session )
{

	m_pClient->disposeMessageSession(session);
}

void GJContext::handleMessage( const Message& msg, MessageSession* session )
{
	m_SessionManager.HandleMessage(msg);
}
bool GJContext::handleIq( const IQ& iq )
{
	return true;
}
void GJContext::handleIqID( const IQ& iq, int context )
{

}
void GJContext::handleLog( LogLevel level, LogArea area, const std::string& message )
{ 
}

VCardManager * GJContext::GetVCardManager() const
{
	return m_pVCardMgr;
}

void GJContext::handleVCard( const JID& jid, const VCard* vcard )
{
	if(!m_pClient)
		return;
	if(jid==m_pClient->jid())
	{

		m_vcSelf=*vcard; 
		AfxGetApp()->PostThreadMessageW(DM_CROSSTHREAD_NOTIFY,CT_RECEIVE_SELF_VCARD,NULL);

	}
}

void GJContext::handleVCardResult( VCardContext context, const JID& jid, StanzaError se /*= StanzaErrorUndefined */ )
{ 
}

const VCard * GJContext::GetSelfVCard() const
{
	return &m_vcSelf;
}

void GJContext::onReceiveSelfVCard()
{
	GetMainFrame().ShowWindow();
}

CHeaderManager & GJContext::GetHeaderManager()
{
	return m_headerMgr;
}



void GJContext::HandleUnreadMessageChanged()
{

}

CSearchFrame & GJContext::GetSearchFrame() const
{
	return *m_pSearchFrame;
}

void GJContext::OnMenu( CMenuWnd *pMenu,CControlUI* pSender,LPCTSTR sType )
{
	CString sName=pSender->GetName();
	if(sName==_T("menu_mainPanel"))
	{
		ShowMainPanel();
	}
	else if(sName==_T("menu_exit"))
	{
		PostQuitMessage(0);
	}
}

void GJContext::handleSubscription( const Subscription& subscription )
{ 
	SubscriptionRequest request;

	request.from=subscription.from();
	request.msg=utf8dec(subscription.status());
	OnSubscriptionRequest(request);
}

void GJContext::OnSubscriptionRequest( const SubscriptionRequest &request )
{
	if(IsCrossThread()) 
	{
		AfxGetApp()->PostThreadMessage(DM_CROSSTHREAD_NOTIFY,CT_HANDLE_SUBSCRIPTION,
			(LPARAM)(new SubscriptionRequest(request)));
		return;
	}
	CSubscriptionFrame *frame=new CSubscriptionFrame(this);
	frame->SetSubscription(request);
	frame->CenterWindow();
	frame->ShowWindow(); 
}
