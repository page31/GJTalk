#include "stdafx.h"
#include "MsgBox.h" 
#include "MsgBoxFrame.h"
namespace msgbox
{
	LONG MsgBox(LPCTSTR lpszContent,LONG type)
	{  
		return MsgBox(lpszContent,AfxGetAppName(),
			type);
	}


	LONG MsgBox(LPCTSTR lpszContent,LPCTSTR lpszTitle,LONG type)
	{
	
		CMsgBoxFrame frame;
		frame.Create(GetFocus(),lpszTitle,UI_WNDSTYLE_DIALOG,0L);
		frame.SetTitle(lpszTitle);
		frame.SetContent(lpszContent,true);
		frame.SetType(type);
		frame.CenterWindow();
		frame.ShowModal();
		return frame.GetResult();
	}


	LONG MsgBox(LPCTSTR lpszContent,LPCTSTR lpszTitle)
	{
		return MsgBox(lpszContent,lpszTitle,MB_OK);
	}

	LONG MsgBox(LPCTSTR lpszContent)
	{
		return MsgBox(lpszContent,MB_OK);
	}
}