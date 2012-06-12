#include "stdafx.h"



#pragma region CBuddyListItem


bool CBuddyListItem::IsSelected()
{
	return m_bSelected;
}

void CBuddyListItem::MouseEnter()
{
	if(!IsSelected())
		this->m_pContainer->SetBkImage(_T("buddy_hover.png"));
}
void CBuddyListItem::MouseLeave()
{
	if(!IsSelected())
		this->m_pContainer->SetBkImage(_T(""));
}
void CBuddyListItem::CancelSelect()
{
	m_bSelected=false;
	if(m_pContainer->IsMouseOver())
		MouseEnter();
	else
		MouseLeave();
}
void CBuddyListItem::Select()
{
	if(m_pGroup&&m_pGroup->m_pList)
	{
		for(auto iter1=m_pGroup->m_pList->m_vGroups.begin();
			iter1!=m_pGroup->m_pList->m_vGroups.end();
			++iter1)
		{
			for(auto iter=(*iter1)->m_vItems.begin();iter!=(*iter1)->m_vItems.end();++iter)
			{
				if(*iter!=this)
				{
					(*iter)->CancelSelect();
				}
			}
		}
	} 
	m_bSelected=true;
	this->m_pContainer->SetBkImage(_T("buddy_focus.png"));
}


void CBuddyListItem::BindUI()
{
	if(!m_pControl)
		return;
	m_pContainer=static_cast<CContainerUI*>(m_pPaintManager->FindSubControlByName(m_pControl,_T("item_container")));
	ASSERT(m_pContainer);
	if(m_pGroup&&m_pGroup->m_pList)
	{
		m_pContainer->OnNotify.Clear();
		m_pContainer->OnNotify+=*m_pGroup->m_pList->m_pChildDelegate;
	}
}

CBuddyListItem::CBuddyListItem():
	m_pControl(NULL),m_pPaintManager(NULL),m_pGroup(NULL),m_pContainer(NULL),m_bSelected(false)
{

}
CBuddyListItem::~CBuddyListItem()
{
}

#pragma endregion


#pragma region CBuddyListGroup

CBuddyListGroup::~CBuddyListGroup()
{
}
void CBuddyListGroup::CaclHeight()
{
	int height=0;
	for (int i = 0; i < m_pContainer->GetCount(); i++)
	{
		height+=m_pContainer->GetItemAt(i)->GetFixedHeight();
	}
	m_pContainer->SetFixedHeight(height);
	if(IsExpaned())
		Expand();
}
CBuddyListGroup::CBuddyListGroup(CPaintManagerUI* pPaintManager)
	:m_pPaintManager(pPaintManager),m_sName(_T("Noname")),m_bIsExpanded(false),m_pList(NULL)
{
	CListContainerElementUI* pSubUI=NULL;
	CDialogBuilder builder;
	pSubUI=static_cast<CListContainerElementUI*>(builder.Create(_T("buddy_list_group.xml"),0,NULL,m_pPaintManager));
	ASSERT(pSubUI);
	m_pGroupLabel=static_cast<CButtonUI*>( pSubUI->FindSubControl(_T("GroupLabel")));
	m_pContainer=static_cast<CVerticalLayoutUI *>(pSubUI->FindSubControl(_T("GroupChildren")));
	ASSERT(m_pGroupLabel);
	ASSERT(m_pContainer);
	m_pGroupLabel->SetText(m_sName);
	m_pUI=pSubUI; 
	Collapse();
}
int CBuddyListGroup::Count() const
{
	return 0;
}
bool CBuddyListGroup::IsExpaned() const
{
	return m_bIsExpanded;
}
void CBuddyListGroup::Expand()
{ 
	m_bIsExpanded=true;
	m_pContainer->SetVisible();
	m_pUI->SetFixedHeight(m_pGroupLabel->GetFixedHeight()+m_pContainer->GetFixedHeight());
	SetName(m_sName);
}
void CBuddyListGroup::Collapse()
{
	m_bIsExpanded=false;
	m_pContainer->SetVisible(false);
	m_pUI->SetFixedHeight(m_pGroupLabel->GetFixedHeight());
	SetName(m_sName);
}

void CBuddyListGroup::SetList(CBuddyListUI* list)
{
	this->m_pList=list;
	BindUI();
	//if(this->m_pList)
	//{
	//	for(auto iter=m_vItems.begin();iter!=m_vItems.end();++iter)
	//	{
	//		(*iter)->m_pControl->OnNotify.Clear();
	//		(*iter)->m_pControl->OnNotify+=*list->m_pChildDelegate;
	//	}
	//}
}

