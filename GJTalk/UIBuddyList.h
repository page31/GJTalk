#include "stdafx.h"
#include <vector>
using namespace std;
namespace DuiLib
{


	class CBuddyListUI;
	class CBuddyListGroup;
	class CBuddyListItem
	{
	private:
		CControlUI* m_pControl;
		CPaintManagerUI* m_pPaintManager;
		CBuddyListGroup *m_pGroup;
		CContainerUI* m_pContainer;
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

		friend class CBuddyListUI;
		friend class CBuddyListItem;
	};


#define CBuddyListGroups 

	class CBuddyListUI:public CListUI
	{
	private:
		vector<CBuddyListGroup*> m_vGroups;
		CPaintManagerUI* m_pPaintManager;
		CDelegate<CBuddyListUI,CBuddyListUI> *m_pChildDelegate;
	public:
		~CBuddyListUI();
		CBuddyListUI( CPaintManagerUI* pPaintManager); 
		void AddGroup(CBuddyListGroup &group);
		CBuddyListGroup& AddGroup(LPCTSTR pstrGroupName);
		void RemoveGroup(CBuddyListGroup &group);
		bool ContainsGroup(const CBuddyListGroup &group) const;
		bool ContainsGroup(LPCTSTR pstrGroupName) const;
		CBuddyListGroup &GetGroup(int index);
		int Count() const;

		bool OnGroupNotify(void* msg);
		friend class CBuddyListGroup;
		friend class CBuddyListItem;
	};
}