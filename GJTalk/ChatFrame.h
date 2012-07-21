#pragma once
#include "GJContextWnd.h"
#include "UIBuddyList.h"
#include "../xmpp/message.h"
#include "SessionManager.h" 
#include "../xmpp/vcardhandler.h"
#include "HeaderManager.h"
#include "../xmpp/presencehandler.h"
class CSessionMessage;
class CChatFrame :
	public CGJContextWnd,
	public gloox::VCardHandler,
	public  HeaderHandler,
	public gloox::PresenceHandler
{

private:

	VCard m_vcard;
	bool m_bVCardInited;
protected:
	
	JID m_jid;
	void OnPostCreate();

	CButtonUI *m_pBtnHeader;
	CLabelUI *m_pTitleLabel;
	CLabelUI *m_pSignatureLabel;
	CRichEditUI *m_pMsgRecord;
	CRichEditUI *m_pMsgEdit;
	CComboBoxUI *m_pFontList;
	CComboBoxUI *m_pFontSize;
	CButtonUI *m_pBtnSend;
	CButtonUI *m_pBtnClose;
public:
	void OnReceiveMessage(const CSessionMessage& msg);
	void AddToRecord(LPCTSTR pstrSender,LPCTSTR pstrMsg,LPCTSTR pstrFont,int fontSize,COLORREF color,time_t time,bool self=false);
	void SendChatMessage(LPCTSTR pstrContent,LPCTSTR pstrFont,int fontSize,COLORREF	color);
	void SendChatMessage();

	const JID &GetTarget() const;
	void Notify(TNotifyUI& msg);
	CChatFrame(GJContext *context);
	void InitTarget(const JID& buddy);
	~CChatFrame(void);
	void OnVCardUpdated();

	virtual void handleVCard( const JID& jid, const VCard* vcard );

	virtual void handleVCardResult( VCardContext context, const JID& jid, StanzaError se = StanzaErrorUndefined );

	virtual void HandleHeaderUpdate( const CHeaderManager &manager,const gloox::JID& jid );

	virtual void handlePresence( const Presence& presence );

};

