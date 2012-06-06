#include "stdafx.h"
#include "GJContext.h"
#include "../xmpp/md5.h"

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


GJContext::GJContext(void)
	:m_pClient(NULL),m_pSelf(NULL),m_pMainFrame(NULL),
	m_pLoginFrame(NULL),m_bRecvData(false),m_pRecvThread(NULL)
{

}

void GJContext::setMainFrame(CMainFrame *frame)
{
	m_pMainFrame=frame;
}


Client* GJContext::GetClient() const
{
	return m_pClient;
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

bool GJContext::SignIn(const string& username,const string& password,CLoginFrame *loginFrame)
{ 
	if(IsCrossThread())
	{

	}
	StopRecv();
	this->m_pLoginFrame=loginFrame;
	bool bOk=true;
	JID *nSelf=new JID();
	if(!nSelf)
	{
		bOk=false;
	}else if(!nSelf->setServer(m_Server)||!nSelf->setUsername(username))
	{
		bOk=false;
	}else { 
		if(m_pClient) 
			m_pClient->disconnect();  
		m_pClient=new Client(*nSelf,encryptPassword(password),m_Port); 

		if(!m_pClient)
			bOk=false;
		else
		{
			m_pClient->registerConnectionListener(this);
			
			m_pClient->registerMessageSessionHandler(this); 
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

GJContext::~GJContext(void)
{
	if(m_pClient)
		m_pClient->disconnect();
}



// handlers
void GJContext::onConnect()
{

	if(IsCrossThread())
	{
		AfxGetApp()->PostThreadMessageW(DM_CROSSTHREAD_NOTIFY,1,NULL);
		return;
	}
	if(m_pLoginFrame)
		m_pLoginFrame->OnConnected();
	//m_tls->handshake();

}

void GJContext::onDisconnect( ConnectionError e )
{
	if(IsCrossThread())
	{
		AfxGetApp()->PostThreadMessageW(DM_CROSSTHREAD_NOTIFY,2,e);
		return;
	}
	if(m_pLoginFrame)
		m_pLoginFrame->OnDisconnected(e);
}

bool GJContext::onTLSConnect( const CertInfo& info )
{

	return true;
}
void GJContext::handleEncryptedData( const TLSBase* /*base*/, const std::string& data )
{

}

void GJContext::handleDecryptedData( const TLSBase* /*base*/, const std::string& data )
{

}

void GJContext::handleHandshakeResult( const TLSBase* /*base*/, bool success, CertInfo& /*certinfo*/ )
{

}

void GJContext::handleMessageSession( MessageSession *session )
{

	m_pClient->disposeMessageSession(session);
}

void GJContext::handleLog( LogLevel level, LogArea area, const std::string& message )
{ 
}