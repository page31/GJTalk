
#pragma once
#include "stdafx.h"
#include <vector>
#include "../xmpp/client.h"
using namespace std;
namespace DuiLib
{

	enum BuddyItemPos
	{
		BIP_BLANK,
		BIP_AVATAR,
		BIP_APP
	};


	class CBuddyListUI;
	class CBuddyListGroup;
	class CBuddyListItem
	{
	private:
		gloox::JID m_jid;
		CControlUI* m_pControl;
		CPaintManagerUI* m_pPaintManager;
		CBuddyListGroup *m_pGroup;
		CContainerUI* m_pContainer;
		CButtonUI* m_pBtnHeader;
		CLabelUI* m_pNameLabel;
		CLabelUI* m_pSignatureLabel;

		CStdString m_sName;
		CStdString m_sSignature;
		CStdString m_sHeaderPath;

	private:
		bool m_bSelected;
		void BindUI();
		void CancelSelect();


	public:
		CBuddyListItem();
		~CBuddyListItem();

		void Select(); 
		void MouseEnter();
		void MouseLeave();
		bool IsSelected();


		void SetName(LPCTSTR pstrName);
		void SetSignature(LPCTSTR pstrSignature);
		void SetHeader(LPCTSTR pstrFile);
		void SetJid(const gloox::JID& jid);
		const gloox::JID& GetJid() const;
		CStdString GetName()const;
		CStdString GetSignature() const;
		CStdString GetHeaderPath() const;


		BuddyItemPos GetMousePoint(int x,int y)const;
		BuddyItemPos GetMousePoint()const;

		friend class CBuddyListGroup;
		friend class CBuddyListUI;
	};
	class CBuddyListGroup
	{
	private:
		CPaintManagerUI* m_pPaintManager;
		CBuddyListUI *m_pList;
		CButtonUI* m_pGroupLabel;
		CVerticalLayoutUI *m_pContainer;
		CStdString m_sName;;

		vector<CBuddyListItem*> m_vItems; 
		bool m_bIsExpanded;
	private:
		void SetList(CBuddyListUI* list);
	public:
		CControlUI* m_pUI;
		~CBuddyListGroup();
		CBuddyListGroup( CPaintManagerUI* pPaintManager);
		void CaclHeight();
		CStdString GetName() const;
		void SetName(LPCTSTR pstrName);
		int Count() const;
		bool IsExpaned() const;
		void Expand();
		void Collapse();
		void Add(CBuddyListItem &item);
		void Remove(CBuddyListItem &item);
		bool ContainsItem(const CBuddyListItem &item) const;

		void BindUI();

		CBuddyListItem* FindBuddyItem(const gloox::JID& jid) const;

		friend class CBuddyListUI;
		friend class CBuddyListItem;
	};


#define CBuddyListGroups  

	typedef struct  
	{ 
		CBuddyListItem* pSender;
		LPCTSTR sType;
		BuddyItemPos pos;
		WPARAM wParam;
		LPARAM lParam;
	} CBuddyItemEvent;

	class CBuddyListUI:public CListUI
	{
	private:
		vector<CBuddyListGroup*> m_vGroups;
		CPaintManagerUI* m_pPaintManager;
		CDelegate<CBuddyListUI,CBuddyListUI> *m_pChildDelegate;
	public: 
		CEventSource OnItemAction;

	public:
		~CBuddyListUI();
		CBuddyListUI( CPaintManagerUI* pPaintManager); 
		void AddGroup(CBuddyListGroup &group);
		CBuddyListGroup& AddGroup(LPCTSTR pstrGroupName);
		CBuddyListGroup *GetGroup(LPCTSTR pstrGroupName);

		void RemoveGroup(CBuddyListGroup &group);
		void AddBuddy(LPCTSTR pstrGroup,CBuddyListItem& item); 
		bool ContainsGroup(const CBuddyListGroup &group) const;
		bool ContainsGroup(LPCTSTR pstrGroupName) const;
		CBuddyListGroup &GetGroup(int index);
		int Count() const;

		CBuddyListItem* FindBuddyItem(const gloox::JID& jid) const;

		bool OnGroupNotify(void* msg);
		friend class CBuddyListGroup;
		friend class CBuddyListItem;
	};
}