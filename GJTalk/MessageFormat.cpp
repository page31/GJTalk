#include "stdafx.h"
#include "MessageFormat.h"


MessageFormat::MessageFormat(void)
	:StanzaExtension(0)
{

}

MessageFormat::MessageFormat( const Tag *tag )
	:StanzaExtension(0)
{

}


MessageFormat::~MessageFormat(void)
{
}

const std::string& MessageFormat::filterString() const
{
	throw std::exception("The method or operation is not implemented.");
}

StanzaExtension* MessageFormat::newInstance( const Tag* tag ) const
{
	throw std::exception("The method or operation is not implemented.");
}

Tag* MessageFormat::tag() const
{
	throw std::exception("The method or operation is not implemented.");
}

StanzaExtension* MessageFormat::clone() const
{
	throw std::exception("The method or operation is not implemented.");
}
