#pragma once
#include "FloatApi.h"
#include "F_Console.h"
class Game
{
	int _fps;
	int _w;
	int _h;
	char* _title;
	
	bool _load_icon;
	int _icon_number;
	Image _icons[100];
	raylib::Color background_col;
public:
	Game();
	void CreateWindow(int w,int h,const char*title,  bool debug = true, int flags = 288);
	bool CanStart();
	void Play(int fps);
	void Destroy();
	void Set_Background_Color(raylib::Color bk_col);
	void Set_Console_Icon(int id);
	void Set_Icons(Image* icons,int n);
	void Set_Icon(Image icon);
};

