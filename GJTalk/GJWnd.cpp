#include "stdafx.h"
#include "GJWnd.h"

LPCTSTR CGJWnd::GetWindowClassName() const
{
	return _T("GJFrameHostWnd");
}
UINT CGJWnd::GetClassStyle() const
{
	return CS_DBLCLKS|CS_SAVEBITS;
}
LRESULT CGJWnd::OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,bool &bHandled)
{
	//long styleValue=::GetWindowLong(m_hWnd,GWL_STYLE);
	//styleValue&=~(WS_CAPTION|WS_OVERLAPPED);
	//::SetWindowLong(m_hWnd,GWL_STYLE,styleValue|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_POPUP|WS_MINIMIZEBOX); 
	m_pm.Init(m_hWnd); 
	HICON icon=::LoadIcon(m_pm.GetInstance(),MAKEINTRESOURCE(IDR_MAINFRAME));
	if(icon)
		SendMessage(WM_SETICON,ICON_SMALL,(LPARAM) icon);
	this->OnPostCreate(); 
	m_pm.AddNotifier(this);
	return 0;
}


LRESULT CGJWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if(wParam==SC_CLOSE)
	{
		SendMessage(WM_CLOSE,0L,0L);
		bHandled=true;
		return 0;
	}
	BOOL bZommed=::IsZoomed(*this);
	LRESULT lRes=CWindowWnd::HandleMessage(uMsg,wParam,lParam);
	if(::IsZoomed(*this)!=bZommed)
	{
		if(!bZommed)
		{
			//hide max
			if(m_pBtnMax)
				m_pBtnMax->SetVisible(false);
			if(m_pBtnResotre)
				m_pBtnResotre->SetVisible(true);
		}else
		{ 
			//hide restore
			if(m_pBtnMax)
				m_pBtnMax->SetVisible(true);
			if(m_pBtnResotre)
				m_pBtnResotre->SetVisible(false);
		}
	}
	return lRes;
}
LRESULT CGJWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	MONITORINFO oMonitor={};
	oMonitor.cbSize=sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this,
		MONITOR_DEFAULTTOPRIMARY),&oMonitor);
	::CRect rcWork=oMonitor.rcWork;
	LPMINMAXINFO lpMMI=(LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x=rcWork.left;
	lpMMI->ptMaxPosition.y=rcWork.top;
	lpMMI->ptMaxSize.x=rcWork.right-rcWork.left;
	lpMMI->ptMaxSize.y=rcWork.bottom-rcWork.top;
	bHandled=false;
	return 0;
}
LRESULT CGJWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	SIZE szRoundCorner=m_pm.GetRoundCorner();
	if(!::IsIconic(*this)&&(szRoundCorner.cx!=0||szRoundCorner.cy!=0))
	{
		::RECT rcClient;
		::GetClientRect(*this,&rcClient);
		HRGN hRgn=::CreateRoundRectRgn(rcClient.left,rcClient.top,rcClient.right+1,rcClient.bottom+1
			,szRoundCorner.cx,szRoundCorner.cy);
		::SetWindowRgn(*this,hRgn,TRUE);
		::DeleteObject(hRgn);
	}
	bHandled=false;
	return 0;
}
LRESULT CGJWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if( !::IsZoomed(*this) ) {
		RECT rcSizeBox = m_pm.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left 
		&& pt.x < rcClient.right - rcCaption.right 
		&& pt.y >= rcCaption.top 
		&& pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
				return HTCAPTION;
	} 
	return HTCLIENT;
}
LRESULT CGJWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return 0;
}
LRESULT CGJWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return 0;
}
LRESULT CGJWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return wParam==0?TRUE:FALSE;
}
LRESULT CGJWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	//::PostQuitMessage(0L);
	LRESULT lRes;
	bHandled=TRUE; 
	if(!m_pm.MessageHandler(uMsg, wParam, lParam, lRes) )   
		lRes=CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	TNotifyUI msg;
	msg.sType=_T("destory");
	msg.wParam=wParam;
	msg.lParam=lParam;
	msg.dwTimestamp=::GetTickCount();
	msg.pSender=NULL;
	this->Notify(msg);
	return lRes;
}

LRESULT CGJWnd::OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{ 
	LRESULT lRes;
	bHandled=TRUE; 
	if(!m_pm.MessageHandler(uMsg, wParam, lParam, lRes) )   
		lRes=CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	TNotifyUI msg;
	msg.sType=_T("ncdestory");
	msg.wParam=wParam;
	msg.lParam=lParam;
	msg.dwTimestamp=::GetTickCount();
	msg.pSender=NULL;
	this->Notify(msg);
	return lRes;
}

