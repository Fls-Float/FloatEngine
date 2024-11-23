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
	_icon_path = FilePathList();
	_load_icon_pre = 0;
	_icon_path.capacity = 20;
}
void Game::CreateWindow(int w, int h, const char* title, bool debug, int flags)
{
	F_Debug::Init(debug);
	using namespace WinFuns;
	SetConfigFlags(flags);
	Init_FConsole();
	InitWindow(w, h, "Create With FloatEngine");
	PollInputEvents();
	if (_load_icon) {
		SetWindowIcons(_icons, _icon_number);
	}
	if (_load_icon_pre) {
		if (_icon_path.paths == nullptr) {
			DEBUG_LOG(LOG_WARNING, "_icon_path为空!", 0);
		}
		else if (_icon_path.count > 1) {
			Image* icons = new Image[_icon_path.count + 1];
			for (int i = 0; i < _icon_path.count; i++) {
				icons[i] = ::LoadImage(_icon_path.paths[i]);
			}
			::SetWindowIcons(icons, _icon_path.count); 
			for (int i = 0; i < _icon_path.count; i++) {
				UnloadImage(icons[i]);
			}
			delete[]icons;
		}
		else if (_icon_path.count == 1 ) {
			Image icon = ::LoadImage(_icon_path.paths[0]);
			::SetWindowIcon(icon);
			::UnloadImage(icon);
		}
	}
	if (!TextIsEqual(title, "")) {
		if (F_Debug::IsOpen()) {
			SetWindowTitle(TextFormat("%s%s", title,"  --   (DEBUG)"));
		}
		else {
			SetWindowTitle(title);
		}
	}
	InitAudioDevice();
	_w = w; _h = h;
	TextCopy(_title, title);
	PollInputEvents();
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
	HWND hwnd =(HWND)GetConsoleWindow();

	// 从资源文件中加载图标（使用资源标识符 IDI_MY_ICON）
	void* hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(id));

	if (hIcon) {
		// 发送消息设置图标
		SendMessage(hwnd, 0x0080,1, (LPARAM)hIcon); // 设置小图标
		SendMessage(hwnd, 0x0080, 0, (LPARAM)hIcon);   // 设置大图标
	}
}

void Game::Set_Title(const char* title)
{
	TextCopy(_title, title);
	SetWindowTitle(title);
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

void Game::Set_Icon_Pre(const char* filePath) {
	if (_icon_path.count >= _icon_path.capacity) {
		if (_icon_path.capacity == 0) {
			_icon_path.capacity = 1;
		}
		else {
			_icon_path.capacity *= 2;
		}
		char** newPaths = new char* [_icon_path.capacity];
		for (unsigned int i = 0; i < _icon_path.count; i++) {
			newPaths[i] = _icon_path.paths[i];
		}
		delete[] _icon_path.paths;
		_icon_path.paths = newPaths;
	}
	size_t pathLen = strlen(filePath) + 1; 
	_icon_path.paths[_icon_path.count] = new char[pathLen];
	strcpy(_icon_path.paths[_icon_path.count], filePath);  
	_icon_path.count++;
	_load_icon_pre = 1;
}

void Game::Set_Icons_Pre(FilePathList filePathList) {
	// 确保传入的文件路径列表有效
	if (filePathList.paths == nullptr || filePathList.count == 0) {
		DEBUG_LOG(LOG_ERROR,"Set_Icons_Pre: 文件路径列表为空。",0);
		return;
	}
	for (unsigned int i = 0; i < filePathList.count; i++) {
		const char* filePath = filePathList.paths[i];
		if (filePath == nullptr) continue; 
		if (_icon_path.count >= _icon_path.capacity) {
			if (_icon_path.capacity == 0) {
				_icon_path.capacity = 1;  
			}
			else {
				_icon_path.capacity *= 2; 
			}
			char** newPaths = new char* [_icon_path.capacity];
			for (unsigned int j = 0; j < _icon_path.count; j++) {
				newPaths[j] = _icon_path.paths[j];
			}
			delete[] _icon_path.paths;
			_icon_path.paths = newPaths;
		}
		size_t pathLen = strlen(filePath) + 1; 
		_icon_path.paths[_icon_path.count] = new char[pathLen];
		strcpy(_icon_path.paths[_icon_path.count], filePath); 
		_icon_path.count++;
	}
	_load_icon_pre = 1;
}


void Game::SetFPS(int fps) {
	::SetTargetFPS(fps);
	_fps = fps;
}
int Game::GetFPS()const {
	return _fps;
}
void Game::Set_Window_Pos(int x,int y) {
	if (x == -1 && y == -1)
	{
		return;
	}
	if (x == -1) {
		x = ::GetWindowPosition().x;
	}
	if (y == -1) {
		y = ::GetWindowPosition().y;
	}
	::SetWindowPosition(x, y);
}

void Game::Set_Window_Opacity(float op) {
	::SetWindowOpacity(op);
}