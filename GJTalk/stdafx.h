
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��



#include "..\DuiLib\UIlib.h"

using namespace DuiLib;



#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\bin\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\bin\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\bin\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\bin\\DuiLib.lib")
#   endif
#endif




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define GLOOX_IMPORTS
#pragma comment(lib,"..\\bin\\XMPP.lib")


#include "GJContext.h"
#include "MsgBox.h"

#define DM_CROSSTHREAD_NOTIFY WM_USER+0x10


#ifndef RGBA
#define RGBA(r,g,b,a)        ((COLORREF)( (((DWORD)(BYTE)(a))<<24) |    RGB(r,g,b) ))
#endif

#ifndef GetAValue
#define GetAValue(rgba)  (LOBYTE((rgba)>>24))
#endif

bool IsCrossThread();

using namespace msgbox;

string cstr_str(CString &cstr);
CString utf8dec(const string& encodedstr);
string utf8enc(const wchar_t * const pstr)
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