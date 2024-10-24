#include "Game.h"
#include "Room.h"
#include <string>
#include "F_Console.h"
Game::Game()
{
	_fps = _w = _h = 0;
	TextCopy(_title, "");
	background_col = BLACK;
	_load_icon = false;
	_icon_number = 0;
	memset(_icons, 0, sizeof _icons);
}
void Game::CreateWindow(int w, int h, const char* title, bool debug,int flags)
{
	F_Debug::Init(debug);
	using namespace WinFuns;
	SetConfigFlags(flags);
	Init_FConsole();
	InitWindow(w, h, "Create With FloatEngine");
	if (_load_icon) {
		SetWindowIcons(_icons, _icon_number);
	}
	if(!TextIsEqual(title,"")) SetWindowTextA(GetWindowHandle(), title);
	else {
		if (F_Debug::IsOpen()) {
			SetWindowTextA(GetWindowHandle(), (std::string("[调试模式(Debug Mode)]") + std::string(title)).c_str());
		}
	}
	BeginDrawing(); EndDrawing();
	InitAudioDevice();
	SetRandomSeed(time(0));
	_w = w; _h = h;
	TextCopy(_title, title);
	_fps = 60;
	SetTargetFPS(60);
}
bool Game::CanStart()
{
	return (IsWindowReady() && IsAudioDeviceReady());
}

void Game::Play(int fps)
{
	if (fps != _fps) {
		_fps = fps;
		SetTargetFPS(_fps);
	}
	while (!WindowShouldClose()) {
		//event
		Room_Run_Now("onEnter");
		//step
		Room_Run_Now("onUpdate");
		//draw
		Room_Run_Now("onRenderBefore");
		BeginDrawing(); {
			ClearBackground(background_col);
			Room_Run_Now("onRender");
		}EndDrawing();
		Room_Run_Now("onRenderNext");
	}
}

void Game::Destroy()
{
	_fps = _w = _h = 0;
	Room* t = Room_Get_Now();
	delete t;
	delete _title;
	CloseAudioDevice();
	CloseWindow();
}


void Game::Set_Background_Color(raylib::Color bk_col)
{
	background_col = bk_col;
}

void Game::Set_Console_Icon(int id)
{
	using namespace WinFuns;
	// 获取当前控制台窗口的句柄
	void* hwnd =GetConsoleWindow();

	// 从资源文件中加载图标（使用资源标识符 IDI_MY_ICON）
	void* hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(id));

	if (hIcon) {
		// 发送消息设置图标
		SendMessage(hwnd, 0x0080,1, (LPARAM)hIcon); // 设置小图标
		SendMessage(hwnd, 0x0080, 0, (LPARAM)hIcon);   // 设置大图标
	}
}

void Game::Set_Icons(Image* icons, int n)
{
	SetWindowIcons(icons, n);
	memset(_icons, 0, sizeof _icons);
	for (int i = 0; i < n; i++) {
		_icons[i] = icons[i];
	}
}

void Game::Set_Icon(Image icon)
{
	SetWindowIcon(icon);
	memset(_icons, 0, sizeof _icons);
	_icons[0] = icon;
}


