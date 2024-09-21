//#define GAME_NO_CONSOLE    //关闭debug

#include "FloatEngine.h"
Game game;
class O1 :public Object {
public:
	F_Rectangle rec,rec2;
	F_Circle circle;
	F_Ellipse ellipse;
	F_Triangle tri;
	void onEnter() {
		rec.Set_Rectangle(Rectangle{ 320,240,100,100 });
		rec2.Set_Rectangle(Rectangle{ 420,260,50,50 });
		rec.Shape_Set();
		rec2.Shape_Set();
		rec.Set_Origin({ 0.5,0.5 });
		rec2.Set_Origin({ 0.5,0.5 });
		circle.Set_Pos(320, 240);
		circle.radius = 30;
		ellipse.Set_Ellipse({ 200,100 }, 100, 50);
		circle.Shape_Set();
		ellipse.Shape_Set();
		tri.Set_Triangle({ 100,100 },{200,200},{150,130});
		tri.rot_origin = { 0.5,0.5 };
	}
	void onUpdate() {
		rec.x = GetMouseX();
		rec.y = GetMouseY();
		rec.angle+=0.1;
		rec2.angle += 0.1;
		rec.Shape_Set();
		rec2.Shape_Set();
		ellipse.angle += 0.1;
		ellipse.Shape_Set();
		tri.angle += 0.1;
		tri.Shape_Set();
	}
	void onRender() {
		F_Render::Draw_Rectangle(rec, Make_Color_RGB(50,50,50));
		F_Render::Draw_Rectangle(rec2, Make_Color_RGB(50, 50, 50));
		F_Render::Draw_Shape(rec, F_Color::Red);
		F_Render::Draw_Shape(rec2, F_Color::Blue);
		F_Render::Draw_Shape(circle, F_Color::Green);
		F_Render::Draw_Shape(ellipse, F_Color::Yellow);
		F_Render::Draw_Shape(tri, F_Color::Orange);
		DrawText(TextFormat("Is Collision1:%d",GJK_Collision_Plus(rec,rec2)),10, 10, 20, F_Color::White);
		DrawText(TextFormat("Is Collision2:%d", GJK_Collision_Plus(rec, circle)), 10, 70, 20, F_Color::White);
		DrawText(TextFormat("Is Collision3:%d", GJK_Collision_Plus(rec, ellipse)), 10, 90, 20, F_Color::White);
		DrawText(TextFormat("Is Collision4:%d", GJK_Collision_Plus(rec, tri)), 10, 110, 20, F_Color::White);

		DrawText(TextFormat("X1=%.1f,Y1=%.1f",rec.x, rec.y), 10, 30, 20, F_Color::White);
		DrawText(TextFormat("X2=%.1f,Y2=%.1f", rec2.x, rec2.y), 10, 50,20, F_Color::White);
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