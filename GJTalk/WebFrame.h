#pragma once
#include "gjcontextwnd.h"
class CWebFrame :
	public CGJContextWnd
{
public:
	CWebFrame(void);
	~CWebFrame(void);
	int Navigate(LPCTSTR  pstrUrl);
	void Stop(void);
	void Refresh(void);
	void GoForward(void);
	void GoBack(void);
	CString& GetUrl(void) const;
};

