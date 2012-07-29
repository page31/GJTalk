#include "stdafx.h"
#include "AddBuddyFrame.h"
#include "InputBox.h"


map<std::string,CAddBuddyFrame*> CAddBuddyFrame::m_opened;

CAddBuddyFrame::CAddBuddyFrame(const JID &jid,GJContext *pContext)
	:CGJContextWnd(pContext)
{
	this->Create(NULL,_T("添加联系人"),UI_WNDSTYLE_EX_DIALOG,UI_WNDSTYLE_EX_DIALOG);
	m_opened[jid.bare()]=this;
	m_jid=jid.bareJID();
}


CAddBuddyFrame::~CAddBuddyFrame(void)
{
	m_opened.erase(m_jid.bare());
}

void CAddBuddyFrame::OnPostCreate()
{
	InitFromXmlFile(_T("AddBuddyFrame.xml"));

	m_pBtnAddGroup=FindControl<CButtonUI>(_T("btnAddGroup"));
	m_pEditMsg=FindControl<CEditUI>(_T("editMsg"));
	m_pEditRemark=FindControl<CEditUI>(_T("editRemark"));
	m_pCBGroups=FindControl<CComboBoxUI>(_T("cbGroups"));

	auto rosterMgr=GetContext()->GetClient()->rosterManager();
	for(auto it=rosterMgr->roster()->begin();it!=rosterMgr->roster()->end();++it)
	{
		StringList groups=it->second->groups();
		for(auto it2=groups.begin();it2!=groups.end();++it2)
		{
			CString strGroupName=utf8dec((*it2).c_str());
			CListLabelElementUI *pItem=new CListLabelElementUI();
			pItem->SetText(strGroupName);
			m_groups.push_back(strGroupName);
			m_pCBGroups->Add(pItem);
		} 
	}
	if(m_pCBGroups->GetCount()>0)
		m_pCBGroups->SelectItem(0);
}

void CAddBuddyFrame::Notify( TNotifyUI& msg )
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
						for (int i=0;i<m_groups.size();i++)
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
				StringList group;
				group.push_back(utf8enc(m_groups[m_pCBGroups->GetCurSel()]));
				GetContext()->GetClient()->rosterManager()->subscribe(m_jid,
					utf8enc(m_pEditRemark->GetText()),group,utf8enc(m_pEditMsg->GetText()));
				this->Close();
			}

		}
	}
	CGJContextWnd::Notify(msg);
}

void CAddBuddyFrame::OpenFor(const JID &jid,GJContext *pContext )
{
	auto it= m_opened.find(jid.bare());
	if(it!=m_opened.end())
	{
		(*it->second).ShowWindow();
		return;
	}
	CAddBuddyFrame *frame=new CAddBuddyFrame(jid,pContext); 
	frame->CenterWindow();
	frame->ShowWindow();

}

void CAddBuddyFrame::OnFinalMessage( HWND hwnd)
{
	if(hwnd==m_hWnd)
		delete this;
}
