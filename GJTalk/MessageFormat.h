#pragma once
#include "../xmpp/gloox.h"
#include "../xmpp/stanzaextension.h"

namespace gloox
{

	class Tag;
	class MessageFormat : public StanzaExtension
	{
	private:
		CString m_font;
		int m_fontSize;
		int m_style;
		COLORREF m_color;
	public:
		MessageFormat(void); 
		MessageFormat (const gloox::Tag *tag);
		virtual ~MessageFormat(void);
		virtual const std::string& filterString() const;

		virtual StanzaExtension* newInstance( const Tag* tag ) const;

		virtual Tag* tag() const;

		virtual StanzaExtension* clone() const;

	};


}