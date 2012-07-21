#include "stdafx.h"
#include "MainFrame.h"
#include "../xmpp/rostermanager.h" 
#include "../xmpp/base64.h"
#include <afx.h>

#define FIX_DOCK_P(d) { if(d>DOCK_MOVE_STEP) d=DOCK_MOVE_STEP;}
#define FIX_DOCK_N(d) { if(d<-DOCK_MOVE_STEP) d=-DOCK_MOVE_STEP;}


CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	CControlUI* pControl=NULL;
	if(_tcsicmp(pstrClass,_T("Friends"))==0)
	{
		pControl= new CBuddyListUI(&m_pm);
	}
	else if(_tcsicmp(pstrClass,_T("FriendGroup"))==0)
	{
		pControl= new CBuddyListUI(&m_pm);
	}
	return pControl;
}



void CMainFrame::OnTrayIconMessage(CTrayIconMessage &msg)
{
	if(msg.pMsg==_T("ldown"))
	{
		if(IsWindowVisible(this->m_hWnd))
		{ 
			if(::IsIconic(this->m_hWnd))
				::ShowWindow(this->m_hWnd,SW_RESTORE);
			SetForegroundWindow(this->m_hWnd);
		}
		else
		{
			::ShowWindow(this->m_hWnd,SW_SHOW);
		}
		if(m_Dock!=DOCK_NONE)
		{
			StopAnimateDock();
			RECT rc,rcWork;
			GetWindowRect(*this,&rc);
			GetWorkArea(&rcWork);
			if(DOCK_TOP==m_Dock&& rc.top<0)
			{
				SetWindowPos(*this,NULL,rc.left,0,0,0,SWP_NOSIZE|SWP_NOZORDER);
			}
			else if(DOCK_LEFT==m_Dock&&rc.left<rcWork.left) 
			{
				SetWindowPos(*this,NULL,0,rc.top,0,0,SWP_NOSIZE|SWP_NOZORDER);
			}
			else if(DOCK_RIGHT==m_Dock&&rc.right>rcWork.right)
			{
				SetWindowPos(*this,NULL,rcWork.right-(rc.right-rc.left),rc.top,0,0,SWP_NOSIZE|SWP_NOZORDER); 
			}
			StopAnimateDock();

		}

	}
}

CMainFrame::CMainFrame(GJContext *context)
	:CGJContextWnd(context)
{
	context->m_pMainFrame=this;
	context->m_pTrayIcon->AddListener(this); 
	m_bMoving=false;
	this->SetCaptionText(m_pContext->GetAppName());
	this->Create(NULL,m_pContext->GetAppName(),UI_WNDSTYLE_EX_FRAME,WS_EX_OVERLAPPEDWINDOW|WS_EX_TOPMOST);
	context->GetHeaderManager().RegisterHandler(this);
}

