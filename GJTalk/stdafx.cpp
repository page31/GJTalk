
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// GJTalk.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


bool IsCrossThread()
{
#ifdef _DEBUG
	bool res=AfxGetThread()->m_hThread!=AfxGetApp()->m_hThread;
	return res;
#else
	return AfxGetThread()->m_hThread!=AfxGetApp()->m_hThread;
#endif 
}