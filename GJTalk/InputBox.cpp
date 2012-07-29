#include "stdafx.h"
#include "InputBox.h"


CInputBox::CInputBox(void)
{
}


CInputBox::~CInputBox(void)
{ 
	if(IsWindow(m_hWnd))
		this->Close();
}

void CInputBox::OnFinalMessage( HWND )
{

}

void CInputBox::Notify( TNotifyUI& msg )
{
	bool bHandled=true;
	if(msg.sType==_T("click"))
	{
		if(msg.pSender==m_pBtnOK)
		{
			strValue=m_pEditValue->GetText();
			this->Close(0);
		}
		else if(msg.pSender==m_pBtnCancel)
		{
			this->Close(1);
		} 
	}
	CGJWnd::Notify(msg);
}

void CInputBox::OnPostCreate()
{
	ASSERT(InitFromXmlFile(_T("InputBox.xml")));

	m_pTipLabel=FindControl<CLabelUI>(_T("lblTip"));
	m_pTitleLabel=FindControl<CLabelUI>(_T("lblTitle"));
	m_pBtnCancel=FindControl<CButtonUI>(_T("btnCancel"));
	m_pBtnOK=FindControl<CButtonUI>(_T("btnOK"));

	m_pEditValue=FindControl<CEditUI>(_T("editValue"));


}

bool CInputBox::ShowInput(LPCTSTR pstrTitle,CString &strRet,LPCTSTR pstrTip,LPCTSTR pstrDefault,HWND parent )
{
	this->Create(NULL,pstrTitle,UI_WNDSTYLE_DIALOG,UI_WNDSTYLE_EX_DIALOG); 
	this->CenterWindow(); 
	m_pTipLabel->SetText(pstrTip);
	m_pTitleLabel->SetText(pstrTitle);
	m_pEditValue->SetText(pstrDefault);
	bool bOk= this->ShowModal(parent)==0;
	if(bOk)
		strRet=strValue;
	return bOk;
}

