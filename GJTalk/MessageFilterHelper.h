#pragma once
class MessageFilterHelper:public DuiLib::IMessageFilterUI
{
public:
	MessageFilterHelper(void);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) ;
	~MessageFilterHelper(void);
};

