#pragma once
#include "GJTalk.h"
#include "GJUser.h"
class GJContext
{
private:
	GJUser self; 
	bool mIsSignedIn;
public:
	GJContext(void);
	 
	bool IsSignedIn();
	GJUser *GetSelf();
	~GJContext(void);
};

