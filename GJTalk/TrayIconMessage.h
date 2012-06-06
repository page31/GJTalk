#pragma once

class CTrayIcon;
// CTrayIconMessage

class CTrayIconMessage
{ 
public:
	CTrayIconMessage();
	LPCTSTR pMsg;
	CTrayIcon* pSender;
	WPARAM wParam;
	LPARAM lParam;
	bool bHandled;
	virtual ~CTrayIconMessage();
};


