#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define LINK_WINFUNS
#include <stdarg.h>
#include <raylib.hpp>
#include <iostream>
#include <vector>
#include "F_Resource.h"
#include "FMath.h"
#include "F_Audio.h"
#include "winfuns.h"


#define CloseWindow raylib::CloseWindow
#define Rectangle raylib::Rectangle
using namespace raylib;

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
Font Load_Font(const char* filePath, int size,const char*m_CodePoint);
Font Load_Font_File_Codepoints(const char* filePath, int size, const char* codePoints_File);
class F_Input {
public:
	static bool mb_down(int mb);
	static bool mb_pressed(int mb);
	static bool mb_released(int mb);
	static bool mb_up(int mb);
	static bool keyboard_pressed(int key);
	static bool keyboard_down(int key);
	static bool keyboard_released(int key);
	static bool keyboard_pressed_repeat(int key);
	//字母转换为键值
	static int to_value(char letter);
	//数字转换为键值
	static int number_to_value(int number);
	static const int key_null;
	static const int key_apostrophe;
	static const int key_comma;
	static const int key_minus;
	static const int key_period;
	static const int key_slash;
	static const int key_zero;
	static const int key_one;
	static const int key_two;
	static const int key_three;
	static const int key_four;
	static const int key_five;
	static const int key_six;
	static const int key_seven;
	static const int key_eight;
	static const int key_nine;
	static const int key_semicolon;
	static const int key_equal;
	static const int key_a;
	static const int key_b;
	static const int key_c;
	static const int key_d;
	static const int key_e;
	static const int key_f;
	static const int key_g;
	static const int key_h;
	static const int key_i;
	static const int key_j;
	static const int key_k;
	static const int key_l;
	static const int key_m;
	static const int key_n;
	static const int key_o;
	static const int key_p;
	static const int key_q;
	static const int key_r;
	static const int key_s;
	static const int key_t;
	static const int key_u;
	static const int key_v;
	static const int key_w;
	static const int key_x;
	static const int key_y;
	static const int key_z;
	static const int key_left_bracket;
	static const int key_backslash;
	static const int key_right_bracket;
	static const int key_grave;
	static const int key_space;
	static const int key_escape;
	static const int key_enter;
	static const int key_tab;
	static const int key_backspace;
	static const int key_insert;
	static const int key_delete;
	static const int key_right;
	static const int key_left;
	static const int key_down;
	static const int key_up;
	static const int key_page_up;
	static const int key_page_down;
	static const int key_home;
	static const int key_end;
	static const int key_caps_lock;
	static const int key_scroll_lock;
	static const int key_num_lock;
	static const int key_print_screen;
	static const int key_pause;
	static const int key_f1;
	static const int key_f2;
	static const int key_f3;
	static const int key_f4;
	static const int key_f5;
	static const int key_f6;
	static const int key_f7;
	static const int key_f8;
	static const int key_f9;
	static const int key_f10;
	static const int key_f11;
	static const int key_f12;
	static const int key_left_shift;
	static const int key_left_control;
	static const int key_left_alt;
	static const int key_left_super;
	static const int key_right_shift;
	static const int key_right_control;
	static const int key_right_alt;
	static const int key_right_super;
	static const int key_kb_menu;
	static const int key_kp_0;
	static const int key_kp_1;
	static const int key_kp_2;
	static const int key_kp_3;
	static const int key_kp_4;
	static const int key_kp_5;
	static const int key_kp_6;
	static const int key_kp_7;
	static const int key_kp_8;
	static const int key_kp_9;
	static const int key_kp_decimal;
	static const int key_kp_divide;
	static const int key_kp_multiply;
	static const int key_kp_subtract;
	static const int key_kp_add;
	static const int key_kp_enter;
	static const int key_kp_equal;
	static const int key_back;
	static const int key_menu;
	static const int key_volume_up;
	static const int key_volume_down;
	// 鼠标按钮
	static const int mouse_button_left;
	static const int mouse_button_right;
	static const int mouse_button_middle;
	static const int mouse_button_side;
	static const int mouse_button_extra;
	static const int mouse_button_forward;
	static const int mouse_button_back;
};
//字母转换为key value
int letter_to_kv(char letter);
//数字转换为key value
int number_to_kv(int number);
class F_Color {
public:
	int R, G, B;
	float A;
	struct Color To_RlCol();
	operator struct Color(); static Color LightGray;
	 static Color Gray;
	 static Color DarkGray;
	 static Color DrakGray;
	 static Color Gold;
	 static Color Orange;
	 static Color Pink;
	 static Color Red;
	 static Color Maroon;
	 static Color Green;
	 static Color Lime;
	 static Color DarkGreen;
	 static Color SkyBlue;
	 static Color Blue;
	 static Color DarkBlue;
	 static Color Purple;
	 static Color Violet;
	 static Color Darkpurple;
	 static Color Beige;
	 static Color Borwn;
	 static Color DarkBrown;
	 static Color White;
	 static Color Black;
	 static Color Blank;
	 static Color Magenta;
	 static Color RayWhite;
	 static Color Yellow;
};
	
