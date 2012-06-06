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
	switch (type)
	{
	case MB_OKCANCEL:
		m_pBtnOk=m_pBtn2;
		m_pBtnCancel=m_pBtn1;
		this->SetButtonText(_T("取消"),_T("确定"));
		m_Result=IDCANCEL;
		break;
	case MB_YESNO:
		m_pBtnNo=m_pBtn1;
		m_pBtnYes=m_pBtn2;
		this->SetButtonText(_T("否"),_T("是"));
		m_Result=IDNO;
		break;
	case MB_YESNOCANCEL:
		m_pBtnCancel=m_pBtn1;
		m_pBtnNo=m_pBtn2;
		m_pBtnYes=m_pBtn3;
		this->SetButtonText(_T("取消"),_T("否"),_T("是"));
		m_Result=IDCANCEL;
		break;
	case MB_RETRYCANCEL:
		m_pBtnCancel=m_pBtn1;
		m_pBtnRetry=m_pBtn2;
		this->SetButtonText(_T("取消"),_T("重试"));
		m_Result=IDCANCEL;
		break;
	default:
		m_pBtnOk=m_pBtn1; 
		this->SetButtonText(_T("确定"));
		m_Result=IDOK;
		break;
	}
}

void CMsgBoxFrame::Notify(TNotifyUI& msg)
{
	if(msg.sType==_T("click"))
	{
		if(msg.pSender==m_pBtnOk)
			m_Result=IDOK;
		else if(msg.pSender==m_pBtnYes)
			m_Result=IDYES;
		else if(msg.pSender==m_pBtnNo)
			m_Result=IDNO;
		else if(msg.pSender==m_pBtnRetry)
			m_Result=IDRETRY;
		else if(msg.pSender==m_pBtnCancel)
			m_Result=IDCANCEL; 
		else if(msg.pSender!=m_pBtnClose)
		{
			CGJWnd::Notify(msg);
			return;
		}
		this->Close(m_Result);
	}
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
void CMsgBoxFrame::SetButtonText(LPCTSTR lpszButton1,LPCTSTR lpszButton2,LPCTSTR lpszButton3)
{

	m_pBtn1->SetVisible(true);
	m_pBtn2->SetVisible(false);
	m_pBtn3->SetVisible(false);
	m_pBtn1->SetText(lpszButton1);
	if(lpszButton2)
	{
		m_pBtn2->SetVisible(true);
		m_pBtn2->SetText(lpszButton2);
		if(lpszButton3)
		{
			m_pBtn3->SetVisible(true);
			m_pBtn3->SetText(lpszButton3);
		}
	}
}

void CMsgBoxFrame::OnPostCreate()
{
	if(!this->InitFromXmlFile(_T("MsgBox.xml")))
		throw CGJWndError::LOAD_ERROR;
	m_pBtn1=m_pm.FindControl(_T("btnOption1"));
	m_pBtn2=m_pm.FindControl(_T("btnOption2"));
	m_pBtn3=m_pm.FindControl(_T("btnOption3"));

	m_pLblTitle=static_cast<CLabelUI*>(m_pm.FindControl(_T("lblTitle")));
	m_pTxtContent=static_cast<CTextUI*>(m_pm.FindControl(_T("txtContent")));
}
CMsgBoxFrame::~CMsgBoxFrame(void)
{
}
