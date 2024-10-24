#include "F_Text.h"
#include <iostream>
F_Text F_Text::operator=(const char* text)
{
	TextCopy(m_text, text);
    return *this;
}

F_Text F_Text::operator=(const F_Text& text)
{
	TextCopy(m_text, text.m_text);
	return *this;
}

bool F_Text::operator==(const char* text)
{
	return TextIsEqual(m_text, text);
}

bool F_Text::operator==(const F_Text& text)
{
	return TextIsEqual(m_text, text.m_text);
}

F_Text::operator const char* ()
{
	return m_text;
}

const char* F_Text::cstr()
{
	return m_text;
}

std::string F_Text::tostr()
{
	return std::string(m_text);
}

void F_Text::operator+=(const F_Text& text)
{
	TextInsert(m_text, text.m_text, TextLength(m_text));
}

void F_Text::operator+=(const char* text)
{
   	TextInsert(m_text, text, TextLength(m_text));
}

F_Text F_Text::operator+(const char* text)
{
	F_Text t = (std::string(m_text)+std::string(text)).c_str();
	return t;
}

F_Text F_Text::operator+(const F_Text& text)
{
}

int F_Text::Length()
{
	return TextLength(m_text);
}

F_Text::F_Text(const char* text)
{
	TextCopy(m_text, text);
}
