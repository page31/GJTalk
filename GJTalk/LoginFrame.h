#pragma once
#include "GJContext.h"
#include "GJWnd.h"
class CLoginFrame:public CGJWnd
{
private:
	CEditUI *m_pEditUser,
		*m_pEditPassword;
protected:
	virtual	void OnPostCreate();
public: 

	void Notify(TNotifyUI& msg);
	CLoginFrame(GJContext &context);
	~CLoginFrame(void); 

};

