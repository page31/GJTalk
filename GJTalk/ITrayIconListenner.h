#pragma once

#include "TrayIcon.h"

class ITrayIconListener
{
public:
	virtual void OnTrayIconMessage(CTrayIconMessage& msg)=0;
};