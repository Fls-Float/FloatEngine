//#define GAME_NO_CONSOLE    //关闭debug

#include "FloatEngine.h"
Game game;

class Block_Test :public Block {
public:
	void onEnter() {
		m_block.width = 50;
		m_block.height = 50;
		m_block.Set_Rot_Origin({ 0,0 });
		x = y = 0;
	}
	void onRender() {
		Block_Chase_Pos(0, 0);
		Draw_Block(F_Color::Green);
	}
};
class O1 :public Actor {
public:
	float size;
	void onEnter() {
		m_block.width = 30;
		m_block.height = 30;
		m_block.rot_origin = { 0.5,0.5 };
		size = 30;
		x = 320;
		y = 240;
		auto i = Create_InstanceTemplate(420, 240, Block_Test(), "Block_test");
		Add_New_Block(i);
	}
	void onUpdate() {
		bool up = F_Input::keyboard_down(F_Input::key_up);
		bool down = F_Input::keyboard_down(F_Input::key_down);
		bool left = F_Input::keyboard_down(F_Input::key_left);
		bool right = F_Input::keyboard_down(F_Input::key_right);
		float move_x=0, move_y=0;
		if (up) move_y -= 2.5;
		if (down) move_y += 2.5;
		if (left) move_x -= 2.5;
		if (right) move_x += 2.5;
		Move(move_x, move_y);
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
	game.CreateWindow(640, 480, "Actor测试");
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