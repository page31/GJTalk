
//=--------------------------------------------------------------------------=
//  (C) Copyright 1996-1999 Microsoft Corporation. All Rights Reserved.
//=--------------------------------------------------------------------------=
#ifndef __CUSTOMSITEH__
#define __CUSTOMSITEH__

#include <mshtmhst.h>
#include <afxocc.h>
// 
// NOTE: 
// Some of the code in this file is MFC implementation specific.
// Changes in future versions of MFC implementation may require
// the code to be changed. Please check the readme of this
// sample for more information 
// 

class CCustomControlSite: public COleControlSite
{
public:
	CCustomControlSite(COleControlContainer *pCnt):COleControlSite(pCnt){}

protected:

	DECLARE_INTERFACE_MAP();


////////////////////////////////
//// Implement IDocHostUIHandler
BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)
	STDMETHOD(ShowContextMenu)(/* [in] */ DWORD dwID,
            /* [in] */ POINT __RPC_FAR *ppt,
            /* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
            /* [in] */ IDispatch __RPC_FAR *pdispReserved);
	STDMETHOD(GetHostInfo)( 
            /* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
	STDMETHOD(ShowUI)( 
            /* [in] */ DWORD dwID,
            /* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
            /* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
            /* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
            /* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc);
	STDMETHOD(HideUI)(void);
	STDMETHOD(UpdateUI)(void);
	STDMETHOD(EnableModeless)(/* [in] */ BOOL fEnable);
	STDMETHOD(OnDocWindowActivate)(/* [in] */ BOOL fEnable);
	STDMETHOD(OnFrameWindowActivate)(/* [in] */ BOOL fEnable);
	STDMETHOD(ResizeBorder)( 
            /* [in] */ LPCRECT prcBorder,
            /* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
            /* [in] */ BOOL fRameWindow);
	STDMETHOD(TranslateAccelerator)( 
            /* [in] */ LPMSG lpMsg,
            /* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
            /* [in] */ DWORD nCmdID);
	STDMETHOD(GetOptionKeyPath)( 
            /* [out] */ LPOLESTR __RPC_FAR *pchKey,
            /* [in] */ DWORD dw);
	STDMETHOD(GetDropTarget)(
            /* [in] */ IDropTarget __RPC_FAR *pDropTarget,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
    STDMETHOD(GetExternal)( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
    STDMETHOD(TranslateUrl)( 
            /* [in] */ DWORD dwTranslate,
            /* [in] */ OLECHAR __RPC_FAR *pchURLIn,
            /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
    STDMETHOD(FilterDataObject)( 
            /* [in] */ IDataObject __RPC_FAR *pDO,
            /* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet);
END_INTERFACE_PART(DocHostUIHandler)

///////////////////////////////////////
//// Implement IInternetSecurityManager
BEGIN_INTERFACE_PART(InternetSecurityManager, IInternetSecurityManager)
		STDMETHOD(SetSecuritySite)(IInternetSecurityMgrSite*);
		STDMETHOD(GetSecuritySite)(IInternetSecurityMgrSite**);
		STDMETHOD(MapUrlToZone)(LPCWSTR,DWORD*,DWORD);
		STDMETHOD(GetSecurityId)(LPCWSTR,BYTE*,DWORD*,DWORD);
		STDMETHOD(ProcessUrlAction)(
            /* [in] */ LPCWSTR pwszUrl,
            /* [in] */ DWORD dwAction,
            /* [size_is][out] */ BYTE __RPC_FAR *pPolicy,
            /* [in] */ DWORD cbPolicy,
            /* [in] */ BYTE __RPC_FAR *pContext,
            /* [in] */ DWORD cbContext,
            /* [in] */ DWORD dwFlags,
            /* [in] */ DWORD dwReserved = 0);
		STDMETHOD(QueryCustomPolicy)(LPCWSTR,REFGUID,BYTE**,DWORD*,BYTE*,DWORD,DWORD);
		STDMETHOD(SetZoneMapping)(DWORD,LPCWSTR,DWORD);
		STDMETHOD(GetZoneMappings)(DWORD,IEnumString**,DWORD);
END_INTERFACE_PART(InternetSecurityManager)


///////////////////////////////////////
//// Implement IServiceProvider
BEGIN_INTERFACE_PART(ServiceProvider, IServiceProvider)
		STDMETHOD(QueryService)(REFGUID,REFIID,void**);
END_INTERFACE_PART(ServiceProvider)



};


class CCustomOccManager :public COccManager
{
public:
	CCustomOccManager(){}
	virtual ~CCustomOccManager(){}
	COleControlSite* CreateSite(COleControlContainer* pCtrlCont)
	{
		CCustomControlSite *pSite = new CCustomControlSite(pCtrlCont);
		return pSite;
	}
};


#endif