
// GJTalk.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "GJContext.h"

// CGJTalkApp:
// �йش����ʵ�֣������ GJTalk.cpp
//

class CGJTalkApp : public CWinApp
{

public:
	CGJTalkApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGJTalkApp theApp;