#include "stdafx.h"
#include "WebFrame.h"

CWebFrame::CWebFrame( GJContext *context)
	:CGJContextWnd(context),m_pHost(NULL),m_pBrowser(NULL)
{
	this->Create(NULL,m_pContext->GetAppName(),UI_WNDSTYLE_DIALOG,0L);
}
CWebFrame::CWebFrame(void):m_pHost(NULL),m_pBrowser(NULL)
{
	this->Create(NULL,m_pContext->GetAppName(),UI_WNDSTYLE_DIALOG,0L);
}


CWebFrame::~CWebFrame(void)
{
	if(m_pBrowser)
		m_pBrowser->Release();
}


void CWebFrame::Navigate(LPCTSTR  pstrUrl)
{
	if(m_pBrowser)
	{ 
		m_pBrowser->Navigate(_bstr_t(pstrUrl),NULL,NULL,NULL,NULL); 
	}
}


void CWebFrame::Stop(void)
{
	if(m_pBrowser)
	{
		m_pBrowser->Stop();
	}
}


void CWebFrame::Refresh(void)
{
	if(m_pBrowser) 
		m_pBrowser->Refresh(); 
}


void CWebFrame::GoForward(void)
{
	if(m_pBrowser)
		m_pBrowser->GoForward();
}


void CWebFrame::GoBack(void)
{
	if(m_pBrowser)
		m_pBrowser->GoBack();
}

void CWebFrame::OnPostCreate()
{
	if(!this->InitFromXmlFile(_T("WebHostFrame.xml")))
		throw CGJWndError::LOAD_ERROR;
	m_pHost=static_cast<CActiveXUI*>(m_pm.FindControl(_T("Host")));
	if(!m_pHost)
		throw CGJWndError::LOAD_ERROR;
	m_pHost->GetControl(IID_IWebBrowser2,(void**)&m_pBrowser);
	if(!m_pBrowser)
		throw CGJWndError::LOAD_ERROR;  
}

CString CWebFrame::GetUrl(void) const
{
	if(m_pBrowser)
	{
		_bstr_t bstr;
		HRESULT hr=m_pBrowser->get_LocationURL(bstr.GetAddress());
		if(SUCCEEDED(hr))
			return CString((LPCTSTR)bstr); 
	} 
	return CString( _T(""));
}
