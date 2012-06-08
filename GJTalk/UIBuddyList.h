#include "stdafx.h"
#include <vector>
using namespace std;
namespace DuiLib
{
	class CUIBuddyListItem
	{
		friend class CUIBuddyListGroup;
	};
	class CUIBuddyListGroup
	{
	private:
		CUIBuddyList *m_pList;
		CContainerUI *m_pContainer;
		LPCTSTR m_sGroupName;
	public:
		int Count() const;
		bool IsExpaned() const;
		void Expand();
		void Collapse();
		void Add(CUIBuddyListItem &item);
		void Remove(CUIBuddyListItem &item);
		bool ContainsItem(const CUIBuddyListItem &item) const;
		friend class CUIBuddyList;
	};
	class CUIBuddyList:public CListUI
	{
	private:
		vector<CUIBuddyListGroup> m_vGroups;
	public:
		void AddGroup(CUIBuddyListGroup &group);
		void RemoveGroup(CUIBuddyListGroup &group);
		void ContainsGroup(const CUIBuddyListGroup &group) const;
		CUIBuddyListGroup &GetGroup(int index);
		int Count() const;
	};
}