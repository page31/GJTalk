#include "stdafx.h"
#include "MessageFilterHelper.h"



LRESULT MessageFilterHelper::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	MSG msg;
	msg.hwnd=NULL;
	msg.message=uMsg;
	msg.wParam=wParam;
	msg.lParam=lParam;
	bHandled=AfxGetApp()->PreTranslateMessage(&msg)==TRUE;
	return 0;
}

MessageFilterHelper::MessageFilterHelper(void)
{
}


MessageFilterHelper::~MessageFilterHelper(void)
{
}
