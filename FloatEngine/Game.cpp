#include "Game.h"
#include "Room.h"
#include <string>
#include "F_Console.h"
Game::Game()
{
	_fps = _w = _h = 0;
	TextCopy(_title, "");
	background_col = BLACK;
}

void Game::CreateWindow(int w, int h, const char* title,int flags)
{
	F_Debug::Init(0);
	SetConfigFlags(flags);
	Init_FConsole();
	InitWindow(w, h, "Create With FloatEngine");
	if(!TextIsEqual(title,"")) SetWindowTextA((HWND)GetWindowHandle(), title);
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


void Game::Set_Background_Color(Color bk_col)
{
	background_col = bk_col;
}

