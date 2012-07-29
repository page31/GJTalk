#pragma once
#include "gjwnd.h"
class CInputBox :
	public CGJWnd
{
private:
	CLabelUI *m_pTitleLabel;
	CLabelUI *m_pTipLabel;
	CEditUI *m_pEditValue;
	CButtonUI *m_pBtnOK;
	CButtonUI *m_pBtnCancel;

	CString strValue;
public:
	CInputBox(void);
	~CInputBox(void);

	virtual void OnFinalMessage( HWND );

	virtual void Notify( TNotifyUI& msg );

	virtual void OnPostCreate();

	bool ShowInput(LPCTSTR pstrTitle,CString &strRet,LPCTSTR pstrTip=NULL,LPCTSTR pstrDefault=NULL,HWND parnet=NULL);


};

