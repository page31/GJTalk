#include "stdafx.h"
#include "MessageFormat.h"


MessageFormat::MessageFormat(void)
	:StanzaExtension(0)
{
	m_style=0;
	m_fontSize=12;
}

MessageFormat::MessageFormat( const Tag *tag )
	:StanzaExtension(0)
{
	string size;
	string style;

	m_font=utf8dec(tag->findAttribute("font"));
	size=tag->findAttribute("size");
	style=tag->findAttribute("style");

	m_fontSize=atoi(size.c_str());
	m_style=atoi(style.c_str());
}


MessageFormat::~MessageFormat(void)
{
}

const std::string& MessageFormat::filterString() const
{
	static const string filter
		="/message/format";
	return filter;
}

StanzaExtension* MessageFormat::newInstance( const Tag* tag ) const
{
	return new MessageFormat(tag);

}

Tag* MessageFormat::tag() const
{
	Tag *tag=new Tag("Format");
	tag->addAttribute("font",utf8enc(m_font));
	tag->addAttribute("size",m_fontSize);
	tag->addAttribute("color",(long) m_color);
	tag->addAttribute("style",m_style);
	return tag;
}

StanzaExtension* MessageFormat::clone() const
{
	return new MessageFormat(*this);
}
