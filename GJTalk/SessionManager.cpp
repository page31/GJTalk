#include "stdafx.h"
#include "SessionManager.h"



CSessionMessage::CSessionMessage(const Message& msg)
{
	m_sSubject=utf8dec(msg.subject());
	m_sBody=utf8dec(msg.body());
	m_From=msg.from();
	m_To=msg.to();  
	if(msg.when())
	{
		int year,month,day,hour,min,sec;
		sscanf_s(msg.when()->stamp().c_str(),"%d-%d-%dT%d:%d:%dZ",
			&year,&month,&day,
			&hour,&min,&sec);
		CTime time(year,month,day,hour,min,sec,0); 
		tm timeinfo;
		time.GetLocalTm(&timeinfo);
		TIME_ZONE_INFORMATION tz;
		GetTimeZoneInformation(&tz);
		m_time=mktime(&timeinfo);
		m_time-=tz.Bias*60;
	}
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
	auto iter=m_sessions.find(frame.GetTarget());
	if(iter!=m_sessions.end())
		m_sessions.erase(iter);
}


CChatFrame& CSessionManager::GetChatFrame(const JID& jid)
{
	CChatFrame *frame=NULL;
	auto iter=m_sessions.find(jid);
	if(iter==m_sessions.end())	
	{
		frame=new CChatFrame(m_pContext);
		auto iterVCard=m_pContext->m_VCards.find(jid);

		m_sessions[jid]=frame;
		frame->InitTarget(jid);

		if(iterVCard!=m_pContext->m_VCards.end())
			frame->handleVCard(jid,&iterVCard->second);

		auto sessionIter=m_cachedSessoins.find(jid);
		if(sessionIter!=m_cachedSessoins.end())
		{
			for (auto iter_=sessionIter->second.messages.begin();iter_!=
				sessionIter->second.messages.end();++iter_)
			{
				frame->OnReceiveMessage(*iter_);
			}
			m_cachedSessoins.erase(sessionIter);
			if(m_cachedSessoins.size()==0)
				m_pContext->HandleUnreadMessageChanged();
		}
	}
	else
		frame=iter->second;
	return *frame;
}

CChatFrame& CSessionManager::OpenChatFrame(const JID& jid)
{ 
	CChatFrame &frame=GetChatFrame(jid);
	frame.ShowWindow();
	if(IsIconic(frame))
		ShowWindow(frame,SW_RESTORE);
	::SetForegroundWindow(frame);
	return frame;
}

bool CSessionManager::IsChatFrameOpened(const JID& jid) const
{
	return m_sessions.find(jid)!=m_sessions.end();
}

void CSessionManager::HandleMessage(const Message& msg)
{ 
	auto roster=m_pContext->GetClient()->rosterManager()->getRosterItem(msg.from());

	if(roster) //known buddy
	{
		if(IsChatFrameOpened(msg.from()))
			GetChatFrame(msg.from()).OnReceiveMessage(msg);
		else
		{
			auto iter_s=m_cachedSessoins.find(msg.from());
			SessionItem *item;
			if(iter_s==m_cachedSessoins.end())
			{ 
				m_cachedSessoins[msg.from()]=SessionItem();
				item=&m_cachedSessoins[msg.from()];
			}
			else
			{
				item=&iter_s->second;
			} 

			item->messages.push_back(msg);
			m_pContext->HandleUnreadMessageChanged();
		}
	}
	else// stranger
	{

	}
}