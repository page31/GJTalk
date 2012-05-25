// XMPP.h : main header file for the XMPP DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CXMPPApp
// See XMPP.cpp for the implementation of this class
//

class CXMPPApp : public CWinApp
{
public:
	CXMPPApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