LRESULT CGJWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	bHandled=FALSE;
	TNotifyUI msg;
	msg.sType=_T("close");
	msg.wParam=wParam;
	msg.lParam=lParam;
	msg.dwTimestamp=::GetTickCount();
	msg.pSender=NULL;
	this->Notify(msg);
	return 0;
}

void  CGJWnd::OnFinalMessage(HWND hWnd)
{
	if(hWnd!=m_hWnd)
		return;
	TNotifyUI msg;
	msg.sType=_T("final"); 
	msg.dwTimestamp=::GetTickCount();
	msg.pSender=NULL;
	this->Notify(msg);
}

LRESULT CGJWnd::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{

	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	::ScreenToClient(m_pm.GetPaintWindow(), &pt);
	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("ie")));
	if( pControl && pControl->IsVisible() ) {
		RECT rc = pControl->GetPos();
		if( ::PtInRect(&rc, pt) ) {
			return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		}
	}

	bHandled = FALSE;
	return 0;
}
LRESULT CGJWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	LRESULT lRes = 0;
	bool bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:        
		lRes = OnCreate(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_CLOSE:         
		lRes = OnClose(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_NCDESTROY:
		lRes=OnNcDestroy(uMsg,wParam,lParam,bHandled);
		break;
	case WM_DESTROY:       
		lRes = OnDestroy(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCACTIVATE:    
		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_NCCALCSIZE:    
		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCPAINT:       
		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCHITTEST:     
		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_SIZE:         
		lRes = OnSize(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_GETMINMAXINFO: 
		lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_SYSCOMMAND:   
		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_MOUSEWHEEL:    
		lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); 
		break; 

	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);

}

bool CGJWnd::InitFromXmlFile(LPCTSTR lpszFilename,IDialogBuilderCallback* callback)
{
	CDialogBuilder builder; 
	CControlUI* pRoot=builder.Create(lpszFilename,
		0,callback,&m_pm);
	if(!pRoot)
		return false;

	m_pm.AttachDialog(pRoot); 
	return true;
}

void CGJWnd::OnPrepare()
{
	m_pBtnClose=m_pm.FindControl(_T("wnd_closebtn"));
	m_pBtnMax=m_pm.FindControl(_T("wnd_maxbtn"));
	m_pBtnMin=m_pm.FindControl(_T("wnd_minbtn"));
	m_pBtnResotre=m_pm.FindControl(_T("wnd_restorebtn"));
	m_pCaption=m_pm.FindControl(_T("wnd_caption"));
	if(m_pCaption)
		m_pCaption->SetText(m_sCaption);
	if(m_pBtnResotre)
		m_pBtnResotre->SetVisible(false);
	if(::IsWindow(m_hWnd))
		::SetWindowText(m_hWnd,m_sCaption);
}

void CGJWnd::GetWorkArea(LPRECT pRect)
{
	MONITORINFO oMonitor={};
	oMonitor.cbSize=sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this,
		MONITOR_DEFAULTTOPRIMARY),&oMonitor);
	*pRect=oMonitor.rcWork;
}

void CGJWnd::Notify(TNotifyUI& msg)
{
	if(msg.sType==_T("windowinit"))
	{
		OnPrepare();
	}
	else if(msg.sType==_T("click"))
	{ 
		if(msg.pSender==m_pBtnClose)
		{
			this->Close();
			//SendMessage(WM_SYSCOMMAND,SC_CLOSE,0L);
			//PostQuitMessage(0L); 
		}
		else if(msg.pSender==m_pBtnMax)
		{
			SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0L); 
		}
		else if(msg.pSender==m_pBtnMin)
		{
			LONG exStyle=::GetWindowLong(this->m_hWnd,GWL_EXSTYLE);
			if(exStyle&WS_EX_NOACTIVATE)
				::ShowWindow(this->m_hWnd,SW_HIDE);
			else 
				SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,0L); 
		}
		else if(msg.pSender==m_pBtnResotre)
		{
			SendMessage(WM_SYSCOMMAND,SC_RESTORE,0L); 
		}
	}
}
void CGJWnd::SetCaptionText(LPCTSTR pstrText)
{
	m_sCaption=pstrText;
	if(m_pCaption)
		m_pCaption->SetText(pstrText);
	if(::IsWindow(m_hWnd))
		SetWindowText(m_hWnd,pstrText);
}
CStdString CGJWnd::GetCaptionText() const
{
	return m_sCaption;
}
CGJWnd::CGJWnd(void) 
	:m_pBtnClose(NULL),
	m_pBtnMax(NULL),m_pBtnResotre(NULL),
	m_pBtnMin(NULL),m_pCaption(NULL)
{
}

CGJWnd::~CGJWnd(void)
{

}

 
