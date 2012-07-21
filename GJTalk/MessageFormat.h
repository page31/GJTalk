#pragma once
#include "../xmpp/gloox.h"
#include "../xmpp/stanzaextension.h"

namespace gloox
{

	class Tag;
	class MessageFormat : public StanzaExtension
	{

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