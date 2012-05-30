#pragma once
#include <string>
#include "GJContextWnd.h"
using namespace std;
class CMainFrame:public CGJContextWnd
{
private: 
protected:
	void OnPostCreate();
public:
	CMainFrame(GJContext *context);
	~CMainFrame(void);
};

