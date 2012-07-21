#pragma once
#include "GJContextWnd.h"
#include "../xmpp/search.h"
#include "../DuiLib/UIList.h"


typedef struct
{
	JID jid;
	CString username;
	CString nickname;
	CString email;
	CString signature;
} tagSearchResult;

class CSearchFrame :
	public CGJContextWnd,
	public gloox::SearchHandler,
	public IListCallbackUI
{
private:
	vector<vector<CString>> m_items;
	gloox::Client *m_pClient;

	CButtonUI *m_pBtnSearch;
	CButtonUI *m_pBtnClose;
	CRichEditUI *m_pTxtKeyword;
	CListUI *m_pResultList;

	Search *m_pSearch;
public:
	CSearchFrame(GJContext * context);
	~CSearchFrame(void);




	bool DoSearch(LPCTSTR pstrKeyword);

	void ClearResult();
	void AddResult(const JID &jid,LPCTSTR pstrUsername,LPCTSTR pstrNickname,LPCTSTR pstrEmail,LPCTSTR pstrSignature);
	void AddResult(tagSearchResult *result);

	virtual void Notify( TNotifyUI& msg );


	virtual void OnPostCreate();

	virtual void handleSearchFields( const JID& directory, int fields, const std::string& instructions );

	virtual void handleSearchFields( const JID& directory, const DataForm* form );

	virtual void handleSearchResult( const JID& directory, const SearchResultList& resultList );

	virtual void handleSearchResult( const JID& directory, const DataForm* form );

	virtual void handleSearchError( const JID& directory, const Error* error );

	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual LPCTSTR GetItemText( CControlUI* pList, int iItem, int iSubItem );

};

