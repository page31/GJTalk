#pragma once
#include "gjcontextwnd.h"
#include <comdef.h>
#include <ExDisp.h>

class CWebFrame :
	public CGJContextWnd
{
private:
	CActiveXUI* m_pHost; 
	IWebBrowser2* m_pBrowser;
public:
	CWebFrame(void);
	CWebFrame( GJContext *context);
	~CWebFrame(void);
	void Navigate(LPCTSTR  pstrUrl);
	void Stop(void);
	void Refresh(void);
	void GoForward(void);
	void GoBack(void);


	CString GetUrl(void) const;

	void OnPostCreate();
};

