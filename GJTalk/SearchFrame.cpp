#include "stdafx.h"
#include "SearchFrame.h"
#include "..\xmpp\iq.h"
#include "..\xmpp\dataformitem.h"
#include "MenuWnd.h"
#include "AddBuddyFrame.h"

#define WM_ADDRESULT WM_USER+0x0050

CSearchFrame::CSearchFrame(GJContext *context)
	:CGJContextWnd(context),m_pClient(NULL) ,m_pSearch(NULL)
{
	this->Create(NULL,_T("查找联系人"),UI_WNDSTYLE_DIALOG,UI_WNDSTYLE_EX_DIALOG);
	this->CenterWindow();
	this->ShowWindow(false);
}


CSearchFrame::~CSearchFrame(void)
{
}

void CSearchFrame::Notify( TNotifyUI& msg )
{
	if(msg.sType==_T("click"))
	{
		if(msg.pSender==this->m_pBtnClose||msg.pSender==CGJWnd::m_pBtnClose)
		{
			this->ShowWindow(false);
			return;
		}
		else if(msg.pSender==m_pBtnSearch)
		{ 
			if(m_pTxtKeyword->GetTextLength()==0)
			{
				m_pTxtKeyword->SetFocus();
				return;
			}
			else
			{
				DoSearch(m_pTxtKeyword->GetText());
			}
		}
	}
	else if(msg.sType==_T("menu"))
	{
		if(msg.pSender==m_pResultList)
		{ 
			if(m_pResultList->GetCurSel()>=0)
			{ 
				CMenuWnd *menu=new CMenuWnd();
				POINT pt=msg.ptMouse;
				ClientToScreen(*this,&pt);
				menu->Init(*this,pt,_T("SearchResultMenu.xml"),this);
				menu->Show();
			}
		}

	}
	CGJContextWnd::Notify(msg);
}

void CSearchFrame::OnPostCreate()
{
	ASSERT(InitFromXmlFile(_T("SearchFrame.xml")));
	m_pBtnClose=FindControl<CButtonUI>(_T("btnClose"));
	m_pBtnSearch=FindControl<CButtonUI>(_T("btnSearch"));
	m_pResultList=FindControl<CListUI>(_T("resultList"));
	m_pTxtKeyword=FindControl<CRichEditUI>(_T("txtKeyword"));

	ASSERT(m_pBtnClose&&m_pBtnSearch&&m_pResultList&&m_pTxtKeyword);
	m_pResultList->SetTextCallback(this);
}



void CSearchFrame::handleSearchFields( const JID& directory, int fields, const std::string& instructions )
{
	throw std::exception("The method or operation is not implemented.");
}

void CSearchFrame::handleSearchFields( const JID& directory, const DataForm* form )
{
	throw std::exception("The method or operation is not implemented.");
}

void CSearchFrame::handleSearchResult( const JID& directory, const SearchResultList& resultList )
{
	throw std::exception("The method or operation is not implemented.");
}

void CSearchFrame::handleSearchResult( const JID& directory, const DataForm* form )
{
	const DataFormReported *reported=form->reported();
	ClearResult();
	for(auto it=form->items().begin();it!=form->items().end();++it)
	{
		DataFormItem *item = *it;
		CString nickname = utf8dec(item->field("Name")->value());
		CString email = utf8dec(item->field("Email")->value());
		CString username = utf8dec(item->field("Username")->value());
		JID jid=item->field("jid")->value();
		CString signature=utf8dec(item->field("Signature")->value());
		AddResult(jid,username,nickname,email,signature);
	}
}

void CSearchFrame::handleSearchError( const JID& directory, const Error* error )
{

	throw std::exception("The method or operation is not implemented.");
}

bool CSearchFrame::DoSearch( LPCTSTR pstrKeyword )
{
	if(!m_pSearch||m_pClient!=GetContext()->GetClient())
	{
		if(m_pSearch)
			delete m_pSearch;
		m_pSearch=NULL;
		if(!GetContext()->GetClient())
			return false;
		m_pSearch=new Search(GetContext()->GetClient());
	}
	gloox::DataForm *form=new DataForm(TypeSubmit);
	form->addField(gloox::DataFormField::TypeHidden,"FORM_TYPE",EmptyString,"jabber:iq:search");
	form->addField(gloox::DataFormField::TypeTextSingle,"search",utf8enc( pstrKeyword),"Search");
	form->addField(gloox::DataFormField::TypeBoolean,"Username","1","Username");
	form->addField(gloox::DataFormField::TypeBoolean,"Name","1","Name");
	form->addField(gloox::DataFormField::TypeBoolean,"Email","1","Email");
	ClearResult();
	m_pSearch->search(JID("search.gjtalk.com"),form,this);
	return true;
}

void CSearchFrame::ClearResult()
{
	m_pResultList->RemoveAll();
	m_items.clear();
}

void CSearchFrame::AddResult( const JID &jid,LPCTSTR pstrUsername,LPCTSTR pstrNickname,LPCTSTR pstrEmail,LPCTSTR pstrSignature )
{
	tagSearchResult *result=new tagSearchResult();
	result->jid=jid;
	result->username=pstrUsername;
	result->nickname=pstrNickname;
	result->email=pstrEmail;
	result->signature=pstrSignature;
	AddResult(result);
}

LRESULT CSearchFrame::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(uMsg==WM_ADDRESULT)
	{
		AddResult((tagSearchResult*)lParam);
		return 0;
	} 
	return CGJContextWnd::HandleMessage(uMsg,wParam,lParam);
}

void CSearchFrame::AddResult( tagSearchResult *result )
{
	if(IsCrossThread())
	{
		PostMessage(WM_ADDRESULT,0,(LPARAM)result);
		return;
	}
	vector<CString> item;
	item.push_back(result->nickname);
	item.push_back(result->signature);
	item.push_back(utf8dec(result->jid.bare()));
	m_items.push_back(item);
	CListTextElementUI *control=new CListTextElementUI();

	m_pResultList->Add(control); 
	delete result;
}

LPCTSTR CSearchFrame::GetItemText( CControlUI* pList, int iItem, int iSubItem )
{
	LPCTSTR pText=NULL; 
	pText= m_items[iItem][iSubItem]; 
	return pText;
}

void CSearchFrame::OnMenu( CMenuWnd *pMenu,CControlUI* pSender,LPCTSTR sType )
{ 
	if(_tcsicmp(sType,_T("click"))==0)
	{
		CStdString pstrName=pSender->GetName();
		if(pstrName==_T("menu_add"))
		{ 
			/*	gloox::StringList groups; 
			GetContext()->GetClient()->rosterManager()->subscribe(utf8enc(m_items[m_pResultList->GetCurSel()][2]),
			EmptyString,groups);*/

			CAddBuddyFrame::OpenFor(JID(utf8enc(m_items[m_pResultList->GetCurSel()][2])),GetContext());


		}
		else if(pstrName==_T("menu_profile"))
		{ 

		}
	}
}

void CSearchFrame::OnOpenning( CMenuWnd *pMenu )
{

}
