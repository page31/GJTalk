// TrayIcon.cpp : implementation file
//

#include "stdafx.h"
#include "GJTalk.h"
#include "TrayIcon.h" 

// CTrayIcon
const UINT CTrayIcon::m_nTimerID    = 4567;
UINT CTrayIcon::m_nMaxTooltipLength  = 64;

IMPLEMENT_DYNAMIC(CTrayIcon, CWnd)

	CTrayIcon::CTrayIcon()
{
	this->Initialise(); 
	CWnd::CreateEx(0, AfxRegisterWndClass(0),_T(""), WS_POPUP, 0,0,0,0, NULL, 0);
	m_tnd.cbSize = sizeof(NOTIFYICONDATA);
	m_tnd.hWnd=this->GetSafeHwnd();

	int cx=GetSystemMetrics(SM_CXSMICON);
	int cy=GetSystemMetrics(SM_CYSMICON);
	 
	m_tnd.hIcon=(HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDR_MAINFRAME),IMAGE_ICON,
		cx,cy,LR_SHARED);



	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP|NIF_INFO;
	m_tnd.szInfoTitle[0] = _T('\0');
	m_tnd.uTimeout    = 3 * 1000; // convert time to ms
	m_tnd.dwInfoFlags = 0;
	m_tnd.uCallbackMessage=WM_USER+0x12;
}

CTrayIcon::~CTrayIcon()
{
	RemoveIcon();
	m_IconList.RemoveAll();
	DestroyWindow();
}
void CTrayIcon::Initialise()
{
	memset(&m_tnd, 0, sizeof(m_tnd));

	m_bEnabled = true;
	m_bHidden  = true;
	m_bRemoved = true;

	m_DefaultMenuItemID    = 0;
	m_DefaultMenuItemByPos = true;

	m_bShowIconPending = FALSE;

	m_uIDTimer   = 0;
	m_hSavedIcon = NULL;

	m_uCreationFlags = 0;
}
void CTrayIcon::AddListener( ITrayIconListener *const pListener)
{
	if(!pListener)
		return;
	RemoveListener(pListener);
	m_listeners.push_front(pListener);
}
void CTrayIcon::RemoveListener( ITrayIconListener *const pListener)
{
	if(!pListener)
		return;

	m_listeners.remove(pListener); 
}
void CTrayIcon::Notify(LPCTSTR lpszMessage,WPARAM wParam,LPARAM lParam)
{
	CTrayIconMessage msg;
	msg.sMsg=lpszMessage;
	msg.wParam=wParam;
	msg.lParam=lParam;
	msg.pSender=this; 
	for(TrayIconListenerList::iterator iter=m_listeners.begin();iter!=m_listeners.end();++iter)
	{
		(*iter)->OnTrayIconMessage(msg);
		if(msg.bHandled)
			break;
	}  
}

bool CTrayIcon::MoveToRight()
{
	RemoveIcon();
	return AddIcon();
}
bool CTrayIcon::RemoveIcon()
{
	m_bShowIconPending = true;

	if (!m_bEnabled || m_bRemoved)
		return true;

	m_tnd.uFlags = 0;
	if (Shell_NotifyIcon(NIM_DELETE, &m_tnd))
		m_bRemoved = m_bHidden = true;

	return (m_bRemoved == true);
}

bool CTrayIcon::AddIcon()
{
	if (!m_bRemoved)
		RemoveIcon();

	if (m_bEnabled)
	{
		m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		if (!Shell_NotifyIcon(NIM_ADD, &m_tnd))
			m_bShowIconPending = TRUE;
		else
			m_bRemoved = m_bHidden = FALSE;
	}
	return (m_bRemoved == FALSE);
}

bool CTrayIcon::HideIcon()
{
	if (!m_bEnabled || m_bRemoved || m_bHidden)
		return true; 

	m_tnd.uFlags = NIF_STATE;
	m_tnd.dwState = NIS_HIDDEN;
	m_tnd.dwStateMask = NIS_HIDDEN;

	m_bHidden = Shell_NotifyIcon( NIM_MODIFY, &m_tnd)?true:false;

	return (m_bHidden == true);
}

