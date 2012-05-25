#include "stdafx.h"
#include "GJContext.h"


GJContext::GJContext(void)
{

}

GJUser  *GJContext::GetSelf()
{
	if(!IsSignedIn())
		return NULL;
	return &self;
}
bool GJContext::IsSignedIn()
{
	return mIsSignedIn;
}

GJContext::~GJContext(void)
{
}
