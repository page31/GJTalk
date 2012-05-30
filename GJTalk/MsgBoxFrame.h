#pragma once
#include "gjwnd.h"
class CMsgBoxFrame :
	public CGJWnd
{
private:
	LONG m_Result;
	CControlUI *m_pBtnOk,*m_pBtnCancel,*m_pBtnYes,*m_pBtnNo,*m_pBtnRetry;
	CLabelUI *m_pLblTitle;
	CTextUI *m_pTxtContent;
public:
	CMsgBoxFrame(void);
	LONG GetResult() const;
	void SetType(LONG type);
	void SetTitle(LPCTSTR lpszTitle);
	void SetContent(LPCTSTR lpszContent,bool isHtml);
	virtual void OnPostCreate();
	~CMsgBoxFrame(void);
};

