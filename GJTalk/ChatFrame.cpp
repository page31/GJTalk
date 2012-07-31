#include "stdafx.h"
#include "ChatFrame.h"
#include <cmath>
#include "MessageFormat.h"


CChatFrame::CChatFrame(GJContext *context)
	:CGJContextWnd(context),m_bVCardInited(false),
	m_pBtnHeader(NULL),m_pTitleLabel(NULL),
	m_pSignatureLabel(NULL),
	m_pMsgRecord(NULL),
	m_pMsgEdit(NULL),
	m_pBtnClose(NULL),
	m_pBtnSend(NULL)
{
	ASSERT(context);
	this->Create(NULL,m_pContext->GetAppName(),UI_WNDSTYLE_FRAME,UI_WNDSTYLE_EX_FRAME);
	if(GetContext()->GetClient())
		GetContext()->GetClient()->registerPresenceHandler(this);
}

CChatFrame::~CChatFrame(void)
{
	if(GetContext()->GetClient())
	{
		GetContext()->GetClient()->removePresenceHandler(this);
		GetContext()->GetVCardManager()->cancelVCardOperations(this);
	}
}

void CChatFrame::OnPostCreate()
{
	this->InitFromXmlFile(_T("ChatFrame.xml"),NULL);

	m_pBtnHeader=static_cast<CButtonUI*>(m_pm.FindControl(_T("buddy_header")));
	m_pTitleLabel=static_cast<CLabelUI*>(m_pm.FindControl(_T("buddy_name")));
	m_pSignatureLabel=static_cast<CLabelUI*>(m_pm.FindControl(_T("buddy_signature")));
	m_pMsgRecord=static_cast<CRichEditUI*>(m_pm.FindControl(_T("msg_record")));
	m_pMsgEdit=static_cast<CRichEditUI*>(m_pm.FindControl(_T("msg_edit")));

	m_pBtnSend=static_cast<CButtonUI*>(m_pm.FindControl(_T("sendbtn")));
	m_pBtnClose=static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));

	ASSERT(m_pBtnHeader);
	ASSERT(m_pTitleLabel);
	ASSERT(m_pSignatureLabel); 
	ASSERT(m_pMsgRecord);
	ASSERT(m_pMsgEdit);


	m_pMsgRecord->SetReadOnly();
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
	else if(msg.sType==_T("click"))
	{
		if(msg.pSender==m_pBtnSend)
		{
			SendChatMessage();
		}
	}
	CGJContextWnd::Notify(msg);
}
void CChatFrame::InitTarget(const JID& jid)
{
	m_jid=jid;
	this->SetCaptionText(utf8dec(jid.username()));

	if(m_vcard.nickname().empty())
		m_vcard.setNickname(m_jid.username());
	OnVCardUpdated();
	if(!m_bVCardInited)
		GetContext()->GetVCardManager()->fetchVCard(m_jid,this); 
}


void CChatFrame::handleVCard( const JID& jid, const VCard* vcard )
{
	if(jid==m_jid&&vcard!=NULL)
	{
		GetContext()->m_VCards[jid]=*vcard;
		m_bVCardInited=true;
		m_vcard=*vcard;
		GetContext()->GetHeaderManager().UpdateHeader(jid,m_vcard.photo().binval);
		OnVCardUpdated();
	} 
}

void CChatFrame::handleVCardResult( VCardContext context, const JID& jid, StanzaError se /*= StanzaErrorUndefined */ )
{

}

void CChatFrame::OnVCardUpdated()
{
	if(m_pTitleLabel)
		m_pTitleLabel->SetText(utf8dec(m_vcard.nickname()));
	SetWindowText(m_hWnd,m_pTitleLabel->GetText());

	if(m_pSignatureLabel)
		m_pSignatureLabel->SetText(utf8dec(m_vcard.desc()));
	auto roster=GetContext()->GetClient()->rosterManager()->getRosterItem(m_jid); 
	if(m_pBtnHeader)
		m_pBtnHeader->SetBkImage(GetContext()->GetHeaderManager().GetHeader(m_jid,
		!roster||roster->online()));
}

void CChatFrame::HandleHeaderUpdate( const CHeaderManager &manager,const gloox::JID& jid )
{
	if(jid==m_jid)
	{
		auto roster=GetContext()->GetClient()->rosterManager()->getRosterItem(m_jid); 
		if(m_pBtnHeader)
			m_pBtnHeader->SetBkImage(GetContext()->GetHeaderManager().GetHeader(m_jid,
			!roster||roster->online()));
	}
}

void CChatFrame::handlePresence( const Presence& presence )
{
	if(m_jid==presence.from())
	{
		if(m_pBtnHeader)
			m_pBtnHeader->SetBkImage(GetContext()->GetHeaderManager().GetHeader(m_jid,
			presence.presence()!=Presence::Unavailable));
	}

}
void CChatFrame::OnReceiveMessage(const CSessionMessage& msg)
{
	auto roster=GetContext()->GetClient()->rosterManager()->getRosterItem(msg.From());
	auto iterVCard=GetContext()->m_VCards.find(msg.From());
	CString strSender;
	if(iterVCard!=GetContext()->m_VCards.end())
		strSender=utf8dec(iterVCard->second.nickname());
	else if(roster)
		strSender=utf8dec(roster->name());
	else
		strSender=utf8dec(msg.From().username());
	AddToRecord(strSender,msg.Body(),_T("ËÎÌו"),12,RGB(0,0,0),msg.Time());
}

void CChatFrame::AddToRecord( LPCTSTR pstrSender,LPCTSTR pstrMsg,LPCTSTR pstrFont,int fontSize,COLORREF color,time_t time,bool self)
{
	ASSERT(m_pMsgRecord);
	CRichEditUI* pRichEdit = m_pMsgRecord;


	CString strTitle;
	strTitle.Format(_T("%s %s\r\n"),pstrSender,GetTimeString(time));


	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR;

	cf.crTextColor = self ? RGB(0,128,64): RGB(0, 0, 255) ;

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);

	pRichEdit->ReplaceSel(strTitle, false);

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);


	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(pstrMsg, false);

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(_T("\r\n"), false);

	cf.crTextColor = RGB(0, 0, 0);
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	pRichEdit->SetParaFormat(pf);

	pRichEdit->EndDown();
}
void CChatFrame::SendChatMessage( LPCTSTR pstrContent,LPCTSTR pstrFont,int fontSize,COLORREF color )
{ 
	Message msg(gloox::Message::Chat,
		m_jid,utf8enc(pstrContent));
	MessageFormat *format=new MessageFormat;

	msg.addExtension(format);
	GetContext()->GetClient()->send(msg);
	AddToRecord(utf8dec(GetContext()->GetSelfVCard()->nickname()),pstrContent,pstrFont,fontSize,color,time(NULL),true);
}

void CChatFrame::SendChatMessage()
{
	CHARFORMAT2 format;
	format.cbSize=sizeof(CHARFORMAT2);
	format.dwMask=CFM_FACE|CFM_SIZE|CFM_COLOR;
	if(m_pMsgEdit->GetTextLength()==0)
		return;
	m_pMsgEdit->GetSelectionCharFormat(format);
	SendChatMessage(m_pMsgEdit->GetText(),format.szFaceName,(int)sqrt((float)format.yHeight) ,format.crTextColor);
	m_pMsgEdit->SetText(_T(""));

}
