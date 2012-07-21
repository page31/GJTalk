
//=--------------------------------------------------------------------------=
//  (C) Copyright 1996-1999 Microsoft Corporation. All Rights Reserved.
//=--------------------------------------------------------------------------=


// 
// NOTE: 
// Some of the code in this file is MFC implementation specific.
// Changes in future versions of MFC implementation may require
// the code to be changed. Please check the readme of this
// sample for more information 
// 

#include "stdafx.h"
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT


// NOTE: THis line is a hardcoded reference to an MFC header file
//  this path may need to be changed to refer to the location of VC5 install
//  for successful compilation.


#undef AFX_DATA
#define AFX_DATA AFX_DATA_EXPORT

#include "custsite.h"
#include <urlmon.h>


BEGIN_INTERFACE_MAP(CCustomControlSite, COleControlSite)
	INTERFACE_PART(CCustomControlSite, IID_IDocHostUIHandler, DocHostUIHandler)
	INTERFACE_PART(CCustomControlSite, IID_IInternetSecurityManager, InternetSecurityManager)
	INTERFACE_PART(CCustomControlSite, IID_IServiceProvider, ServiceProvider)
END_INTERFACE_MAP()

	

ULONG FAR EXPORT  CCustomControlSite::XDocHostUIHandler::AddRef()
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
	return pThis->ExternalAddRef();
}


ULONG FAR EXPORT  CCustomControlSite::XDocHostUIHandler::Release()
{                            
    METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
	return pThis->ExternalRelease();
}

HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::QueryInterface(REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
	return hr;
}

// * CImpIDocHostUIHandler::GetHostInfo
// *
// * Purpose: Called at initialization
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::GetHostInfo( DOCHOSTUIINFO* pInfo )
{

	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return S_OK;
}

// * CImpIDocHostUIHandler::ShowUI
// *
// * Purpose: Called when MSHTML.DLL shows its UI
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::ShowUI(
				DWORD dwID, 
				IOleInPlaceActiveObject * /*pActiveObject*/,
				IOleCommandTarget * pCommandTarget,
				IOleInPlaceFrame * /*pFrame*/,
				IOleInPlaceUIWindow * /*pDoc*/)
{

	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return S_OK;
}

// * CImpIDocHostUIHandler::HideUI
// *
// * Purpose: Called when MSHTML.DLL hides its UI
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::HideUI(void)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return S_OK;
}

// * CImpIDocHostUIHandler::UpdateUI
// *
// * Purpose: Called when MSHTML.DLL updates its UI
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::UpdateUI(void)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
	return S_OK;
}

// * CImpIDocHostUIHandler::EnableModeless
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::EnableModeless
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::EnableModeless(BOOL /*fEnable*/)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::OnDocWindowActivate
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::OnDocWindowActivate
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::OnDocWindowActivate(BOOL /*fActivate*/)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::OnFrameWindowActivate
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::OnFrameWindowActivate
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::OnFrameWindowActivate(BOOL /*fActivate*/)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::ResizeBorder
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::ResizeBorder
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::ResizeBorder(
				LPCRECT /*prcBorder*/, 
				IOleInPlaceUIWindow* /*pUIWindow*/,
				BOOL /*fRameWindow*/)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::ShowContextMenu
// *
// * Purpose: Called when MSHTML.DLL would normally display its context menu
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::ShowContextMenu(
				DWORD /*dwID*/, 
				POINT* /*pptPosition*/,
				IUnknown* /*pCommandTarget*/,
				IDispatch* /*pDispatchObjectHit*/)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return S_OK; // We've shown our own context menu. MSHTML.DLL will no longer try to show its own.
}

// * CImpIDocHostUIHandler::TranslateAccelerator
// *
// * Purpose: Called from MSHTML.DLL's TranslateAccelerator routines
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::TranslateAccelerator(LPMSG lpMsg,
            /* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
            /* [in] */ DWORD nCmdID)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return S_FALSE;
}

// * CImpIDocHostUIHandler::GetOptionKeyPath
// *
// * Purpose: Called by MSHTML.DLL to find where the host wishes to store 
// *	its options in the registry
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::GetOptionKeyPath(BSTR* pbstrKey, DWORD)
{

	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}

STDMETHODIMP CCustomControlSite::XDocHostUIHandler::GetDropTarget( 
            /* [in] */ IDropTarget __RPC_FAR *pDropTarget,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}	


