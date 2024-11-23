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
	bool _load_icon_pre;
	int _icon_number;
	Image _icons[100];
	FilePathList _icon_path;
	raylib::Color background_col;
public:
	Game();
	//FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_HIDDEN | FLAG_WINDOW_ALWAYS_RUN
	void CreateWindow(int w,int h,const char*title,  bool debug = true, int flags = 8608);
	bool CanStart();
	void Play(int fps);
	void SetFPS(int fps);
	int GetFPS() const;
	//设置窗口坐标（-1代表不变）
	void Set_Window_Pos(int x = -1, int y = -1);
	void Set_Window_Opacity(float op);
	void Destroy();
	void Set_Background_Color(raylib::Color bk_col);
	void Set_Console_Icon(int id);
	void Set_Title(const char* title);
	void Set_Icons(Image* icons,int n);
	void Set_Icon(Image icon);
	void Set_Icon_Pre(const char* filePath);
	void Set_Icons_Pre(FilePathList filePathList);
};

