#include "stdafx.h"
#include "LoginFrame.h"
#include "..\XMPP\TestExportClass.h"
 
 
CLoginFrame::CLoginFrame(GJContext &context)
	:CGJWnd(context)
{ 
	TestExportClass cls;
	cls.Test();
}
CLoginFrame::~CLoginFrame(void)
{
}
