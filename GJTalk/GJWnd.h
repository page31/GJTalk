#pragma once 

class GJContext;
class CGJWnd:public CWindowWnd
{
protected:
	GJContext &context;
public:  
	CPaintManagerUI m_pm;
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	LRESULT OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,bool &bHandled);
	CGJWnd(GJContext &context);
	GJContext &GetContext();
 

	~CGJWnd(void);
};

