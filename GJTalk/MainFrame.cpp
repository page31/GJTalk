#include "stdafx.h"
#include "MainFrame.h"

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
	this->Create(NULL,m_pContext->GetAppName(),UI_WNDSTYLE_EX_FRAME,WS_EX_OVERLAPPEDWINDOW|WS_EX_TOPMOST);
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
	ASSERT(m_pOptTabBuddyList);
	ASSERT(m_pOptTabRecentList);
	ASSERT(m_pListTable);
	ASSERT(m_pBuddyList);
	ASSERT(m_pRecentList);
	m_pBuddyList->AddGroup(_T("test"));
	m_pBuddyList->AddGroup(_T("test2"));
	CBuddyListItem *item=new CBuddyListItem(); 
	m_pBuddyList->GetGroup(0).Add(*item);

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
	if(uMsg==WM_MOVING)
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

}



void CMainFrame::AddContactGroup(LPCTSTR pstrGroupName)
{

}
void CMainFrame::AddContactItem(CBuddyListItem& item)
{

}


CMainFrame::~CMainFrame(void)
{
}
