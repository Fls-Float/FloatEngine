#pragma once
#include <raylib.hpp>
class F_Text
{
	char* m_text;
public:
	F_Text operator=(const char* text);
	F_Text operator=(const F_Text& text);
	bool operator==(const char* text);
   	bool operator==(const F_Text& text);
	operator const char*();
	F_Text operator+(const F_Text& text);
	F_Text operator+(const char* text);
	void operator+=(const char* text);
   	void operator+=(const F_Text& text);
	int Length();
	F_Text() {
		m_text = new char[1];
	}
	F_Text(const char* text);
	~F_Text() {
		delete m_text;
	}
};

