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
 * Filename:    XMPP.cpp
 * Author(s):   Dries Staelens
 * Copyright:   Copyright (c) 2009 Dries Staelens
 * Description: Implementation of the XMPP class.
 */
#include "stdafx.h"
#include "XMPP.h"
//#include "MainWnd.h"

/*
 * Constructor
 */
XMPP::XMPP() :
	m_Handlers(),
	m_Logger(),
	m_ConnectionManager(m_Handlers, m_Logger)
{
}

/*
 * Destructor
 */
XMPP::~XMPP()
{
}

/*
 * Sets the proxy settings used for connecting to the XMPP server.
 */
STDMETHODIMP XMPP::SetProxySettings(
	LPTSTR method,
	LPTSTR server,
	USHORT port,
	LPTSTR username,
	LPTSTR password,
	bool useSSL)
{
	if(_wcsicmp(method, L"none") == 0)
	{
		m_ConnectionManager.SetProxySettings(PROXYMETHOD_NONE,
			L"", 0, L"", L"", false);
	}
	else if(_wcsicmp(method, L"connect") == 0)
	{
		m_ConnectionManager.SetProxySettings(PROXYMETHOD_CONNECT,
			server, port, username, password, useSSL);
	}
	else if(_wcsicmp(method, L"socks4") == 0)
	{
	}
	else if(_wcsicmp(method, L"socks5") == 0)
	{
	}
	else if(_wcsicmp(method, L"bosh") == 0)
	{
	}
	else
	{
		return E_NOTIMPL;
	}
	return S_OK;
}

/*
 * Attempts to connect to the given XMPP server.
 */
STDMETHODIMP XMPP::Connect(LPTSTR server, USHORT port, bool useSSL)
{
	m_ConnectionManager.Connect(server, port, useSSL);
	return S_OK;
}

/*
 * Disconnects from the server (when connected).
 */
STDMETHODIMP XMPP::Disconnect()
{
	m_ConnectionManager.Disconnect();
	return S_OK;
}

/*
 * Initiates TLS negotiation.
 */
STDMETHODIMP XMPP::StartTLS()
{
	m_ConnectionManager.StartTLS();
	return S_OK;
}

/*
 * Initiates Stream Compression.
 */
STDMETHODIMP XMPP::StartSC()
{
	m_ConnectionManager.StartSC();
	return S_OK;
}

/*
 * Sends XML to the XMPP server. The XML is stored in an MSXML IXMLDOMDocument
 * object.
 */
STDMETHODIMP XMPP::SendXML(IDispatch* pDisp)
{
	MSXML2::IXMLDOMDocument *pXMLDoc = NULL;
	LPTSTR xmlString;

	if(pDisp != NULL &&
		SUCCEEDED(pDisp->QueryInterface(IID_IXMLDOMDocument,
			(LPVOID*) &pXMLDoc)) &&
		SUCCEEDED(pXMLDoc->get_xml(&xmlString)))
	{
		m_ConnectionManager.SendText(xmlString);

		::SysFreeString(xmlString);
		pXMLDoc->Release();
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}

/*
 * Sends a string of text to the XMPP server.
 */
STDMETHODIMP XMPP::SendText(LPTSTR strText)
{
	
	m_ConnectionManager.SendText(std::wstring(strText));
	return S_OK;
}

/*
 * Returns the local IP used by the client to connect to the server.
 */
STDMETHODIMP XMPP::get_ConnectionIP(LPTSTR *strIP)
{
	*strIP = ::SysAllocString(m_ConnectionManager.GetConnectionIP().c_str());
	return S_OK;
}

/*
 * Returns the Fully Qualified Domain name of the XMPP server.
 */
STDMETHODIMP XMPP::get_ConnectionFQDN(LPTSTR *serverFQDN)
{
	*serverFQDN = ::SysAllocString(m_ConnectionManager.GetServerFQDN().c_str());
	return S_OK;
}

/*
 * Sets the handler for the event generated when connection is set up.
 */
STDMETHODIMP XMPP::put_ConnectedHandler(LPTSTR handler)
{
	m_Handlers.SetConnectedHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated when the connection has ended.
 */
STDMETHODIMP XMPP::put_DisconnectedHandler(LPTSTR handler)
{
	m_Handlers.SetDisconnectedHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated when an iq stanza is received.
 */
STDMETHODIMP XMPP::put_IQHandler(LPTSTR handler)
{
	
	m_Handlers.SetIQHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated when a message stanza is received.
 */
STDMETHODIMP XMPP::put_MessageHandler(LPTSTR handler)
{
	m_Handlers.SetMessageHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated when a presence
 * stanza is received.
 */
STDMETHODIMP XMPP::put_PresenceHandler(LPTSTR handler)
{
	m_Handlers.SetPresenceHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated the initial XML is received from
 * the server.
 */
STDMETHODIMP XMPP::put_DocumentStartHandler(LPTSTR handler)
{
	m_Handlers.SetDocumentStartHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated when </stream:stream> is
 * received from the server.
 */
STDMETHODIMP XMPP::put_DocumentEndHandler(LPTSTR handler)
{
	m_Handlers.SetDocumentEndHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated for XMPP stanzas with the stream
 * prefix.
 */
STDMETHODIMP XMPP::put_StreamHandler(LPTSTR handler)
{
	m_Handlers.SetStreamHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated if a TLS connection is
 * successfully negotiated.
 */
STDMETHODIMP XMPP::put_StartTLSSucceededHandler(LPTSTR handler)
{
	m_Handlers.SetStartTLSSucceededHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated if a TLS connection could not be
 * negotiated.
 */
STDMETHODIMP XMPP::put_StartTLSFailedHandler(LPTSTR handler)
{
	m_Handlers.SetStartTLSFailedHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated if Stream Compression was
 * successfully initiated.
 */
STDMETHODIMP XMPP::put_StartSCSucceededHandler(LPTSTR handler)
{
	m_Handlers.SetStartSCSucceededHandler(handler);
	return S_OK;
}

/*
 * Sets the handler for the event generated if Stream Compression initiation
 * has failed.
 */
STDMETHODIMP XMPP::put_StartSCFailedHandler(LPTSTR handler)
{
	m_Handlers.SetStartSCFailedHandler(handler);
	return S_OK;
}

STDMETHODIMP XMPP::get_ServerFQDN(LPTSTR *serverFQDN)
{
	*serverFQDN = ::SysAllocString(m_ConnectionManager.GetServerFQDN().c_str());
	return S_OK;
}

/*
 * Sets the receiver of the events generated by this class.
 */
//void XMPP::SetMainWnd(MainWnd* pMainWnd)
//{
//	m_Handlers.SetMainWindow((MainWnd*) pMainWnd);
//}
