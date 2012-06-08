#pragma once
#include <string>
#include "GJContextWnd.h"
#include "TrayIcon.h"


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
class CMainFrame:public CGJContextWnd,public ITrayIconListener
{

private: 
	DOCK_STYLE m_Dock;
	static const UINT m_uDockTimer=0x123;
	static const UINT m_uDockOutCheckTimer=0x124;
	bool m_bMoving;

protected:
	void OnPostCreate();
public:
	void DoAnimateDock();
	void StartAnimateDock();
	void StopAnimateDock();
	void Notify(TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnTrayIconMessage(CTrayIconMessage &msg);
	void LoadUser();
	void UpdateDock(LPRECT pRect=NULL);
	CMainFrame(GJContext *context);
	~CMainFrame(void);
};

