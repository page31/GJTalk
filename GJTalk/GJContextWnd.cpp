#include "stdafx.h"
#include "GJContextWnd.h"


CGJContextWnd::CGJContextWnd(void)
{
}

void CGJContextWnd::SetContext(GJContext* context)
{
	m_pContext=context;
}
GJContext* CGJContextWnd::GetContext() const
{
	return m_pContext;
}
CGJContextWnd::CGJContextWnd(GJContext* context)
	:m_pContext(context)
{
}


CGJContextWnd::~CGJContextWnd(void)
{
}

LPCTSTR CGJContextWnd::GetWindowClassName() const
{
	return CGJWnd::GetWindowClassName();
}

LRESULT CGJContextWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return CGJWnd::HandleMessage(uMsg,wParam,lParam);
}

void CGJContextWnd::OnFinalMessage( HWND hWnd)
{
	CGJWnd::OnFinalMessage(hWnd);
}

void CGJContextWnd::Notify( TNotifyUI& msg )
{
	CGJWnd::Notify(msg);
}