void CMainFrame::OnPostCreate()
{
	if(!this->InitFromXmlFile(_T("MainFrame.xml"),this))
		throw CGJWndError::LOAD_ERROR;  
	m_pBuddyList= static_cast<CBuddyListUI*>(m_pm.FindControl(pstrBuddyListName));
	m_pRecentList=static_cast<CBuddyListUI*>(m_pm.FindControl(pstrRecentBuddyListName));
	m_pOptTabBuddyList=static_cast<COptionUI*>(m_pm.FindControl(pstrOptTabBuddyListName));
	m_pOptTabRecentList=static_cast<COptionUI*>(m_pm.FindControl(pstrOptTabRecentListName));
	m_pListTable=static_cast<CTabLayoutUI*>(m_pm.FindControl(pstrListTableName));

	m_pBtnHeader=static_cast<CButtonUI*>(m_pm.FindControl(pstrButtonHeaderName));
	m_pBtnSearch=FindControl<CButtonUI>(pstrButtonSearchName);


	m_pEditSignaure=static_cast<CEditUI*>(m_pm.FindControl(pstrEditSignatureName));
	m_pLabelName=static_cast<CLabelUI*>(m_pm.FindControl(pstrLabelNameName));




	ASSERT(m_pOptTabBuddyList);
	ASSERT(m_pOptTabRecentList);
	ASSERT(m_pListTable);
	ASSERT(m_pBuddyList);
	ASSERT(m_pRecentList);
	ASSERT(m_pBtnHeader);
	ASSERT(m_pEditSignaure);
	ASSERT(m_pLabelName);

	m_pBuddyList->OnItemAction+= CDelegate<CMainFrame,CMainFrame>(this,&CMainFrame::OnBuddyItemAction);

	/*m_pBuddyList->AddGroup(_T("test"));
	m_pBuddyList->AddGroup(_T("test2"));
	CBuddyListItem *item;

	for (int i = 0; i < 10; i++)
	{
	item=new CBuddyListItem(); 
	CString str;
	str.Format(_T("item %d"),i);
	item->SetName(str);
	item->SetHeader(_T("header.jpg"));
	str.Format(_T("signature %d"),i);
	item->SetSignature(str);
	JID jid;
	jid.setUsername(cstr_str(str));
	item->SetJid(jid);

	m_pBuddyList->GetGroup(i%m_pBuddyList->GetCount()).Add(*item);
	}*/
	ASSERT(GetContext());
	LoadUser();
	GetContext()->GetClient()->rosterManager()->registerRosterListener(this);  
	ASSERT(GetContext()->GetSelfVCard());
	auto vcard=GetContext()->GetSelfVCard();
	m_pLabelName->SetText( utf8dec(vcard->nickname()));
	m_pEditSignaure->SetText(utf8dec(vcard->desc()));
	m_pBtnHeader->SetBkImage(GetContext()->GetHeaderManager().GetHeader(*GetContext()->GetSelf(),true));
	UpdateDock();
}
void CMainFrame::UpdateDock(LPRECT pRect)
{
	RECT rc;
	POINT mp;
	::GetCursorPos(&mp);
	if(!pRect) 
		GetWindowRect(*this,&rc); 
	else 
		rc=*pRect; 


	RECT rcWork;
	GetWorkArea(&rcWork);
	if(mp.y-rcWork.top<10)
	{
		m_Dock=DOCK_TOP;
		OffsetRect(&rc,0,rcWork.top-rc.top);
	}
	else if(rcWork.right-mp.x<10)
	{
		m_Dock=DOCK_RIGHT;
		OffsetRect(&rc,rcWork.right-rc.right,0);
	}
	else if(mp.x-rcWork.left<10)
	{
		m_Dock=DOCK_LEFT;
		OffsetRect(&rc,rcWork.left-rc.left,0);
	}
	else
	{
		m_Dock=DOCK_NONE;
	}
	if(pRect)
		*pRect=rc;
}
void CMainFrame::Notify(TNotifyUI& msg)
{
	if(msg.sType==_T("selectchanged"))
	{
		if(msg.pSender==m_pOptTabBuddyList)
			m_pListTable->SelectItem(0);
		else if(msg.pSender==m_pOptTabRecentList)
			m_pListTable->SelectItem(1);

	}
	else if(msg.sType==_T("windowinit"))
	{

	}
	else if(msg.sType==_T("click"))
	{
		if(msg.pSender==m_pBtnHeader)
		{

		}
		else if(msg.pSender==m_pBtnSearch)
		{
			GetContext()->GetSearchFrame().ShowWindow();
			::SetForegroundWindow(GetContext()->GetSearchFrame());
		}
	}
	CGJContextWnd::Notify(msg);
}


void CMainFrame::StartAnimateDock()
{ 
	StopAnimateDock();
	::SetTimer(*this,m_uDockTimer,DOCK_MOVE_INTERVAL,NULL);
}
void CMainFrame::StopAnimateDock()
{
	::KillTimer(*this,m_uDockTimer);
}
void CMainFrame::DoAnimateDock()
{
	if(m_bMoving||m_Dock==DOCK_NONE)
	{
		StopAnimateDock();
		return;
	}
	RECT rc,rcWork;
	int dx=0,dy=0;
	POINT mp; 
	bool mousein;
	::GetCursorPos(&mp);
	GetWorkArea(&rcWork);
	::GetWindowRect(*this,&rc);
	mousein=mp.x>=rc.left&&mp.x<=rc.right&&mp.y>=rc.top&&mp.y<=rc.bottom;
	if(mousein)
	{
		switch (m_Dock)
		{ 
		case DOCK_TOP:
			dy=rcWork.top-rc.top;
			FIX_DOCK_P(dy);
			break;
		case DOCK_LEFT:
			dx=rcWork.left-rc.left;
			FIX_DOCK_P(dx);
			break;
		case DOCK_RIGHT:
			dx=rcWork.right-rc.right;
			FIX_DOCK_N(dy);
			break; 
		}
	}
	else
	{
		switch (m_Dock)
		{ 
		case DOCK_TOP:
			dy=rcWork.top-rc.bottom+DOCK_HANDLE_WIDTH;
			FIX_DOCK_N(dy);
			break;
		case DOCK_LEFT:
			dx=rcWork.left-rc.right+DOCK_HANDLE_WIDTH;
			FIX_DOCK_N(dx);
			break;
		case DOCK_RIGHT:
			dx=rcWork.right-rc.left-DOCK_HANDLE_WIDTH;
			FIX_DOCK_P(dx);
			break; 
		}
	}
	if(dx||dy)
	{
		OffsetRect(&rc,dx,dy);
		::SetWindowPos(*this,NULL,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOSIZE|SWP_NOZORDER);
	}
	else
	{
		StopAnimateDock();
		return;
	}
}
LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg==WM_CLOSE)
	{
		PostQuitMessage(0);
	}
	else if(uMsg==WM_MOVING)
	{
		m_bMoving=true;
		RECT* rc=(RECT* )lParam;
		UpdateDock(rc);
		StopAnimateDock();
	}
	else if(uMsg==WM_EXITSIZEMOVE)
	{
		m_bMoving=false; 
	}
	else if(uMsg==WM_TIMER&&wParam==m_uDockTimer)
	{
		DoAnimateDock();
		return 0;
	}
	if(uMsg==WM_MOUSELEAVE||uMsg==WM_MOUSEHOVER
		||uMsg==WM_NCMOUSELEAVE||uMsg==WM_NCMOUSEMOVE)
	{
		StartAnimateDock();
	}
	return CGJWnd::HandleMessage(uMsg,wParam,lParam);
}

