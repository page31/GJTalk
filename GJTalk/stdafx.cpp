
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
		buffer=new wchar_t[cchBuff+2];
		::MultiByteToWideChar(CP_UTF8,0,encodedstr.data(),cchStr,buffer,cchBuff);
		buffer[cchBuff]='\0';
		buffer[cchBuff+1]='\0';
	}
	if(buffer)
	{ 
		strRet=CString(buffer);
		delete[] buffer;
	} 
	return strRet;
}

std::string utf8enc( const wchar_t * const pstr )
{
	string strRet;
	char *buffer=NULL;
	int cchStr=(int)_tcslen(pstr);
	int cchBuff
		=::WideCharToMultiByte(CP_UTF8,
		0,pstr,cchStr,NULL,0,NULL,NULL);
	if(cchBuff>0)
	{
		buffer=new char[cchBuff+1];
		::WideCharToMultiByte(CP_UTF8,0,pstr,cchStr,
			buffer,cchBuff,NULL,NULL);
		buffer[cchBuff]='\0';
	}
	if(buffer)
	{ 
		strRet=buffer;
		delete[] buffer; 
	}
	return strRet;
}

CString GetCurrentTimeString(bool includeDate/*=false*/ )
{ 
	return GetTimeString(time(NULL),includeDate);
}

CString GetTimeString( time_t time,bool includeDate/*=false*/ )
{
	tm ti;
	tm *timeinfo=&ti;
	localtime_s(timeinfo,&time); 
	CString strTime; 
	if(includeDate)
		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
		timeinfo->tm_year,timeinfo->tm_mon,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	else 
		strTime.Format(_T("%02d:%02d:%02d"),
		timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	return strTime; 
}

list<string> SingleStringList( const string &str )
{
	list<string> list;
	list.push_back(str);
	return list;
}

