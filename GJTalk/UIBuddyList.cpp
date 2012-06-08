#include "stdafx.h"


#pragma region CBuddyListGroupUI

CBuddyListGroupUI::~CBuddyListGroupUI()
{
}
CBuddyListGroupUI::CBuddyListGroupUI(CPaintManagerUI* pPaintManager)
	:m_pPaintManager(pPaintManager)
{
	CListContainerElementUI* pSubUI=NULL;
	CDialogBuilder builder;
	pSubUI=static_cast<CListContainerElementUI*>(builder.Create(_T("buddy_list_group.xml"),0,NULL,m_pPaintManager));
	CListContainerElementUI::Add(pSubUI);

}
int CBuddyListGroupUI::Count() const
{
	return 0;
}
bool CBuddyListGroupUI::IsExpaned() const
{
	return false;
}
void CBuddyListGroupUI::Expand()
{ 
}
void CBuddyListGroupUI::Collapse()
{
}
void CBuddyListGroupUI::Add(CBuddyListItem &item)
{
}
void CBuddyListGroupUI::Remove(CBuddyListItem &item)
{
}
bool CBuddyListGroupUI::ContainsItem(const CBuddyListItem &item) const
{
	return false;
}  
#pragma endregion

#pragma region CBuddyListUI

CBuddyListUI::~CBuddyListUI()
{
	m_vGroups.clear();
}
CBuddyListUI::CBuddyListUI( CPaintManagerUI* pPaintManager)
	:m_pPaintManager(pPaintManager)
{
}
CBuddyListGroupUI& CBuddyListUI::AddGroup(LPCTSTR pstrGroupName)
{
	ASSERT(!ContainsGroup(pstrGroupName));
	CBuddyListGroupUI *pGroup=new CBuddyListGroupUI(m_pManager);
	pGroup->SetText(pstrGroupName);
	AddGroup(*pGroup);
	return *pGroup;
}
void CBuddyListUI::AddGroup(CBuddyListGroupUI &group)
{
	ASSERT(!ContainsGroup(group));
	m_vGroups.push_back(group);
	CListUI::Add(&group);
}
void CBuddyListUI::RemoveGroup(CBuddyListGroupUI &group)
{

}
bool CBuddyListUI::ContainsGroup(const CBuddyListGroupUI &group) const
{

	return ContainsGroup(group.GetText());
}
bool CBuddyListUI::ContainsGroup(LPCTSTR pstrGroupName) const
{
	for(auto iter=m_vGroups.begin();iter!=m_vGroups.end();++iter)
	{
		if(pstrGroupName==iter->GetText())
		{
			return true;
		}
	}
	return false;
}
CBuddyListGroupUI &CBuddyListUI::GetGroup(int index)
{
	return m_vGroups[index];
}
int CBuddyListUI::Count() const
{
	return m_vGroups.size();
}  
#pragma endregion
