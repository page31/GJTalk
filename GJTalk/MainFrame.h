#pragma once
#include <string>
#include "GJWnd.h"
using namespace std;
class CMainFrame:public CGJWnd
{
private: 
public:
	CMainFrame(GJContext &context);
	~CMainFrame(void);
};
