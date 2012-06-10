#pragma once
#include <string>
#include "GJContextWnd.h"
#include "TrayIcon.h"
#include "UIBuddyList.h"

#define DOCK_MOVE_STEP (50)
#define DOCK_MOVE_INTERVAL (10)
#define DOCK_HANDLE_WIDTH (2)
using namespace std;


enum DOCK_STYLE
{
	DOCK_NONE,
	DOCK_TOP,
	DOCK_LEFT,
	DOCK_RIGHT
};


LPCTSTR const pstrBuddyListName=_T("listContact");
LPCTSTR const pstrRecentBuddyListName=_T("listRecent"); 
LPCTSTR const pstrOptTabBuddyListName=_T("optContactTab"); 
LPCTSTR const pstrOptTabRecentListName=_T("optRecentTab"); 
LPCTSTR const pstrListTableName=_T("tabContactList");  

class CMainFrame:public CGJContextWnd,public ITrayIconListener,public IDialogBuilderCallback
{

private: 
	DOCK_STYLE m_Dock;
	static const UINT m_uDockTimer=0x123;
	static const UINT m_uDockOutCheckTimer=0x124;
	bool m_bMoving;

	CBuddyListUI* m_pBuddyList;
	CBuddyListUI* m_pRecentList;

	COptionUI* m_pOptTabBuddyList;
	COptionUI* m_pOptTabRecentList;
	CTabLayoutUI* m_pListTable;
public:


protected:
	void OnPostCreate();
public:

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);


	void Notify(TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnTrayIconMessage(CTrayIconMessage &msg);
	void LoadUser();

	void AddContactGroup(LPCTSTR pstrGroupName);
	void AddContactItem(CBuddyListItem& item);

	void UpdateDock(LPRECT pRect=NULL);
	void DoAnimateDock();
	void StartAnimateDock();
	void StopAnimateDock();
	CMainFrame(GJContext *context);
	~CMainFrame(void);
};

