#pragma once
#include "gjwnd.h"
class CChatFrame :
	public CGJWnd
{

protected:
	void OnPostCreate();
public:
	CChatFrame(GJContext &context);
	~CChatFrame(void);
};

