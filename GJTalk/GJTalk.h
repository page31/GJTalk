
// GJTalk.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "GJContext.h"

class GJContext;
// CGJTalkApp:
// �йش����ʵ�֣������ GJTalk.cpp
//

class CGJTalkApp : public CWinApp
{

public:
	GJContext *m_pContext;

	CGJTalkApp();

	// ��д
public:
	virtual BOOL InitInstance();
	void OnCrossThreadNotify(WPARAM wParam,LPARAM lParam);
	// ʵ��
	DECLARE_MESSAGE_MAP();
};

extern CGJTalkApp theApp;