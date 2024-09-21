#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <raylib.hpp>
#include <iostream>
#include <vector>
#include "F_Resource.h"
#include "FMath.h"
#include "F_Audio.h"
#include "Windows.h"
#undef PlaySound
#undef DrawText
#undef CreateWindow
#define Rectangle raylib::Rectangle
class F_Debug {
	static int m_debug;
public:
	//初始化
	static void Init(bool debug= true);
	//开启Debug
	static void Start();
	//关闭Debug
	static void Close();
	//是否开启Debug
   	static bool IsOpen();
};
void DEBUG_LOG(int lv, const char* text, bool english = 1, bool auto_enter = 1);
#define repeat(n) for(int __r_i__=0;__r_i__<int(n);__r_i__++)
typedef float Float;
Color Make_Color_RGB(int r, int g, int b, float a = 1.0f);
/*数组中是否含有某数据*/
template<class T>
inline bool Array_Has(T a[], T b) {
	for (int i = 0; i < sizeof(a) / sizeof(T); i++) {
		if (a[i] == b) {
			return true;
		}
	}
	return false;
}
#define  ARR_SIZE( a )  ( sizeof( (a) ) / sizeof( (a[0]) ) )
static char* m_CodePoint;
void Set_CodePoint(const char* cp);
void Add_CodePoint(const char* t);
Font Load_Font(const char* filePath, int size);

class F_Color {
public:
	inline static Color LightGray = LIGHTGRAY;
	inline static Color Gray = GRAY;
	inline static Color DarkGray = DARKGRAY;
	inline static Color DrakGray = YELLOW;
	inline static Color Gold = GOLD;
	inline static Color Orange = ORANGE;
	inline static Color Pink = PINK;
	inline static Color Red = RED;
	inline static Color Maroon = MAROON;
	inline static Color Green = GREEN;
	inline static Color Lime = LIME;
	inline static Color DarkGreen = DARKGREEN;
	inline static Color SkyBlue = SKYBLUE;
	inline static Color Blue = BLUE;
	inline static Color DarkBlue = DARKBLUE;
	inline static Color Purple = PURPLE;
	inline static Color Violet = VIOLET;
	inline static Color Darkpurple = DARKPURPLE;
	inline static Color Beige = BEIGE;
	inline static Color Borwn = BROWN;
	inline static Color DarkBrown = DARKBROWN;
	inline static Color White = WHITE;
	inline static Color Black = BLACK;
	inline static Color Blank = BLANK;
	inline static Color Magenta = MAGENTA;
	inline static Color RayWhite = RAYWHITE;
	inline static Color Yellow = YELLOW;
	

};
//绘制
class F_Render {
public:
	/*图片输出*/
	/*
	 * [Texture*]img - 图片
	 * [int]ind - 帧
	 * [float]x(y) - 坐标
	 * [float]x(y)_scale - 缩放
	 * [float]x(y)_origin - 原点
	 * [float]angle - 角度
	*/
	static void Draw_Sprite_Ex(Texture* img, int ind, float x, float y, float x_scale, float y_scale, float x_origin, float y_origin, float angle, float alpha, Color col);
	static void Draw_Text_Ex(Font font, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float size, Color col, float alpha = 1.0f);
	static void Draw_Shape(F_Shape shape, Color col);
	static void Draw_Rectangle(F_Rectangle rect, Color col);
	static void Draw_Circle(F_Circle circle, Color col);
	static void Draw_Ellipse(F_Ellipse ellipse, Color col);
	static void Draw_Shape_Fill(F_Shape shape,Color col);
	static void Draw_Polygon(F_Polygon polygon, Color col);
	static void Draw_Polygon(std::vector<FVec2> points, Color color);
	static void Draw_Triangle(F_Triangle triangle, Color col);
	static void Draw_Line(F_Line line, Color col);

	//以下封装raylib的绘图函数
	static void Line(float x, float y);
};
//camera
class F_Camera {
	RenderTexture camera_render;
	bool auto_free;
	float width;
	float height;
public:
	Color col;
	float angle;
	float x_scale;
	float y_scale;
	float x;
	float y;
	Vector2 origin;

	F_Camera();
	F_Camera(const F_Camera* c);
	void Load(float w, float h,bool _auto_free=true);
	void Unload();
	float Get_Width() const;
	float Get_Height() const;
	RenderTexture Get() ;
	~F_Camera();
};
void Load_FCamera(float w, float h, F_Camera* camera);
void Unload_FCamera(F_Camera* camera);

void Begin_Mode_FCamera(F_Camera& _camera, Color Background_Color);
void End_Mode_FCamera(F_Camera& _camera, Color Background_Color, int reverse = 0);
void Begin_Mode_FCamera(F_Camera* _camera, Color Background_Color);
void End_Mode_FCamera(F_Camera* _camera, Color Background_Color, int reverse = 0);

/*声音播放(audio)*/
void Audio_Play(F_Audio audio);
void Audio_Stop(F_Audio audio);
void Audio_Music_Update(F_Audio audio);


//file
class F_File {
	FilePathList drop_list;
public:
	//选择文件(返回""为失败)
	static std::string Get_Open_File_Name(std::string strFilter);
	//选择文件(返回""为失败)
	static std::string Get_Open_File_Name(std::string strFilter, unsigned int flag);
	//
	void Flush_Drop_Files();
	const char* Get_Drop_File(int index);
};

