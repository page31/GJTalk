
// stdafx.cpp : 只包括标准包含文件的源文件
// GJTalk.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

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