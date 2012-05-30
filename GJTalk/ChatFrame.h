#pragma once
#include "GJContextWnd.h"
class CChatFrame :
	public CGJContextWnd
{

protected:
	void OnPostCreate();
public:
	CChatFrame(GJContext *context);
	~CChatFrame(void);
};

