#pragma once

class CTrayIcon;
// CTrayIconMessage

class CTrayIconMessage
{ 
public:
	CTrayIconMessage();
	CString sMsg;
	CTrayIcon* pSender;
	WPARAM wParam;
	LPARAM lParam;
	bool bHandled;
	virtual ~CTrayIconMessage();
};


