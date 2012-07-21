#pragma once

#include <gloox.h>
#include <stanzaextension.h>

namespace gloox
{
	class Tag;
	class GLOOX_API MessageFormat : public  StanzaExtension
	{
	public:
		MessageFormat(void);
		MessageFormat(const Tag *tag=0);
		virtual ~MessageFormat(void);
		virtual Tag *tag() const;
	};

}

