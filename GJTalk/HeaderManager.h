#pragma once
#include <list> 
#include "../xmpp/jid.h"

class CHeaderManager;

class HeaderHandler
{
public:
	virtual void HandleHeaderUpdate(const CHeaderManager &manager,const gloox::JID& jid)=0;
};

class CHeaderManager
{
private:
	std::list<HeaderHandler*> m_handlers; 
	CString m_path;
public:

	void SetPath(LPCTSTR pstrPath);
	CString GetPath() const;

	void RegisterHandler(HeaderHandler* handler);
	void RemoveHandler(HeaderHandler *handler);
	void NotifyUpdate(const gloox::JID& jid);
	void UpdateHeader(const gloox::JID& jid,const string& val);
	CString GetHeader(const gloox::JID& jid,bool online=true) const;
	CHeaderManager(void);
	~CHeaderManager(void);
};

