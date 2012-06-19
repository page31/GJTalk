#include "stdafx.h"
#include "SessionManager.h"



CSessionMessage::CSessionMessage(const Message& msg)
{
	m_sSubject =msg.subject().c_str();
	m_sBody=msg.body().c_str();
	m_From=msg.from();
	m_To=msg.to(); 
}
CString CSessionMessage::Subjiect() const
{
	return m_sSubject;
}
CString CSessionMessage::Body() const
{
	return m_sBody;
}
JID CSessionMessage::From() const
{
	return m_From;
}
JID CSessionMessage::To() const
{
	return m_To;
}

CSessionManager::CSessionManager():m_pContext(NULL)
{ 
}

CSessionManager::CSessionManager( GJContext *pContext)
{
	m_pContext=pContext;
}
void CSessionManager::SetContext(GJContext *pContext)
{
	m_pContext=pContext;
}

CSessionManager::~CSessionManager(void)
{
}

void CSessionManager::OnChatFrameClose(const CChatFrame& frame)
{
	auto iter=m_sessions.find(frame.GetTarget()->GetJid().username());
	if(iter!=m_sessions.end())
		m_sessions.erase(iter);
}


CChatFrame& CSessionManager::GetChatFrame(CBuddyListItem& buddy)
{
	CChatFrame *frame=NULL;
	auto iter=m_sessions.find(buddy.GetJid().username());
	if(iter==m_sessions.end())	
	{
		frame=new CChatFrame(m_pContext);
		m_sessions[buddy.GetJid().username()]=frame;
		frame->InitTarget(buddy);
	}
	else
		frame=iter->second;
	return *frame;
}

CChatFrame& CSessionManager::OpenChatFrame(CBuddyListItem& buddy)
{ 
	CChatFrame &frame=GetChatFrame(buddy);
	frame.ShowWindow();
	if(IsIconic(frame))
		ShowWindow(frame,SW_RESTORE);
	::SetForegroundWindow(frame);
	return frame;
}

bool CSessionManager::IsChatFrameOpened(const CBuddyListItem& buddy) const
{
	return m_sessions.find(buddy.GetJid().username())!=m_sessions.end();
}
void CSessionManager::HandleMessage(const Message& msg)
{
	auto buddy=m_pContext->GetBuddyList()->FindBuddyItem(msg.from());
	if(buddy) //known buddy
	{
		if(IsChatFrameOpened(*buddy))
			GetChatFrame(*buddy).OnReceiveMessage(msg);
		else
		{
			auto iter_s=m_cachedSessoins.find(msg.from().username());
			SessionItem item;
			if(iter_s==m_cachedSessoins.end())
			{
				item.buddy=buddy; 
				m_cachedSessoins[msg.from().username()]=item;
			}
			else
			{
				item=iter_s->second;
			}
			/*item.messages.push_back(msg);*/
		}
	}
	else// stranger
	{

	}
}