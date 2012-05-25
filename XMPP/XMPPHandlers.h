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
 * Filename:    XMPPHandlers.h
 * Author(s):   Dries Staelens
 * Copyright:   Copyright (c) 2009 Dries Staelens
 * Description: TODOTODOTODO
 */
#pragma once

//class MainWnd;

class XMPPHandlers
{
	_bstr_t m_ConnectedHandler;
	_bstr_t m_DisconnectedHandler;
	_bstr_t m_IQHandler;
	_bstr_t m_MessageHandler;
	_bstr_t m_PresenceHandler;
	_bstr_t m_DocumentStartHandler;
	_bstr_t m_DocumentEndHandler;
	_bstr_t m_StreamHandler;
	_bstr_t m_StartTLSSucceededHandler;
	_bstr_t m_StartTLSFailedHandler;
	_bstr_t m_StartSCSucceededHandler;
	_bstr_t m_StartSCFailedHandler;

	//MainWnd* m_pMainWnd;
public:
	XMPPHandlers();
	~XMPPHandlers();

	//void SetMainWindow(MainWnd* mainWindow);

	void OnConnected();
	void OnDisconnected();
	void OnStanza(MSXML2::IXMLDOMDocument *pXMLDoc, const LPTSTR stanzaName);
	void OnDocumentStart(const _bstr_t& xmlFragment);
	void OnDocumentEnd(const _bstr_t& xmlFragment);
	void OnStartTLSSucceeded();
	void OnStartTLSFailed();
	void OnStartSCSucceeded();
	void OnStartSCFailed();

	void SetConnectedHandler(const LPTSTR handler);
	void SetDisconnectedHandler(const LPTSTR handler);
	void SetIQHandler(const LPTSTR handler);
	void SetMessageHandler(const LPTSTR handler);
	void SetPresenceHandler(const LPTSTR handler);
	void SetDocumentStartHandler(const LPTSTR handler);
	void SetDocumentEndHandler(const LPTSTR handler);
	void SetStreamHandler(const LPTSTR handler);
	void SetStartTLSSucceededHandler(const LPTSTR handler);
	void SetStartTLSFailedHandler(const LPTSTR handler);
	void SetStartSCSucceededHandler(const LPTSTR handler);
	void SetStartSCFailedHandler(const LPTSTR handler);

};