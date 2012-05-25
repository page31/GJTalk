#include "stdafx.h"
#include "GJContext.h"


GJContext::GJContext(void)
	:m_pClient(NULL),m_pSelf(NULL),m_pMainFrame(NULL)
{

}

void GJContext::setMainFrame(CMainFrame *frame)
{
	m_pMainFrame=frame;
}

bool GJContext::init(const string& server,int port)
{
	m_Server=server;
	m_Port=port;
	return true;
}
bool GJContext::signIn(const string& username,const string& password)
{ 
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
		m_pClient=new Client(*nSelf,password,m_Port); 

		if(!m_pClient)
			bOk=false;
		else
		{
			m_pClient->registerConnectionListener(this);
			m_pClient->registerMessageSessionHandler(this);
			//m_pClient->registerTagHandler(this);
			//m_pClient->logInstance().registerLogHandler(this);
			bOk= m_pClient->connect(true);
		}
	}
	if(!bOk&&nSelf) 
		delete nSelf;
	else 
	{
		if(m_pSelf)
			delete m_pSelf;
		m_pSelf=nSelf; 
		 
	}

	return bOk;
}
JID *GJContext::getSelf() const
{
	if(!isSignedIn())
		return NULL;
	return m_pSelf;
}
bool GJContext::isSignedIn() const
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
     
      //m_tls->handshake();
    
    }

     void GJContext::onDisconnect( ConnectionError e )
    {
    
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