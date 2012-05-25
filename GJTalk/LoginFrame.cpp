#include "stdafx.h"
#include "LoginFrame.h"
#include "../xmpp/client.h"


CLoginFrame::CLoginFrame(GJContext &context)
	:CGJWnd(context)
{  
	context.signIn("test","123");

}
CLoginFrame::~CLoginFrame(void)
{
}