void CMainFrame::LoadUser()
{ 
	auto rosterMgr=GetContext()->GetClient()->rosterManager();  
	for(auto iter=rosterMgr->roster()->begin();iter!=rosterMgr->roster()->end();
		++iter)
	{
		handleItemAdded(iter->second->jid());
	}
	rosterMgr->fill();

}



void CMainFrame::AddContactGroup(LPCTSTR pstrGroupName)
{

}
bool CMainFrame::OnBuddyItemAction(LPVOID param)
{
	CBuddyItemEvent *event=(CBuddyItemEvent*)param;
	if(event->sType==_T("click"))
	{

	}
	else if(event->sType==_T("mouseenter"))
	{

	}
	else if(event->sType==_T("mouseleave"))
	{

	}
	else if(event->sType==_T("dbclick"))
	{
		if(m_pContext)
			m_pContext->m_SessionManager.OpenChatFrame(event->pSender->GetJid());
	}

	return false;
}

void CMainFrame::AddContactItem(CBuddyListItem& item)
{

}


CMainFrame::~CMainFrame(void)
{
	GetContext()->GetHeaderManager().RemoveHandler(this);
}



////
void CMainFrame::handleItemAdded( const JID& jid ) 
{
	auto roster=GetContext()->GetClient()->rosterManager()->getRosterItem(jid); 
	if(roster==NULL)
		return;
	CBuddyListItem *item=new CBuddyListItem;
	item->SetJid(jid);
	item->SetName(utf8dec(roster->name()));

	item->SetHeader(GetContext()->GetHeaderManager().GetHeader(jid,item->IsOnline()));

	m_pBuddyList->AddBuddy(utf8dec(roster->groups().front()),*item); 
	auto vcardMgr=GetContext()->GetVCardManager();
	if(vcardMgr)
		vcardMgr->fetchVCard(jid,this);
}

void CMainFrame::handleItemSubscribed( const JID& jid ) 
{

}


void CMainFrame::handleItemRemoved( const JID& jid ) 
{

}


void CMainFrame::handleItemUpdated( const JID& jid ) 
{

}


void CMainFrame::handleItemUnsubscribed( const JID& jid ) 
{
}


void CMainFrame::handleRoster( const Roster& roster ) 
{

}

void CMainFrame::handleRosterPresence( const RosterItem& item, const std::string& resource,
									  Presence::PresenceType presence, const std::string& msg ) 

{
	if(m_pBuddyList)
	{
		CBuddyListItem* buddy= m_pBuddyList->FindBuddyItem(item.jid());
		if(buddy)
		{
			bool online=buddy->IsOnline();
			buddy->SetPresence(presence);
			buddy->SetSignature(utf8dec(msg));
			if(online!=buddy->IsOnline())
			{
				buddy->SetHeader(GetContext()->GetHeaderManager().GetHeader(item.jid(),buddy->IsOnline()));
			}
		}
	}
}

void CMainFrame::handleSelfPresence( const RosterItem& item, const std::string& resource,
									Presence::PresenceType presence, const std::string& msg ) 
{

}


bool CMainFrame::handleSubscriptionRequest( const JID& jid, const std::string& msg ) 
{
	return false;
}


bool CMainFrame::handleUnsubscriptionRequest( const JID& jid, const std::string& msg ) 
{
	return false;
}


void CMainFrame::handleNonrosterPresence( const Presence& presence ) 
{
}


void CMainFrame::handleRosterError( const IQ& iq ) 
{
}

void CMainFrame::handleVCard( const JID& jid, const VCard* vcard )
{
	GetContext()->m_VCards[jid]=*vcard;
	if(!m_pBuddyList)
		return; 
	auto buddy=m_pBuddyList->FindBuddyItem(jid);
	if(!buddy)
		return; 
	buddy->SetName(utf8dec(vcard->nickname()));
	buddy->SetSignature(utf8dec(vcard->desc())); 

	GetContext()->GetHeaderManager().UpdateHeader(jid,vcard->photo().binval); 
}

void CMainFrame::handleVCardResult( VCardContext context, const JID& jid, StanzaError se /*= StanzaErrorUndefined */ )
{
	if(context==FetchVCard)
	{

	}
}

void CMainFrame::HandleHeaderUpdate( const CHeaderManager& manager,const JID& jid )
{
	if(!m_pBuddyList)
		return;
	auto buddy=m_pBuddyList->FindBuddyItem(jid);
	if(!buddy)
		return;
	buddy->SetHeader(manager.GetHeader(jid,buddy->IsOnline()));
}




////