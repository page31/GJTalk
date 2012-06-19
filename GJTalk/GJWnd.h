#pragma once 
#include "stdafx.h"
namespace CGJWndError
{
	enum WNDERROR
	{
		LOAD_ERROR
	};
}
class CGJWnd:public CWindowWnd,public INotifyUI
{
protected:
	CStdString m_sCaption;
public:  
	CPaintManagerUI m_pm;
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	LRESULT OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,bool &bHandled);
	CGJWnd(void);

	bool InitFromXmlFile( LPCTSTR lpszFilename,IDialogBuilderCallback* builderCallback=NULL);
	LRESULT  HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	void  OnFinalMessage(HWND /*hWnd*/);

	void GetWorkArea(LPRECT pRect);

	void SetCaptionText(LPCTSTR pstrText);
	CStdString GetCaptionText() const;
	virtual	void Notify(TNotifyUI& msg);

	void OnPrepare();
protected:
	CControlUI *m_pBtnClose,*m_pBtnMax,*m_pBtnResotre,
		*m_pBtnMin;
	CControlUI *m_pCaption;
	virtual void OnPostCreate() = 0;

	~CGJWnd(void);
};

