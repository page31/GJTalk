#pragma once
#include "gjcontextwnd.h"
class CAcceptSubscriptionFrame :
	public CGJContextWnd
{
private:
	vector<CString> m_groups;
	CComboBoxUI *m_pCBGroups;
	CEditUI *m_pEditRemark;
	CEditUI *m_pEditMsg;
	CButtonUI *m_pBtnAddGroup;

	CString m_strRemark;
	CString m_strGroup;
public:
	CAcceptSubscriptionFrame(GJContext *context);
	~CAcceptSubscriptionFrame(void);

	virtual void OnPostCreate();

	virtual void Notify( TNotifyUI& msg );


	void AddGroupItem(LPCTSTR pstrName);

	CString Remark() const;
	CString GroupName() const;

};

