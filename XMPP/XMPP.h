/**
 * This file is part of Pandion.
 *
 * Pandion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pandion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pandion.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Filename:    XMPP.h
 * Author(s):   Dries Staelens
 * Copyright:   Copyright (c) 2009 Dries Staelens
 * Description: Declaration of the XMPP class.
 */
#pragma once

#include "XMPPLogger.h"
#include "XMPPHandlers.h"
#include "XMPPConnectionManager.h"
 
/*
 * The XMPP class implements the (COM) interface to the XMPP functionality. 
 */
class AFX_CLASS_EXPORT XMPP 
{
private:
	/*
	 * The XMPP event handler.
	 */
	XMPPHandlers			m_Handlers;
	/*
	 * The XMPP event logger.
	 */
	XMPPLogger				m_Logger;
	/*
	 * The XMPP connection manager.
	 */
	XMPPConnectionManager	m_ConnectionManager;
public:
	XMPP();
	~XMPP();

	/* IXMPP implementation */
	STDMETHOD(SetProxySettings)(LPTSTR method,
		LPTSTR server, USHORT port,
		LPTSTR username, LPTSTR password,
		bool useSSL = false);

	STDMETHOD(Connect)(LPTSTR server, USHORT port, bool useSSL = false);
	STDMETHOD(Disconnect)();
	STDMETHOD(StartTLS)();
	STDMETHOD(StartSC)();

	STDMETHOD(SendXML)(IDispatch* pDisp);
	STDMETHOD(SendText)(LPTSTR strText);

	STDMETHOD(get_ConnectionIP)(LPTSTR *strIP);
	STDMETHOD(get_ConnectionFQDN)(LPTSTR *fqdn);

	STDMETHOD(put_ConnectedHandler)(LPTSTR handler);
	STDMETHOD(put_DisconnectedHandler)(LPTSTR handler);
	STDMETHOD(put_IQHandler)(LPTSTR handler);
	STDMETHOD(put_MessageHandler)(LPTSTR handler);
	STDMETHOD(put_PresenceHandler)(LPTSTR handler);
	STDMETHOD(put_DocumentStartHandler)(LPTSTR handler);
	STDMETHOD(put_DocumentEndHandler)(LPTSTR handler);
	STDMETHOD(put_StreamHandler)(LPTSTR handler);
	STDMETHOD(put_StartTLSSucceededHandler)(LPTSTR handler);
	STDMETHOD(put_StartTLSFailedHandler)(LPTSTR handler);
	STDMETHOD(put_StartSCSucceededHandler)(LPTSTR handler);
	STDMETHOD(put_StartSCFailedHandler)(LPTSTR handler);

	STDMETHOD(get_ServerFQDN)(LPTSTR *serverFQDN);

	/* Other methods */
	//virtual void SetMainWnd(MainWnd* pMainWnd);
};
