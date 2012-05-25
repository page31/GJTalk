// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#pragma warning(disable : 4995)


#define SECURITY_WIN32
// Windows
#include <winsock2.h>
#include <windows.h>
#include <psapi.h>
#include <sspi.h>
#include <WinCrypt.h>
#include <MSTcpIP.h>
//#include <Security.h> 
#pragma comment(lib,"Dnsapi.lib")
#pragma comment(lib,"Secur32.lib")
#pragma comment(lib,"Crypt32.lib")
#include <strsafe.h>

//com
#include <comutil.h>

//xml
#import <msxml3.dll>


// STL
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <memory>
#include <sstream>
#include <iomanip>
#include <utility>

#include <schannel.h>

//zlib
#define ZLIB_WINAPI
#include "../zlib/include/zlib.h"

