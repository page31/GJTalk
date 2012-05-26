#pragma once
#include "GJContext.h"
#include "GJWnd.h"
class CLoginFrame:public CGJWnd
{
private:

protected:
	virtual	void OnPostCreate();
public: 
	CLoginFrame(GJContext &context);
	~CLoginFrame(void); 

};

