
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

string cstr_str(CString &cstr)
{
#ifdef _UNICODE
	CStringA cstrA(cstr.GetBuffer());
	cstr.ReleaseBuffer();
	string str=cstrA.GetBuffer();
	cstrA.ReleaseBuffer();
	return str;
#else
	return cstr;
#endif
}

CString utf8dec(const string& encodedstr)
{
	CString strRet;
	wchar_t *buffer=NULL;
	int cchStr=encodedstr.size();
	int cchBuff=::MultiByteToWideChar(CP_UTF8,0,encodedstr.data(),cchStr,NULL,0);
 
	if(cchBuff>0)
	{
		buffer=new wchar_t[cchBuff+1];
		::MultiByteToWideChar(CP_UTF8,0,encodedstr.data(),cchStr,buffer,cchBuff);
		buffer[cchBuff]='\0';
	}
	if(buffer)
	{ 
		strRet=CString(buffer);
		delete[] buffer;
	} 
	return strRet;
}

