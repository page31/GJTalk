#include "stdafx.h"
#include <vector>
using namespace std;
namespace DuiLib
{


	class CBuddyListUI;
	class CBuddyListItem
	{
		friend class CBuddyListGroupUI;
	};
	class CBuddyListGroupUI:public CListContainerElementUI
	{
	private:
		CPaintManagerUI* m_pPaintManager;
		CBuddyListUI *m_pList;
		CLabelUI* m_pGroupLabel;
		CContainerUI *m_pContainer;
		LPCTSTR m_sGroupName;
	public:
		~CBuddyListGroupUI();
		CBuddyListGroupUI( CPaintManagerUI* pPaintManager);
		int Count() const;
		bool IsExpaned() const;
		void Expand();
		void Collapse();
		void Add(CBuddyListItem &item);
		void Remove(CBuddyListItem &item);
		bool ContainsItem(const CBuddyListItem &item) const;
		friend class CBuddyListUI;
	};


#define CBuddyListGroupUIs 

	class CBuddyListUI:public CListUI
	{
	private:
		vector<CBuddyListGroupUI> m_vGroups;
		CPaintManagerUI* m_pPaintManager;
	public:
		~CBuddyListUI();
		CBuddyListUI( CPaintManagerUI* pPaintManager);
		void AddGroup(CBuddyListGroupUI &group);
		CBuddyListGroupUI& AddGroup(LPCTSTR pstrGroupName);
		void RemoveGroup(CBuddyListGroupUI &group);
		bool ContainsGroup(const CBuddyListGroupUI &group) const;
		bool ContainsGroup(LPCTSTR pstrGroupName) const;
		CBuddyListGroupUI &GetGroup(int index);
		int Count() const;
	};
}