bool CTrayIcon::ShowIcon()
{
	if (m_bRemoved)
		return AddIcon();

	if (!m_bHidden)
		return true;

	m_tnd.uFlags = NIF_STATE;
	m_tnd.dwState = 0;
	m_tnd.dwStateMask = NIS_HIDDEN;
	Shell_NotifyIcon ( NIM_MODIFY, &m_tnd );
	m_bHidden = FALSE; 
	return (m_bHidden == true);
}
void CTrayIcon::SetFocus()
{ 
	Shell_NotifyIcon(NIM_SETFOCUS, &m_tnd ); 
}
bool CTrayIcon::SetIcon(HICON hIcon)
{
	if (!m_bEnabled)
		return false;

	m_tnd.uFlags = NIF_ICON;
	m_tnd.hIcon = hIcon;

	if (m_bHidden)
		return true;
	else
		return Shell_NotifyIcon(NIM_MODIFY, &m_tnd)?true:false;
}
bool CTrayIcon::SetIcon(LPCTSTR lpszIconName)
{
	HICON hIcon = (HICON) ::LoadImage(AfxGetResourceHandle(), 
		lpszIconName,
		IMAGE_ICON, 
		0, 0,
		LR_DEFAULTCOLOR);
	if (!hIcon)
		return FALSE;

	bool bSuccess = SetIcon(hIcon);
	::DestroyIcon(hIcon); 
	return bSuccess;
}
bool CTrayIcon::SetIcon(UINT nIDResource)
{
	return SetIcon(MAKEINTRESOURCE(nIDResource));
}




bool CTrayIcon::SetStandardIcon(LPCTSTR lpIconName)
{
	HICON hIcon = LoadIcon(NULL, lpIconName);

	return SetIcon(hIcon);
}

bool CTrayIcon::SetStandardIcon(UINT nIDResource)
{
	return SetStandardIcon(MAKEINTRESOURCE(nIDResource));
}

HICON CTrayIcon::GetIcon() const
{
	return (m_bEnabled)? m_tnd.hIcon : NULL;
}

bool CTrayIcon::SetIconList(UINT uFirstIconID, UINT uLastIconID) 
{
	if (uFirstIconID > uLastIconID)
		return FALSE;

	const CWinApp* pApp = AfxGetApp();
	if (!pApp)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_IconList.RemoveAll();
	TRY {
		for (UINT i = uFirstIconID; i <= uLastIconID; i++)
			m_IconList.Add(pApp->LoadIcon(i));
	}
	CATCH(CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		m_IconList.RemoveAll();
		return FALSE;
	}
	END_CATCH

		return TRUE;
}

bool CTrayIcon::SetIconList(HICON* pHIconList, UINT nNumIcons)
{
	m_IconList.RemoveAll();

	TRY {
		for (UINT i = 0; i < nNumIcons; i++)
			m_IconList.Add(pHIconList[i]);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		m_IconList.RemoveAll();
		return FALSE;
	}
	END_CATCH

		return TRUE;
}
bool CTrayIcon::ShowBalloon(LPCTSTR szText,
							LPCTSTR szTitle  /*=NULL*/,
							DWORD   dwIcon   /*=NIIF_NONE*/,
							UINT    uTimeout /*=10*/ )
{


	// Verify input parameters.

	// The balloon tooltip text can be up to 255 chars long.
	ASSERT(AfxIsValidString(szText));
	ASSERT(lstrlen(szText) < 256);

	// The balloon title text can be up to 63 chars long.
	if (szTitle)
	{
		ASSERT(AfxIsValidString( szTitle));
		ASSERT(lstrlen(szTitle) < 64);
	}

	// dwBalloonIcon must be valid.
	ASSERT(NIIF_NONE == dwIcon    || NIIF_INFO == dwIcon ||
		NIIF_WARNING == dwIcon || NIIF_ERROR == dwIcon);

	// The timeout must be between 10 and 30 seconds.
	ASSERT(uTimeout >= 10 && uTimeout <= 30);


	m_tnd.uFlags = NIF_INFO;
	_tcsncpy_s(m_tnd.szInfo,sizeof(m_tnd.szInfo), szText, 256);
	if (szTitle)
		_tcsncpy_s(m_tnd.szInfoTitle,sizeof(m_tnd.szInfoTitle), szTitle, 64);
	else
		m_tnd.szInfoTitle[0] = _T('\0');
	m_tnd.dwInfoFlags = dwIcon;
	m_tnd.uTimeout = uTimeout * 1000;   // convert time to ms

	BOOL bSuccess = Shell_NotifyIcon (NIM_MODIFY, &m_tnd);

	// Zero out the balloon text string so that later operations won't redisplay
	// the balloon.
	m_tnd.szInfo[0] = _T('\0');

	return bSuccess==TRUE; 
}
bool CTrayIcon::Animate(UINT nDelayMilliSeconds, int nNumSeconds /*=-1*/)
{
	if (m_IconList.IsEmpty())
		return FALSE;

	StopAnimation();

	m_nCurrentIcon = 0;
	m_StartTime = COleDateTime::GetCurrentTime();
	m_nAnimationPeriod = nNumSeconds;
	m_hSavedIcon = GetIcon();

	// Setup a timer for the animation
	m_uIDTimer = SetTimer(m_nTimerID, nDelayMilliSeconds, NULL);

	return (m_uIDTimer != 0);
}

