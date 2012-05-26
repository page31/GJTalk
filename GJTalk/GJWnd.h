#pragma once 

class GJContext;
class CGJWnd:public CWindowWnd,public INotifyUI
{
protected:
	GJContext &context;

public:  
	CPaintManagerUI m_pm;
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	LRESULT OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,bool &bHandled);
	CGJWnd(GJContext &context);
	GJContext &GetContext() const;

	LRESULT  HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual	void Notify(TNotifyUI& msg);
	void OnPrepare();
protected:
	CControlUI *m_pBtnClose,*m_pBtnMax,*m_pBtnResotre,
		*m_pBtnMin;
	virtual void OnPostCreate() = 0;

	~CGJWnd(void);
};

