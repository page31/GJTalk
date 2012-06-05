#pragma once
#include "gjwnd.h"
#include "GJContext.h"
class GJContext;
class CGJContextWnd :
	public CGJWnd
{ 
protected:
	GJContext* m_pContext;
public:
	void SetContext(GJContext* context);
	GJContext* GetContext() const;
	CGJContextWnd(void);
	CGJContextWnd(GJContext* context);
	virtual void OnPostCreate()=0;
	~CGJContextWnd(void);
};

