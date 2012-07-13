#pragma once
#include <string>
#include "GJContextWnd.h"
#include "TrayIcon.h"
#include "UIBuddyList.h"

#include "../xmpp/rostermanager.h"

#define DOCK_MOVE_STEP (50)
#define DOCK_MOVE_INTERVAL (10)
#define DOCK_HANDLE_WIDTH (2)
using namespace std;
using namespace gloox;

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

class CMainFrame:public CGJContextWnd,public ITrayIconListener,public IDialogBuilderCallback,
	public gloox::RosterListener
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

	bool OnBuddyItemAction(LPVOID param);



	void UpdateDock(LPRECT pRect=NULL);
	void DoAnimateDock();
	void StartAnimateDock();
	void StopAnimateDock();
	CMainFrame(GJContext *context);
	~CMainFrame(void);

	//// roster 

	virtual void handleItemAdded( const JID& jid ) ;

	virtual void handleItemSubscribed( const JID& jid ) ;


	virtual void handleItemRemoved( const JID& jid ) ;


	virtual void handleItemUpdated( const JID& jid ) ;


	virtual void handleItemUnsubscribed( const JID& jid ) ;


	virtual void handleRoster( const Roster& roster ) ;

	virtual void handleRosterPresence( const RosterItem& item, const std::string& resource,
		Presence::PresenceType presence, const std::string& msg ) ;

	virtual void handleSelfPresence( const RosterItem& item, const std::string& resource,
		Presence::PresenceType presence, const std::string& msg ) ;


	virtual bool handleSubscriptionRequest( const JID& jid, const std::string& msg ) ;


	virtual bool handleUnsubscriptionRequest( const JID& jid, const std::string& msg ) ;


	virtual void handleNonrosterPresence( const Presence& presence ) ;


	virtual void handleRosterError( const IQ& iq ) ;


	////


	friend class GJContext;
};

