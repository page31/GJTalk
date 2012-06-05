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
