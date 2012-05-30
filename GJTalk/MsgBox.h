#pragma once
#include "stdafx.h"
namespace msgbox
{
	 
	LONG MsgBox(LPCTSTR lpszContent);
	LONG MsgBox(LPCTSTR lpszContent,LPCTSTR lpszTitle);
	LONG MsgBox(LPCTSTR lpszContent,LONG type);
	LONG MsgBox(LPCTSTR lpszContent,LPCTSTR lpszTitle,LONG type);
}