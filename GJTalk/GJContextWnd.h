#pragma once
#include "gjwnd.h"
#include "GJContext.h"
class GJContext;
class CGJContextWnd :
	public CGJWnd
{ 
protected:
	GJContext* m_pContext;
public:
	void SetContext(GJContext* context);
	GJContext* GetContext() const;
	CGJContextWnd(void);
	CGJContextWnd(GJContext* context);
	virtual void OnPostCreate()=0;

	~CGJContextWnd(void);

	virtual LPCTSTR GetWindowClassName() const;

	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual void OnFinalMessage( HWND );

	virtual void Notify( TNotifyUI& msg );

};

