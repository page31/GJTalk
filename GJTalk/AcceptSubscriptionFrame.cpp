#include "stdafx.h"
#include "AcceptSubscriptionFrame.h"
#include "InputBox.h"


CAcceptSubscriptionFrame::CAcceptSubscriptionFrame(GJContext *context)
	:CGJContextWnd(context)
{
	this->Create(NULL,_T("添加联系人"),UI_WNDSTYLE_FRAME^WS_THICKFRAME,UI_WNDSTYLE_EX_FRAME);
}


CAcceptSubscriptionFrame::~CAcceptSubscriptionFrame(void)
{
}

void CAcceptSubscriptionFrame::OnPostCreate()
{
	InitFromXmlFile(_T("AcceptSubscriptionFrame.xml"));
	m_pBtnAddGroup=FindControl<CButtonUI>(_T("btnAddGroup"));
	m_pEditRemark=FindControl<CEditUI>(_T("editRemark"));
	m_pCBGroups=FindControl<CComboBoxUI>(_T("cbGroups"));

	auto rosterMgr=GetContext()->GetClient()->rosterManager();
	for(auto it=rosterMgr->roster()->begin();it!=rosterMgr->roster()->end();++it)
	{
		StringList groups=it->second->groups();
		for(auto it2=groups.begin();it2!=groups.end();++it2)
		{ 
			AddGroupItem(utf8dec((*it2).c_str()));
		} 
	}
	if(m_pCBGroups->GetCount()==0)
	{
		AddGroupItem(_T("我的好友"));
	}
	if(m_pCBGroups->GetCount()>0)
		m_pCBGroups->SelectItem(0);
}

void CAcceptSubscriptionFrame::Notify( TNotifyUI& msg )
{
	if(msg.pSender)
	{
		CStdString strName=msg.pSender->GetName();
		if(msg.sType==_T("click"))
		{
			if(strName==_T("btnAddGroup"))
			{
#pragma region btnAddGroup

				CString strValue;
				bool bOk=true;
				while (strValue.IsEmpty()&&bOk)
				{

					CInputBox inputBox;
					bOk=inputBox.ShowInput(_T("新建分组"),strValue,_T("输入分组名："),_T(""),*this);
					if(bOk)
					{
						strValue= strValue.Trim();
						for (size_t i=0;i<m_groups.size();i++)
						{ 
							if(m_groups[i]==strValue)
							{
								m_pCBGroups->SelectItem(i);
								break;;
							}
						}
						m_groups.push_back(strValue); 
						CListLabelElementUI *pItem=new CListLabelElementUI();
						pItem->SetText(strValue);
						m_pCBGroups->Add(pItem);
						m_pCBGroups->SelectItem(m_groups.size()-1);
					} 
				} 
#pragma endregion

			}  
			else if(strName==_T("btnAdd"))
			{
				m_strGroup=m_groups[m_pCBGroups->GetCurSel()];
				m_strRemark=m_pEditRemark->GetText();
				this->Close(0);
			}
			else if(strName==_T("btnCancel"))
			{
				this->Close();
			}
		}
	}
	CGJContextWnd::Notify(msg);
}

CString CAcceptSubscriptionFrame::Remark() const
{
	return m_strRemark;
}

CString CAcceptSubscriptionFrame::GroupName() const
{
	return m_strGroup;
}

void CAcceptSubscriptionFrame::AddGroupItem( LPCTSTR pstrName )
{
	CListLabelElementUI *pItem=new CListLabelElementUI();
	pItem->SetText(pstrName);
	m_groups.push_back(pstrName);
	m_pCBGroups->Add(pItem);
}