STDMETHODIMP CCustomControlSite::XDocHostUIHandler::GetExternal( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

        
STDMETHODIMP CCustomControlSite::XDocHostUIHandler::TranslateUrl( 
            /* [in] */ DWORD dwTranslate,
            /* [in] */ OLECHAR __RPC_FAR *pchURLIn,
            /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}
        
STDMETHODIMP CCustomControlSite::XDocHostUIHandler::FilterDataObject( 
            /* [in] */ IDataObject __RPC_FAR *pDO,
            /* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}


//////////////////////////////////////////////////////////////////////////////
// InternetSecurityManager Methods
HRESULT FAR EXPORT CCustomControlSite::XInternetSecurityManager
										::QueryInterface(REFIID riid, void** ppvObj)
{

	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)
    HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
	return hr;
}

ULONG FAR EXPORT CCustomControlSite::XInternetSecurityManager::AddRef()
{
	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)
	return pThis->ExternalAddRef();
}

ULONG FAR EXPORT CCustomControlSite::XInternetSecurityManager::Release()
{                            

	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)
	return pThis->ExternalRelease();
}

HRESULT CCustomControlSite::XInternetSecurityManager
							::SetSecuritySite (IInternetSecurityMgrSite *pSite)
{
	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)
	return INET_E_DEFAULT_ACTION;
}

HRESULT CCustomControlSite::XInternetSecurityManager
							::GetSecuritySite(IInternetSecurityMgrSite **ppSite)
{

	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)
	return INET_E_DEFAULT_ACTION;
}

HRESULT CCustomControlSite::XInternetSecurityManager
							::MapUrlToZone(LPCWSTR pwszUrl,DWORD *pdwZone,DWORD dwFlags)
{
	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)
	return INET_E_DEFAULT_ACTION;
}


HRESULT  FAR EXPORT CCustomControlSite::XInternetSecurityManager
										::GetSecurityId(LPCWSTR pwszUrl,
										BYTE *pbSecurityId,
										DWORD *pcbSecurityId, 
										DWORD dwReserved)
{

	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)
	return INET_E_DEFAULT_ACTION;
}

 
STDMETHODIMP CCustomControlSite::XInternetSecurityManager
				::ProcessUrlAction(	/* [in] */ LPCWSTR pwszUrl,
									/* [in] */ DWORD dwAction,
									/* [size_is][out] */ BYTE __RPC_FAR *pPolicy,
									/* [in] */ DWORD cbPolicy,
									/* [in] */ BYTE __RPC_FAR *pContext,
									/* [in] */ DWORD cbContext,
									/* [in] */ DWORD dwFlags,
									/* [in] */ DWORD dwReserved)
{

	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)

		/*CSecuMgrDlg* pDlg = (CSecuMgrDlg*)AfxGetMainWnd();
		if (NULL == pDlg)
		return INET_E_DEFAULT_ACTION;
		pDlg->UpdateData();*/

	DWORD dwPolicy=URLPOLICY_ALLOW;
	
//	if (dwAction <= URLACTION_ACTIVEX_MAX && dwAction >= URLACTION_ACTIVEX_MIN)
//		dwPolicy = pDlg->m_bNoActiveX ? URLPOLICY_DISALLOW : URLPOLICY_ALLOW;
//	else if ((dwAction <= URLACTION_JAVA_MAX && dwAction >= URLACTION_JAVA_MIN) ||
//				URLACTION_HTML_JAVA_RUN == dwAction)
//		if (pDlg->m_bNoJava)
//			dwPolicy = URLPOLICY_JAVA_PROHIBIT;
//		else
//			return INET_E_DEFAULT_ACTION;
//	else if (dwAction <= URLACTION_SCRIPT_MAX && dwAction >= URLACTION_SCRIPT_MIN)
//		dwPolicy = pDlg->m_bNoScript ? URLPOLICY_DISALLOW : URLPOLICY_ALLOW;
//// !! If the compiler can't find URLACTION_CROSS_DOMAIN_DATA, make sure you are building with
//// !! the latest version of the IE headers -- URLMON.H specifically -- from MSDN Downloads for the 
//// !! Web Workshop or the Platform SDK
//	else if (URLACTION_CROSS_DOMAIN_DATA == dwAction)
//		dwPolicy = pDlg->m_bCrossData ? URLPOLICY_ALLOW : URLPOLICY_DISALLOW;
//	else
//		return INET_E_DEFAULT_ACTION;

	if ( cbPolicy >= sizeof (DWORD))
	{
		*(DWORD*) pPolicy = dwPolicy;
		return S_OK;
	} 
	else 
	{
		return S_FALSE;
	}
}

HRESULT CCustomControlSite::XInternetSecurityManager
							::QueryCustomPolicy(LPCWSTR pwszUrl,
												REFGUID guidKey,
												BYTE **ppPolicy,
												DWORD *pcbPolicy,
												BYTE *pContext,
												DWORD cbContext,
												DWORD dwReserved)
{
	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)
	return INET_E_DEFAULT_ACTION;
}

HRESULT CCustomControlSite::XInternetSecurityManager
							::SetZoneMapping(DWORD dwZone,
											LPCWSTR lpszPattern,
											DWORD dwFlags)
{
	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)
	return INET_E_DEFAULT_ACTION;
}

HRESULT CCustomControlSite::XInternetSecurityManager
							::GetZoneMappings(DWORD dwZone,
											IEnumString **ppenumString, 
											DWORD dwFlags)
{
	METHOD_PROLOGUE(CCustomControlSite, InternetSecurityManager)
	return INET_E_DEFAULT_ACTION;
}

/////////////////////////////////////////////////////////////
// ServiceProvider Methods
ULONG FAR EXPORT CCustomControlSite::XServiceProvider::AddRef()
{
	METHOD_PROLOGUE(CCustomControlSite, ServiceProvider)
	return pThis->ExternalAddRef();
}

ULONG FAR EXPORT CCustomControlSite::XServiceProvider::Release()
{                            
	METHOD_PROLOGUE(CCustomControlSite, ServiceProvider)
	return pThis->ExternalRelease();
}

HRESULT FAR EXPORT CCustomControlSite::XServiceProvider
										::QueryInterface(REFIID riid, 
														void** ppvObj)
{
	METHOD_PROLOGUE(CCustomControlSite, ServiceProvider)
    HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
	return hr;
}
STDMETHODIMP CCustomControlSite::XServiceProvider
								::QueryService(REFGUID guidService,  
												REFIID riid,
												void** ppvObject)
{

	if (guidService == SID_SInternetSecurityManager && 
					riid == IID_IInternetSecurityManager)
	{
		METHOD_PROLOGUE(CCustomControlSite, ServiceProvider)
		HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObject);
		return hr;
	} 
	else 
	{
		*ppvObject = NULL;

	}
	return E_NOINTERFACE;
}

