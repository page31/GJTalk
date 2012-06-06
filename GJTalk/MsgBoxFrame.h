#pragma once
#include "gjwnd.h"
class CMsgBoxFrame :
	public CGJWnd
{
private:
	LONG m_Result;
	CControlUI *m_pBtnOk,*m_pBtnCancel,*m_pBtnYes,*m_pBtnNo,*m_pBtnRetry;
	CControlUI *m_pBtn1,*m_pBtn2,*m_pBtn3;
	CLabelUI *m_pLblTitle;
	CTextUI *m_pTxtContent;
public:
	CMsgBoxFrame(void);
	LONG GetResult() const;
	void SetType(LONG type);
	void SetTitle(LPCTSTR lpszTitle);
	void SetContent(LPCTSTR lpszContent,bool isHtml);
	void SetButtonText(LPCTSTR lpszButton1=NULL,LPCTSTR lpszButton2=NULL,LPCTSTR lpszButton3=NULL);
	void Notify(TNotifyUI& msg);
	virtual void OnPostCreate();
	~CMsgBoxFrame(void);
};

