#include "stdafx.h"
#include "ChatFrame.h"


CChatFrame::CChatFrame(GJContext *context)
	:CGJContextWnd(context)
{
	ASSERT(context);
	this->Create(NULL,m_pContext->GetAppName(),UI_WNDSTYLE_FRAME,UI_WNDSTYLE_EX_FRAME);
}

const JID&CChatFrame::GetTarget() const
{  
	return m_jid;
}
void CChatFrame::Notify(TNotifyUI& msg)
{
	if(msg.sType==_T("close"))
	{
		m_pContext->m_SessionManager.OnChatFrameClose(*this);
	}
	CGJContextWnd::Notify(msg);
}
void CChatFrame::InitTarget(const JID& jid)
{
	m_jid=jid;
	this->SetCaptionText(utf8dec(jid.username()));
}
void CChatFrame::OnPostCreate()
{
	this->InitFromXmlFile(_T("ChatFrame.xml"),NULL);

}


void CChatFrame::OnReceiveMessage(const CSessionMessage& msg)
{

}

CChatFrame::~CChatFrame(void)
{

}
