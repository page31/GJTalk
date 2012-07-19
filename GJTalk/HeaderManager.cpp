#include "stdafx.h"
#include "HeaderManager.h"
#include <atlimage.h>
#include <atlcom.h>
#include <GdiPlusImaging.h>

using namespace std;
using namespace gloox;

CHeaderManager::CHeaderManager(void)
{
	TCHAR buffer[MAX_PATH]; 
	SHGetFolderPath(NULL,CSIDL_APPDATA,0,SHGFP_TYPE_CURRENT,buffer);
	m_path.Format(_T("%s\\%s\\Headers"),buffer,_T("GJTalk"));
	SHCreateDirectoryEx(NULL,m_path,NULL);
}


CHeaderManager::~CHeaderManager(void)
{
}

void CHeaderManager::RegisterHandler( HeaderHandler* handler )
{
	if(!handler)
		return;
	for(auto iter=m_handlers.begin();iter!=m_handlers.end();++iter)
		if((*iter)==handler)
			return;
	m_handlers.push_front(handler);
}

void CHeaderManager::RemoveHandler( HeaderHandler *handler )
{
	for (auto iter=m_handlers.begin();iter!=m_handlers.end();++iter)
	{
		if((*iter)==handler)
		{
			m_handlers.erase(iter);
			return;
		}
	}
}

void CHeaderManager::NotifyUpdate( const JID& jid )
{
	for (auto iter=m_handlers.begin();iter!=m_handlers.end();++iter) 
		(*iter)->HandleHeaderUpdate(*this,jid); 
}

void CHeaderManager::UpdateHeader( const JID& jid,const string& val )
{
	LPCTSTR pstrPath_=_T("%s\\%s_%d.png"); 
	CString path;
	if(val.empty())
	{
		path.Format(pstrPath_,m_path,utf8dec(jid.username()),0);
		DeleteFile(path);
		path.Format(pstrPath_,m_path,utf8dec(jid.username()),1);
		DeleteFile(path);
	}
	else
	{
		CImage img;
		HGLOBAL hMem=GlobalAlloc(GMEM_FIXED,val.size());
		BYTE* pMem=(BYTE*)GlobalLock(hMem);
		memcpy(pMem,val.data(),val.size());
		GlobalUnlock(hMem);

		IStream *pStream;
		CreateStreamOnHGlobal(hMem,FALSE,&pStream);

		img.Load(pStream);

		pStream->Release();
		GlobalFree(hMem);

		img.SetPixel(0,0,0);
		img.SetPixel(0,img.GetHeight()-1,0);
		img.SetPixel(img.GetWidth()-1,0,0);
		img.SetPixel(img.GetHeight()-1,img.GetWidth()-1,0);



		path.Format(pstrPath_,m_path,utf8dec(jid.username()) ,1);
		img.Save(path, Gdiplus::ImageFormatPNG);

		BYTE *pData=(BYTE*)img.GetBits(); 
		int bytePerPixel=img.GetBPP()/8;
		int pit=img.GetPitch(); 
		int mh=img.GetHeight();
		int mw=img.GetWidth();
		for (int y = 0; y < mh; y++)
		{
			for (int x = 0; x < mw; x++)
			{
				BYTE *pCur=pit*y+x*bytePerPixel+pData;
				int grayVal=(*pCur)*0.3+(*(pCur+1))*0.59+(*(pCur+2))*0.11;
				*pCur=*(pCur+1)=*(pCur+2)=grayVal; 
			}
		}

		path.Format(pstrPath_,m_path,utf8dec( jid.username()),0);
		img.Save(path);
	}

	NotifyUpdate(jid);
}

CString CHeaderManager::GetHeader(const JID& jid, bool online/*=true*/ ) const
{
	CString strHeader;
	strHeader.Format(_T("%s\\%s_%d.png"),m_path,utf8dec( jid.username()),online?1:0);
	CFileStatus status;
	if(!CFile::GetStatus(strHeader,status))
		strHeader.Format(_T("%s\\default%d.png"),m_path,online?1:0);
	return strHeader;
}

void CHeaderManager::SetPath( LPCTSTR pstrPath )
{
	m_path=pstrPath;
}

CString CHeaderManager::GetPath() const
{
	return m_path;
}