void CBuddyListGroup::Add(CBuddyListItem &item)
{ 
	if(ContainsItem(item))
		return;
	if(!item.m_pControl)
	{
		CDialogBuilder builder;
		item.m_pControl=static_cast<CListContainerElementUI*>(builder.Create(_T("buddy_list_item.xml"),0,NULL,m_pPaintManager));
		item.m_pGroup=this;
		item.m_pPaintManager=m_pPaintManager;
		ASSERT(item.m_pControl);
		item.m_pControl->SetUserData(_T("GroupItem")); 
	}
	item.BindUI();
	this->m_pContainer->Add(item.m_pControl);
	m_vItems.push_back(&item);
	CaclHeight();
}
void CBuddyListGroup::Remove(CBuddyListItem &item)
{
	for(auto iter=m_vItems.begin();iter!=m_vItems.end();++iter)
	{
		if(*iter==&item)
		{
			m_vItems.erase(iter);
			CaclHeight();
			return;
		}
	}

}
bool CBuddyListGroup::ContainsItem(const CBuddyListItem &item) const
{
	for(auto iter=m_vItems.begin();iter!=m_vItems.end();++iter)
	{
		if(*iter==&item)
		{ 
			return true;
		}
	}
	return false;
}  
void CBuddyListGroup::SetName(LPCTSTR pstrName)
{
	m_sName=pstrName;
	CStdString strText;
	if(IsExpaned())
		strText=_T("<i list_icon_b.png>");
	else
		strText=_T("<i list_icon_a.png>");
	strText+=pstrName; 
	m_pGroupLabel->SetShowHtml();
	m_pGroupLabel->SetText(strText);
}
void CBuddyListGroup::BindUI()
{ 
	m_pGroupLabel->OnNotify+=*m_pList->m_pChildDelegate;
	for(auto iter=m_vItems.begin();iter!=m_vItems.end();++iter)
	{
		(*iter)->BindUI();
	}
}
CStdString CBuddyListGroup::GetName() const
{
	return m_sName;
}
#pragma endregion

#pragma region CBuddyListUI

CBuddyListUI::~CBuddyListUI()
{
	m_vGroups.clear();
	delete m_pChildDelegate;
}
CBuddyListUI::CBuddyListUI( CPaintManagerUI* pPaintManager)
	:m_pPaintManager(pPaintManager)
{
	m_pChildDelegate=new CDelegate<CBuddyListUI,CBuddyListUI>(this,&CBuddyListUI::OnGroupNotify);
	ASSERT(m_pChildDelegate);
}
CBuddyListGroup& CBuddyListUI::AddGroup(LPCTSTR pstrGroupName)
{
	ASSERT(!ContainsGroup(pstrGroupName));
	CBuddyListGroup *pGroup=new CBuddyListGroup(m_pManager); 
	pGroup->SetName(pstrGroupName);
	AddGroup(*pGroup);
	return *pGroup;
}
void CBuddyListUI::AddGroup(CBuddyListGroup &group)
{
	ASSERT(!ContainsGroup(group));
	m_vGroups.push_back(&group);
	group.m_pList=this;
	group.BindUI();
	CListUI::Add(group.m_pUI);
	this->Invalidate();
}
void CBuddyListUI::RemoveGroup(CBuddyListGroup &group)
{

}
bool CBuddyListUI::ContainsGroup(const CBuddyListGroup &group) const
{ 
	return ContainsGroup(group.GetName());
}
bool CBuddyListUI::ContainsGroup(LPCTSTR pstrGroupName) const
{
	for(auto iter=m_vGroups.begin();iter!=m_vGroups.end();++iter)
	{
		if( pstrGroupName==(*iter)->GetName())
		{
			return true;
		}
	}
	return false;
}
CBuddyListGroup &CBuddyListUI::GetGroup(int index)
{
	return *m_vGroups[index];
}
int CBuddyListUI::Count() const
{
	return m_vGroups.size();
}

bool CBuddyListUI::OnGroupNotify(void* pMsg)
{
	TNotifyUI& msg=*static_cast<TNotifyUI*>(pMsg);

	for(auto iter=m_vGroups.begin();iter!=m_vGroups.end();++iter)
	{
		if(msg.pSender==(*iter)->m_pGroupLabel)
		{
			if(msg.sType==_T("click"))
			{
				if((*iter)->IsExpaned())
					(*iter)->Collapse();
				else
					(*iter)->Expand();
				return true;
			}
			break;
		}
		for(auto iter_item=(*iter)->m_vItems.begin();iter_item!=(*iter)->m_vItems.end();++iter_item)
		{
			if(msg.pSender==(*iter_item)->m_pContainer)
			{
				if(msg.sType==_T("mouseenter"))
				{
					(*iter_item)->MouseEnter();
				}
				else if(msg.sType==_T("mouseleave"))
				{
					(*iter_item)->MouseLeave();
				}else if(msg.sType==_T("click"))
				{
					(*iter_item)->Select();
				}
				break;
			}
		}
	} 
	return false;
}
#pragma endregion
