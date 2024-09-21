#pragma once
#include <raylib.hpp>
#include "winfuns.h"
#include "FloatApi.h"
#include "F_Console.h"
class Game
{
	int _fps;
	int _w;
	int _h;
	char* _title;
	
	
	
	Color background_col;
public:
	Game();
	void CreateWindow(int w,int h,const char*title, int flags = FLAG_WINDOW_ALWAYS_RUN | FLAG_MSAA_4X_HINT);
	bool CanStart();
	void Play(int fps);
	void Destroy();
	void Set_Background_Color(Color bk_col);
};

