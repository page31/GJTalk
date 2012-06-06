#pragma once
#include <forward_list>
#include "TrayIconMessage.h"
#include "ITrayIconListenner.h"
// CTrayIcon
using namespace std;

typedef forward_list<ITrayIconListener*> TrayIconListenerList;
class CTrayIcon : public CWnd
{
	DECLARE_DYNAMIC(CTrayIcon)
protected:
	TrayIconListenerList m_listeners;

public:



	CTrayIcon();



	void AddListener(  ITrayIconListener *const listener);
	void RemoveListener(  ITrayIconListener *const listener);


	bool Enabled() { return m_bEnabled; }
	bool Visible() { return !m_bHidden; } 

	bool    SetTooltipText(LPCTSTR pszTooltipText);
	bool    SetTooltipText(UINT nID);
	CString GetTooltipText() const;

	// Change or retrieve the icon displayed
	bool  SetIcon(HICON hIcon);
	bool  SetIcon(LPCTSTR lpszIconName);
	bool  SetIcon(UINT nIDResource);
	bool  SetStandardIcon(LPCTSTR lpIconName);
	bool  SetStandardIcon(UINT nIDResource);
	HICON GetIcon() const; 
	void  SetFocus();
	bool  HideIcon();
	bool  ShowIcon();
	bool  AddIcon();
	bool  RemoveIcon();
	bool  MoveToRight(); 
	bool ShowBalloon(LPCTSTR szText, LPCTSTR szTitle = NULL,
		DWORD dwIcon = NIIF_NONE, UINT uTimeout = 10); 
	// For icon animation
	bool  SetIconList(UINT uFirstIconID, UINT uLastIconID); 
	bool  SetIconList(HICON* pHIconList, UINT nNumIcons); 
	bool  Animate(UINT nDelayMilliSeconds, int nNumSeconds = -1);
	bool  StepAnimation();
	bool  StopAnimation();
	virtual ~CTrayIcon();
protected:
	afx_msg void OnTimer(UINT nIDEvent);
	void Notify(LPCTSTR lpszMessage,WPARAM wParam,LPARAM lParam);
	void Initialise();
	void InstallIconPending();

	virtual void CustomizeMenu(CMenu*) {}	// Used for customizing the menu
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);
protected:
	NOTIFYICONDATA  m_tnd;
	bool            m_bEnabled;         // does O/S support tray icon?
	bool            m_bHidden;          // Has the icon been hidden?
	bool            m_bRemoved;         // Has the icon been removed?
	bool            m_bShowIconPending; // Show the icon once tha taskbar has been created

	CArray<HICON, HICON> m_IconList;
	UINT_PTR     m_uIDTimer;
	INT_PTR      m_nCurrentIcon;
	COleDateTime m_StartTime;
	UINT         m_nAnimationPeriod;
	HICON        m_hSavedIcon;
	UINT         m_DefaultMenuItemID;
	bool         m_DefaultMenuItemByPos;
	UINT         m_uCreationFlags;

	static const UINT m_nTimerID;
	static UINT  m_nMaxTooltipLength;
	DECLARE_MESSAGE_MAP()
};


