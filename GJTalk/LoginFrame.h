#pragma once
#include "GJContext.h"
#include "GJContextWnd.h"
#include "../xmpp/client.h"
using namespace gloox;
class CLoginFrame:public CGJContextWnd
{
private:
	CEditUI *m_pEditUser,
		*m_pEditPassword;
protected:
	virtual	void OnPostCreate();
public: 
	void OnConnected();
	void OnDisconnected( ConnectionError error);
	void Notify(TNotifyUI& msg);
	CLoginFrame(GJContext *context);
	~CLoginFrame(void); 

};