bool CTrayIcon::StepAnimation()
{
	if (!m_IconList.GetSize())
		return FALSE;

	m_nCurrentIcon++;
	if (m_nCurrentIcon >= m_IconList.GetSize())
		m_nCurrentIcon = 0;

	return SetIcon(m_IconList[m_nCurrentIcon]);
}

bool CTrayIcon::StopAnimation()
{
	bool bResult = FALSE;

	if (m_uIDTimer)
		bResult = KillTimer(m_uIDTimer)?true:false;
	m_uIDTimer = 0;

	if (m_hSavedIcon)
		SetIcon(m_hSavedIcon);
	m_hSavedIcon = NULL;

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// CSystemTray tooltip text manipulation

bool CTrayIcon::SetTooltipText(LPCTSTR pszTip)
{
	ASSERT(AfxIsValidString(pszTip)); // (md)
	ASSERT(_tcslen(pszTip) < m_nMaxTooltipLength);

	if (!m_bEnabled) 
		return FALSE;

	m_tnd.uFlags = NIF_TIP; 
	_tcsncpy_s(m_tnd.szTip,sizeof( m_tnd.szTip), pszTip, m_nMaxTooltipLength-1);

	if (m_bHidden)
		return TRUE;
	else
		return Shell_NotifyIcon(NIM_MODIFY, &m_tnd)?true:false;
}

bool CTrayIcon::SetTooltipText(UINT nID)
{
	CString strText;
	VERIFY(strText.LoadString(nID));

	return SetTooltipText(strText);
}

CString CTrayIcon::GetTooltipText() const
{
	CString strText;
	if (m_bEnabled)
		strText = m_tnd.szTip;

	return strText;
}



LRESULT CTrayIcon::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == m_tnd.uCallbackMessage)
		return OnTrayNotification(wParam, lParam);

	return CWnd::WindowProc(message, wParam, lParam);
}


LRESULT CTrayIcon::OnTrayNotification(UINT wParam, LONG lParam) 
{
	if (wParam != m_tnd.uID)
		return 0L; 
	WORD msg=LOWORD(lParam);
	switch (msg)
	{
	case WM_LBUTTONUP:
		this->Notify(_T("lup"),wParam,lParam);
		break;
	case WM_LBUTTONDOWN:
		this->Notify(_T("ldown"),wParam,lParam);
		break;
	case WM_LBUTTONDBLCLK:
		this->Notify(_T("ldbclick"),wParam,lParam);
		break;
	case WM_RBUTTONUP:
		this->Notify(_T("rup"),wParam,lParam);
		break;
	case WM_RBUTTONDOWN:
		this->Notify(_T("rdown"),wParam,lParam);
	case WM_RBUTTONDBLCLK: 
		this->Notify(_T("rdbclick"),wParam,lParam);
	default:
		break;
	}
	return 1;
}
void CTrayIcon::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent != m_uIDTimer) 
		return; 
	COleDateTime CurrentTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan period = CurrentTime - m_StartTime;

	if (m_nAnimationPeriod > 0 && m_nAnimationPeriod < period.GetTotalSeconds())
	{
		StopAnimation();
		return;
	} 
	StepAnimation();
}
////
BEGIN_MESSAGE_MAP(CTrayIcon, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CTrayIcon message handlers


