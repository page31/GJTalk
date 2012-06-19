#include "stdafx.h"
#include "ChatFrame.h"


CChatFrame::CChatFrame(GJContext *context)
	:CGJContextWnd(context),m_pBuddy(NULL)
{
	ASSERT(context);
	this->Create(NULL,m_pContext->GetAppName(),UI_WNDSTYLE_FRAME,UI_WNDSTYLE_EX_FRAME);
}

CBuddyListItem *CChatFrame::GetTarget() const
{  
	return m_pBuddy;
}
void CChatFrame::Notify(TNotifyUI& msg)
{
	if(msg.sType==_T("close"))
	{
		m_pContext->m_SessionManager.OnChatFrameClose(*this);
	}
	CGJContextWnd::Notify(msg);
}
void CChatFrame::InitTarget(CBuddyListItem& buddy)
{
	m_pBuddy=&buddy;
	this->SetCaptionText(buddy.GetName());
}
void CChatFrame::OnPostCreate()
{
	this->InitFromXmlFile(_T("ChatFrame.xml"),NULL);

}


void CChatFrame::OnReceiveMessage(const Message& msg)
{

}

CChatFrame::~CChatFrame(void)
{

}
