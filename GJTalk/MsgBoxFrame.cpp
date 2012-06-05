#include "stdafx.h"
#include "MsgBoxFrame.h"


LONG CMsgBoxFrame::GetResult() const
{
	return m_Result;
}


CMsgBoxFrame::CMsgBoxFrame(void)
{
}
void CMsgBoxFrame::SetType(LONG type)
{

}
void CMsgBoxFrame::SetTitle(LPCTSTR lpszTitle)
{
	if(m_pLblTitle)
		m_pLblTitle->SetText(lpszTitle);
}
void CMsgBoxFrame::SetContent(LPCTSTR lpszContent,bool isHtml)
{
	if(m_pTxtContent)
	{
		m_pTxtContent->SetShowHtml(isHtml);
		m_pTxtContent->SetText(lpszContent);
	}
}

void CMsgBoxFrame::OnPostCreate()
{
	if(!this->InitFromXmlFile(_T("MsgBox.xml")))
		throw CGJWndError::LOAD_ERROR;
	m_pBtnCancel=static_cast<CButtonUI*> (m_pm.FindControl(_T("btnCancel")));
	m_pBtnOk=static_cast<CButtonUI*> (m_pm.FindControl(_T("btnOK")));
	m_pBtnYes=static_cast<CButtonUI*> (m_pm.FindControl(_T("btnYes")));
	m_pBtnNo=static_cast<CButtonUI*> (m_pm.FindControl(_T("btnNo")));
	m_pBtnRetry=static_cast<CButtonUI*> (m_pm.FindControl(_T("btnRetry")));

	m_pLblTitle=static_cast<CLabelUI*>(m_pm.FindControl(_T("lblTitle")));
	m_pTxtContent=static_cast<CTextUI*>(m_pm.FindControl(_T("txtContent")));
}
CMsgBoxFrame::~CMsgBoxFrame(void)
{
}
