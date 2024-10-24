//#define GAME_NO_CONSOLE    //关闭debug

#include "FloatEngine.h"
Game game;
class Block_Test :public Block {
public:
	void onEnter() {
		m_block.width = 50;
		m_block.height = 50;
		x = y = 0;
	}
	void onRender() {
		F_Rectangle t;
		t.width = m_block.width;
		t.height = m_block.height;
		t.x = x;
		t.y = y;
		t.angle = m_angle;
		F_Render::Draw_Rectangle(t,F_Color::Green);
	}
};
class O1 :public Actor {
public:
	float size;
	void onEnter() {
		m_block.width = 30;
		m_block.height = 30;
		size = 30;
		x = 320;
		y = 240;
	}
	void onUpdate() {
		
	}
	void onRender() {
		DrawCircle(x, y, size, F_Color::Red);
	}
};
class R1:public Room {
public:
	Object* o1;
	R1() {
		o1 = new O1;
		Create(o1);
	}
	~R1() {
	}
	Room* reset() {
		return new R1;
	}
};

int main() {
	game.CreateWindow(640, 480, "");
	if (game.CanStart()) {
		auto r1 = new R1;
		Room_Goto(r1);

		game.Play(60);
	}
	game.Destroy();
	return 0;
}

/*
#include <raylib.hpp>
int main() {
	InitWindow(640, 480, "test");
	SetTargetFPS(60);
	RenderTexture rt = LoadRenderTexture(640, 480);
	while (!WindowShouldClose()) {
		BeginDrawing(); {
			ClearBackground(BLACK);
			DrawTriangle({ 100,100 }, { 100,200 }, { 200,100 }, WHITE);
		}EndDrawing();
	}
	UnloadRenderTexture(rt);
}
*/