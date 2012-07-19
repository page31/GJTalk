
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持



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