Color Make_Color_RGB(int r, int g, int b, float a = 1.0f);
F_Color Make_FColor_RGB(int r, int g, int b, float a = 1.0f);
//绘制
class F_Render {
	static float _draw_line_thick;
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
	//设置
	static void Draw_Set_Line_Thick(float thick);
	//struct
	static void Draw_Sprite_Ex(struct Texture* img, int ind, float x, float y, float x_scale, float y_scale, float x_origin, float y_origin, float angle, float alpha, struct Color col);
	static void Draw_Text_Ex(struct Font font, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float size, struct Color col, float alpha = 1.0f);
	static void Draw_Shape(F_Shape shape, struct Color col);
	static void Draw_Rectangle(F_Rectangle rect, struct Color col);
	static void Draw_Circle( F_Circle circle, struct Color col);
	static void Draw_Ellipse(F_Ellipse ellipse,struct Color col);
	static void Draw_Shape_Fill(F_Shape shape, struct Color col);
	static void Draw_Polygon(F_Polygon polygon, struct Color col);
	static void Draw_Polygon(std::vector<FVec2> points, Color color);
	static void Draw_Triangle(F_Triangle triangle, struct  Color col);
	static void Draw_Line(F_Line line, Color col);
	static void Draw_Text_Outline(struct Font fnt, const char* text, float x, float y, float o_x,float o_y, float rot,  float spacing,float font_size, float thick, Color col, Color outline_color,float alpha = 1.0f, bool fill_all = false);
	static void Draw_Text_Shadow(struct Font fnt, const char* text,float x, float y,  float o_x,float o_y, float rot, float spacing,float font_size, float thick, Color col, Color shadow_color,float alpha = 1.0f, bool fill_all = false);

	//以下封装raylib的绘图函数

	static void Draw_Line(float x1,float y1,float x2,float y2,Color col);
	static void Draw_Rectangle(float x,float y,float w,float h,Color col,bool line_mode=false);
	static void Draw_Rectangle(float x, float y, float w, float h,float rot,FVec2 origin,Color col, bool line_mode = false);
	static void Draw_Circle(float x, float y, float radius, Color col, bool line_mode = false);
	static void Draw_Circle(float x, float y, float radius, float startAngle, float angle, Color col, bool line_mode = false);
	static void Draw_Ellipse(float x, float y, float a, float b,Color col, bool line_mode = false);
	static void Draw_Triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color col, bool line_mode = false);
	static void Draw_Triangle(float x1, float y1, float x2, float y2, float x3, float y3, float rot, Color col, bool line_mode=false);
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
	RenderTexture Get();
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
	static FilePathList drop_list;
	static bool loaded;
public:
	//选择文件(返回""为失败)
	static std::string Get_Open_File_Name(std::string strFilter);
	//选择文件(返回""为失败)
	static std::string Get_Open_File_Name(std::string strFilter, unsigned int flag);
	//
	static void Flush_Drop_Files();
	static const char* Get_Drop_File(int index);
	F_File();
	~F_File();
};

//ini
class F_Ini {
public:
	//[Class]
	//name = ...
	//...
	int Get_Int_Value_From_Name(std::string Class,std::string name); 
	std::string Get_String_Value_From_Name(std::string Class, std::string name);
	double Get_Double_Value_From_Name(std::string Class, std::string name);

	bool Write_Int_Value(std::string Class, std::string name,int value);
	bool Write_String_Value(std::string Class, std::string name,std::string value);
	bool Write_Double_Value(std::string Class, std::string name,double value);
};

