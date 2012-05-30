
// GJTalk.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "GJContext.h"

class GJContext;
// CGJTalkApp:
// 有关此类的实现，请参阅 GJTalk.cpp
//

class CGJTalkApp : public CWinApp
{

public:
	GJContext *m_pContext;

	CGJTalkApp();

	// 重写
public:
	virtual BOOL InitInstance();
	void OnCrossThreadNotify(WPARAM wParam,LPARAM lParam);
	// 实现
	DECLARE_MESSAGE_MAP();
};

extern CGJTalkApp theApp;