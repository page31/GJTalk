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
 * Filename:    XMPPConnectionManager.h
 * Author(s):   Dries Staelens
 * Copyright:   Copyright (c) 2009 Dries Staelens
 * Description: TODOTODOTODO
 */

#pragma once
#include "XMPPSocket.h"
#include "XMPPSendQueue.h"
#include "XMPPXMLParser.h"
#include "SRVLookup.h"

enum ProxyMethod {
	PROXYMETHOD_NONE,
	PROXYMETHOD_CONNECT,
	PROXYMETHOD_SOCKS4,
	PROXYMETHOD_SOCKS5,
	PROXYMETHOD_BOSH
};

class XMPPConnectionManager
{
private:
	XMPPSocket			m_Socket;
	XMPPSendQueue	m_SendQueue;
	XMPPXMLParser	m_XMLParser;
	XMPPHandlers&	m_Handlers;
	XMPPLogger&		m_Logger;

	ProxyMethod     m_ProxyMethod;
	std::wstring    m_ProxyServer;
	unsigned short  m_ProxyPort;
	std::wstring    m_ProxyUsername;
	std::wstring    m_ProxyPassword;
	bool            m_ProxyUseSSL;

	std::wstring	m_ServerName;
	unsigned short	m_Port;
	bool			m_UseSSL;

	bool			m_DoStartTLS;
	bool			m_DoStartSC;
	bool			m_DoDisconnect;

	HANDLE          m_CanStartNewThread;

	std::vector<BYTE> m_DataBuffer;
public:
	XMPPConnectionManager(XMPPHandlers& handlers, XMPPLogger& logger);
	~XMPPConnectionManager();

	void SetProxySettings(ProxyMethod method,
		std::wstring server, unsigned short port,
		std::wstring username, std::wstring password,
		bool useSSL);

	void Connect(const std::wstring& server,
		unsigned short port, bool useSSL);
	void Disconnect();
	void StartTLS();
	void StartSC();
	void SendText(const std::wstring& utf16Text);

	std::wstring GetConnectionIP();
	std::wstring GetServerFQDN();
private:
	static DWORD __stdcall ConnectionMainProc(void *pThis);
	DWORD ConnectionMain();
	bool DoConnect();
	bool DoConnectWithSRV(SRVLookup& srvLookup);
	bool DoConnectWithoutSRV();
	bool DoRecvData();
	void DoStartTLS();
	void DoStartSC();

	void AsyncSend(BYTE *data, DWORD len);
	
	unsigned PopNextCharFromBuffer();
};
