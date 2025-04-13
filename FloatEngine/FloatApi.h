/**
 * @file FloatApi.h
 * @brief ��������API
 * @defgroup FloatApi ��������ģ��
 *
 * �ṩ��������ĸ��ֹ��ܣ�
 * - ���Թ���
 * - ���봦��
 * - ��ɫ����
 * - ��Ⱦ����
 * - ���������
 * - ��Ƶ����
 * - �ļ�����
 * - INI/JSON�ļ�����(1.4.2)
 * - Lua֧��(1.4.3)
 */

#pragma once
#define FLOAT_API_VERSION "1.4.3"
#define _CRT_SECURE_NO_WARNINGS
#define LINK_WINFUNS
#include "raylib.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <stdexcept>
#include "F_Resource.h"
#include "FMath.h"
#include "F_Audio.h"
#include "winfuns.h"
#include "F_Shader.h"
#include "F_Font.h"
#include "F_Anim.h"
#include "Enum.h"
#include "Sprite.h"
#include <chrono>
#include "lua/include/lua.hpp"
#include "LuaBridge/LuaBridge.h"
 /**
  * @class F_Debug
  * @ingroup FloatApi
  *
  * @brief �ṩ���Թ���
  *
  * @note ʹ��ʾ����
  * @code
  * F_Debug::Init(true);
  * F_Debug::Start();
  * @endcode
  */
class F_Debug {
    static int m_debug;
public:
    /**
     * @brief ��ʼ�����Թ���
     * @param debug �Ƿ�������
     */
    static void Init(bool debug = true);

    /**
     * @brief ��������
     */
    static void Start();

    /**
     * @brief �رյ���
     */
    static void Close();

    /**
     * @brief �жϵ����Ƿ���
     * @return �����������true�����򷵻�false
     */
    static bool IsOpen();

};

/**
 * @brief ��ӡ������־
 * @param lv ��־����
 * @param text ��־����
 * @param english �Ƿ�ʹ��Ӣ��
 * @param auto_enter �Ƿ��Զ�����
 */
void DEBUG_LOG(int lv, const char* text, bool english = 1, bool auto_enter = 1);

#define repeat(n) for(int __r_i__=0;__r_i__<int(n);__r_i__++)
typedef float Float;

/**
 * @brief �ж��������Ƿ���ĳ����
 * @tparam T ��������
 * @param a ����
 * @param b Ҫ���ҵ�����
 * @return �����������true�����򷵻�false
 */
template<class T>
inline bool Array_Has(T a[], T b) {
    for (int i = 0; i < sizeof(a) / sizeof(T); i++) {
        if (a[i] == b) {
            return true;
        }
    }
    return false;
}

#define ARR_SIZE(a) ( sizeof( (a) ) / sizeof( (a[0]) ) )

/**
 * @brief ���ļ���������
 * @param filePath �ļ�·��
 * @param size �����С
 * @param m_CodePoint ���
 * @return ���ص�����
 */
Font Load_Font(const char* filePath, int size, const char* m_CodePoint);

/**
 * @brief ���ļ��������壨ʹ������ļ���
 * @param filePath �ļ�·��
 * @param size �����С
 * @param codePoints_File ����ļ�
 * @return ���ص�����
 */
Font Load_Font_File_Codepoints(const char* filePath, int size, const char* codePoints_File);

/**
 * @class F_Input
 * @ingroup FloatApi
 *
 * @brief �ṩ���봦����
 *
 * @note ʹ��ʾ����
 * @code
 * if (F_Input::keyboard_pressed(F_Input::key_a)) {
 *     // �������¼�
 * }
 * @endcode
 */
class F_Input {
public:
    /**
 * @brief �����갴ť�Ƿ���
 * @param mb ��갴ť
 * @return ������·���true�����򷵻�false
 */
    static bool mb_down(int mb);

    /**
     * @brief �����갴ť�Ƿ�ոհ���
     * @param mb ��갴ť
     * @return ����ոհ��·���true�����򷵻�false
     */
    static bool mb_pressed(int mb);

    /**
     * @brief �����갴ť�Ƿ�ո��ͷ�
     * @param mb ��갴ť
     * @return ����ո��ͷŷ���true�����򷵻�false
     */
    static bool mb_released(int mb);

    /**
     * @brief �����갴ť�Ƿ�δ����
     * @param mb ��갴ť
     * @return ���δ���·���true�����򷵻�false
     */
    static bool mb_up(int mb);

    /**
     * @brief �����̰����Ƿ�ոհ���
     * @param key ���̰���
     * @return ����ոհ��·���true�����򷵻�false
     */
    static bool keyboard_pressed(int key);

    /**
     * @brief �����̰����Ƿ���
     * @param key ���̰���
     * @return ������·���true�����򷵻�false
     */
    static bool keyboard_down(int key);

    /**
     * @brief �����̰����Ƿ�ո��ͷ�
     * @param key ���̰���
     * @return ����ո��ͷŷ���true�����򷵻�false
     */
    static bool keyboard_released(int key);

    /**
     * @brief �����̰����Ƿ��ظ�����
     * @param key ���̰���
     * @return ����ظ����·���true�����򷵻�false
     */
    static bool keyboard_pressed_repeat(int key);


    /**
     * @brief ��ĸת��Ϊ��ֵ
     * @param letter ��ĸ
     * @return ��ֵ
     */
    static int to_value(char letter);

    /**
     * @brief ����ת��Ϊ��ֵ
     * @param number ����
     * @return ��ֵ
     */
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

    // ��갴ť
    static const int mouse_button_left;
    static const int mouse_button_right;
    static const int mouse_button_middle;
    static const int mouse_button_side;
    static const int mouse_button_extra;
    static const int mouse_button_forward;
    static const int mouse_button_back;
};

/**
 * @brief ��ĸת��Ϊ��ֵ
 * @param letter ��ĸ
 * @return ��ֵ
 */
int letter_to_kv(char letter);

/**
 * @brief ����ת��Ϊ��ֵ
 * @param number ����
 * @return ��ֵ
 */
int number_to_kv(int number);

/**
 * @class F_Color
 * @ingroup FloatApi
 *
 * @brief �ṩ��ɫ������
 *
 * @note ʹ��ʾ����
 * @code
 * F_Color color = Make_FColor_RGB(255, 0, 0, 1.0f);
 * @endcode
 */
class F_Color {
public:
    int R, G, B;
    float A;

    /**
     * @brief ת��Ϊraylib��Color����
     * @return Color����
     */
    struct Color To_RlCol();

    /**
     * @brief ����ת������������
     * @return Color����
     */
    operator struct Color();

    static Color LightGray;
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

/**
 * @brief ����RGB��ɫ
 * @param r ��ɫ����
 * @param g ��ɫ����
 * @param b ��ɫ����
 * @param a ͸����
 * @return ��ɫ����
 */
Color Make_Color_RGB(int r, int g, int b, float a = 1.0f);

/**
 * @brief ����FColor����
 * @param r ��ɫ����
 * @param g ��ɫ����
 * @param b ��ɫ����
 * @param a ͸����
 * @return FColor����
 */
F_Color Make_FColor_RGB(int r, int g, int b, float a = 1.0f);

/**
 * @class F_Render
 * @ingroup FloatApi
 *
 * @brief �ṩ��Ⱦ����
 *
 * @note ʹ��ʾ����
 * @code
 * F_Render::Draw_Sprite_Ex(&texture, 0, 100, 100, 1.0f, 1.0f, 0.5f, 0.5f, 0.0f, 1.0f, RED);
 * @endcode
 */
class F_Render {
    static float _draw_line_thick;
public:
    /**
     * @brief ����������ϸ
     * @param thick ��ϸֵ
     */
    static void Draw_Set_Line_Thick(float thick);
    /**
     * @brief ���ƾ���
     * @param img ͼƬ����
     * @param ind ֡����
     * @param x x����
     * @param y y����
     * @param x_scale x������
     * @param y_scale y������
     * @param x_origin x��ԭ��
     * @param y_origin y��ԭ��
     * @param angle ��ת�Ƕ�
     * @param alpha ͸����
     * @param col ��ɫ
     */
    static void Draw_Sprite_Ex(struct Texture* img, int ind, float x, float y, float x_scale, float y_scale, float x_origin, float y_origin, float angle, float alpha, struct Color col);
    /**
    * @brief ���ƾ���
    * @param spr �������
    * @param x x����
    * @param y y����
    * @param x_scale x������
    * @param y_scale y������
    * @param x_origin x��ԭ��
    * @param y_origin y��ԭ��
    * @param angle ��ת�Ƕ�
    * @param alpha ͸����
    * @param col ��ɫ
    */
    static void Draw_Sprite(Sprite spr, float x, float y, float x_scale, float y_scale, float x_origin, float y_origin, float angle, float alpha, struct Color col);
    /**
     * @brief �����ı�
     * @param font ����
     * @param text �ı�����
     * @param x x����
     * @param y y����
     * @param o_x x��ԭ��
     * @param o_y y��ԭ��
     * @param rot ��ת�Ƕ�
     * @param spacing �ַ����
     * @param size �����С
     * @param col ��ɫ
     * @param alpha ͸����
     */
    static void Draw_Text_Ex(struct Font font, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float size, struct Color col, float alpha = 1.0f);

    /**
     * @brief ������״
     * @param shape ��״����
     * @param col ��ɫ
     */
    static void Draw_Shape(F_Shape shape, struct Color col);

    /**
     * @brief ���ƾ���
     * @param rect ���ζ���
     * @param col ��ɫ
     */
    static void Draw_Rectangle(F_Rectangle rect, struct Color col);

    /**
     * @brief ����Բ��
     * @param circle Բ�ζ���
     * @param col ��ɫ
     */
    static void Draw_Circle(F_Circle circle, struct Color col);

    /**
     * @brief ������Բ
     * @param ellipse ��Բ����
     * @param col ��ɫ
     */
    static void Draw_Ellipse(F_Ellipse ellipse, struct Color col);

    /**
     * @brief �����״
     * @param shape ��״����
     * @param col ��ɫ
     */
    static void Draw_Shape_Fill(F_Shape shape, struct Color col);

    /**
     * @brief ���ƶ����
     * @param polygon ����ζ���
     * @param col ��ɫ
     */
    static void Draw_Polygon(F_Polygon polygon, struct Color col);

    /**
     * @brief ���ƶ����
     * @param points ����ζ���
     * @param color ��ɫ
     */
    static void Draw_Polygon(std::vector<FVec2> points, Color color);

    /**
     * @brief ����������
     * @param triangle �����ζ���
     * @param col ��ɫ
     */
    static void Draw_Triangle(F_Triangle triangle, struct Color col);

    /**
     * @brief ��������
     * @param line ��������
     * @param col ��ɫ
     */
    static void Draw_Line(F_Line line, Color col);

    /**
     * @brief ���ƴ��������ı�
     * @param fnt ����
     * @param text �ı�����
     * @param x x����
     * @param y y����
     * @param o_x x��ԭ��
     * @param o_y y��ԭ��
     * @param rot ��ת�Ƕ�
     * @param spacing �ַ����
     * @param font_size �����С
     * @param thick �������
     * @param col ��ɫ
     * @param outline_color ������ɫ
     * @param alpha ͸����
     * @param fill_all �Ƿ���������ַ�
     */
    static void Draw_Text_Outline(struct Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color outline_color, float alpha = 1.0f, bool fill_all = false);

    /**
     * @brief ���ƴ���Ӱ���ı�
     * @param fnt ����
     * @param text �ı�����
     * @param x x����
     * @param y y����
     * @param o_x x��ԭ��
     * @param o_y y��ԭ��
     * @param rot ��ת�Ƕ�
     * @param spacing �ַ����
     * @param font_size �����С
     * @param thick ��Ӱ���
     * @param col ��ɫ
     * @param shadow_color ��Ӱ��ɫ
     * @param alpha ͸����
     * @param fill_all �Ƿ���������ַ�
     */
    static void Draw_Text_Shadow(struct Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color shadow_color, float alpha = 1.0f, bool fill_all = false);

    /**
     * @brief ���ƴ��������ı�
     * @param fnt ����
     * @param text �ı�����
     * @param x x����
     * @param y y����
     * @param o_x x��ԭ��
     * @param o_y y��ԭ��
     * @param rot ��ת�Ƕ�
     * @param spacing �ַ����
     * @param font_size �����С
     * @param thick �������
     * @param col ��ɫ
     * @param outline_color ������ɫ
     * @param alpha ͸����
     * @param fill_all �Ƿ���������ַ�
     */
    static void Draw_Text_Outline(F_Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color outline_color, float alpha = 1.0f, bool fill_all = false);

    /**
     * @brief ���ƴ���Ӱ���ı�
     * @param fnt ����
     * @param text �ı�����
     * @param x x����
     * @param y y����
     * @param o_x x��ԭ��
     * @param o_y y��ԭ��
     * @param rot ��ת�Ƕ�
     * @param spacing �ַ����
     * @param font_size �����С
     * @param thick ��Ӱ���
     * @param col ��ɫ
     * @param shadow_color ��Ӱ��ɫ
     * @param alpha ͸����
     * @param fill_all �Ƿ���������ַ�
     */
    static void Draw_Text_Shadow(F_Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color shadow_color, float alpha = 1.0f, bool fill_all = false);

    /**
     * @brief �����ı�
     * @param font ����
     * @param text �ı�����
     * @param x x����
     * @param y y����
     * @param o_x x��ԭ��
     * @param o_y y��ԭ��
     * @param rot ��ת�Ƕ�
     * @param spacing �ַ����
     * @param size �����С
     * @param col ��ɫ
     * @param alpha ͸����
     */
    static void Draw_Text_Ex(F_Font font, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float size, struct Color col, float alpha = 1.0f);

    /**
     * @brief �����ı�
     * @param text �ı�����
     * @param x x����
     * @param y y����
     * @param o_x x��ԭ��
     * @param o_y y��ԭ��
     * @param rot ��ת�Ƕ�
     * @param size �����С
     * @param col ��ɫ
     * @param alpha ͸����
     */
    static void Draw_Text(const char* text, float x, float y, float o_x, float o_y, float rot, float size, struct Color col, float alpha = 1.0f);

    /**
     * @brief ��������
     * @param x1 ���x����
     * @param y1 ���y����
     * @param x2 �յ�x����
     * @param y2 �յ�y����
     * @param col ��ɫ
     */
    static void Draw_Line(float x1, float y1, float x2, float y2, Color col);

    /**
     * @brief ���ƾ���
     * @param x x����
     * @param y y����
     * @param w ���
     * @param h �߶�
     * @param col ��ɫ
     * @param line_mode �Ƿ�Ϊ�߿�ģʽ
     */
    static void Draw_Rectangle(float x, float y, float w, float h, Color col, bool line_mode = false);

    /**
     * @brief ������ת����
     * @param x x����
     * @param y y����
     * @param w ���
     * @param h �߶�
     * @param rot ��ת�Ƕ�
     * @param origin ��תԭ��
     * @param col ��ɫ
     * @param line_mode �Ƿ�Ϊ�߿�ģʽ
     */
    static void Draw_Rectangle(float x, float y, float w, float h, float rot, FVec2 origin, Color col, bool line_mode = false);

    /**
     * @brief ����Բ��
     * @param x x����
     * @param y y����
     * @param radius �뾶
     * @param col ��ɫ
     * @param line_mode �Ƿ�Ϊ�߿�ģʽ
     */
    static void Draw_Circle(float x, float y, float radius, Color col, bool line_mode = false);

    /**
     * @brief ����Բ��
     * @param x x����
     * @param y y����
     * @param radius �뾶
     * @param startAngle ��ʼ�Ƕ�
     * @param angle �Ƕ�
     * @param col ��ɫ
     * @param line_mode �Ƿ�Ϊ�߿�ģʽ
     */
    static void Draw_Circle(float x, float y, float radius, float startAngle, float angle, Color col, bool line_mode = false);

    /**
     * @brief ������Բ
     * @param x x����
     * @param y y����
     * @param a ����
     * @param b ����
     * @param col ��ɫ
     * @param line_mode �Ƿ�Ϊ�߿�ģʽ
     */
    static void Draw_Ellipse(float x, float y, float a, float b, Color col, bool line_mode = false);

    /**
     * @brief ����������
     * @param x1 ����1 x����
     * @param y1 ����1 y����
     * @param x2 ����2 x����
     * @param y2 ����2 y����
     * @param x3 ����3 x����
     * @param y3 ����3 y����
     * @param col ��ɫ
     * @param line_mode �Ƿ�Ϊ�߿�ģʽ
     */
    static void Draw_Triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color col, bool line_mode = false);

    /**
     * @brief ������ת������
     * @param x1 ����1 x����
     * @param y1 ����1 y����
     * @param x2 ����2 x����
     * @param y2 ����2 y����
     * @param x3 ����3 x����
     * @param y3 ����3 y����
     * @param rot ��ת�Ƕ�
     * @param col ��ɫ
     * @param line_mode �Ƿ�Ϊ�߿�ģʽ
     */
    static void Draw_Triangle(float x1, float y1, float x2, float y2, float x3, float y3, float rot, Color col, bool line_mode = false);
};

/**
 * @class F_Camera
 * @ingroup FloatApi
 *
 * @brief �ṩ�����������
 *
 * @note ʹ��ʾ����
 * @code
 * F_Camera camera;
 * camera.Load(800, 600);
 * Begin_Mode_FCamera(camera, BLACK);
 * // ��Ⱦ����
 * End_Mode_FCamera(camera, BLACK);
 * @endcode
 */
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

    /**
     * @brief Ĭ�Ϲ��캯��
     */
    F_Camera();

    /**
     * @brief �������캯��
     * @param c F_Camera����
     */
    F_Camera(const F_Camera* c);

    /**
     * @brief ���������
     * @param w ���
     * @param h �߶�
     * @param _auto_free �Ƿ��Զ��ͷ�
     */
    void Load(float w, float h, bool _auto_free = true);

    /**
     * @brief ж�������
     */
    void Unload();

    /**
     * @brief ��ȡ��������
     * @return ���
     */
    float Get_Width() const;

    /**
     * @brief ��ȡ������߶�
     * @return �߶�
     */
    float Get_Height() const;

    /**
     * @brief ��ȡ�������Ⱦ����
     * @return ��Ⱦ����
     */
    RenderTexture Get();

    /**
     * @brief ��������
     */
    ~F_Camera();
};

/**
 * @brief ���������
 * @param w ���
 * @param h �߶�
 * @param camera ���������
 */
void Load_FCamera(float w, float h, F_Camera* camera);

/**
 * @brief ж�������
 * @param camera ���������
 */
void Unload_FCamera(F_Camera* camera);

/**
 * @brief ��ʼ�����ģʽ
 * @param _camera ���������
 * @param Background_Color ������ɫ
 */
void Begin_Mode_FCamera(F_Camera& _camera, Color Background_Color);

/**
 * @brief ���������ģʽ
 * @param _camera ���������
 * @param Background_Color ������ɫ
 * @param reverse �Ƿ�ת
 */
void End_Mode_FCamera(F_Camera& _camera, Color Background_Color, int reverse = 0);

/**
 * @brief ��ʼ�����ģʽ
 * @param _camera ���������
 * @param Background_Color ������ɫ
 */
void Begin_Mode_FCamera(F_Camera* _camera, Color Background_Color);

/**
 * @brief ���������ģʽ
 * @param _camera ���������
 * @param Background_Color ������ɫ
 * @param reverse �Ƿ�ת
 */
void End_Mode_FCamera(F_Camera* _camera, Color Background_Color, int reverse = 0);

/**
 * @brief ������Ƶ
 * @param audio ��Ƶ����
 */
void Audio_Play(F_Audio audio);

/**
 * @brief ֹͣ��Ƶ
 * @param audio ��Ƶ����
 */
void Audio_Stop(F_Audio audio);

/**
 * @brief ��������
 * @param audio ��Ƶ����
 */
void Audio_Music_Update(F_Audio audio);

/**
 * @class F_File
 * @ingroup FloatApi
 *
 * @brief �ṩ�ļ�������
 *
 * @note ʹ��ʾ����
 * @code
 * std::string filename = F_File::Get_Open_File_Name("*.txt");
 * @endcode
 */
class F_File {
    static FilePathList drop_list;
    static bool loaded;
public:
    /**
     * @brief ѡ���ļ�
     * @param strFilter �ļ�������
     * @return �ļ�����ʧ�ܷ��ؿ��ַ�����
     */
    static std::string Get_Open_File_Name(std::string strFilter);

    /**
     * @brief ѡ���ļ�
     * @param strFilter �ļ�������
     * @param flag ��־
     * @return �ļ�����ʧ�ܷ��ؿ��ַ�����
     */
    static std::string Get_Open_File_Name(std::string strFilter, unsigned int flag);

    /**
     * @brief ѡ���ļ�
     * @param strFilter �ļ�������
     * @param flag ��־
     * @return �ļ�����ʧ�ܷ��ؿ��ַ�����
     */
    static std::wstring Get_Open_File_NameW(std::wstring strFilter, unsigned int flag);

    /**
     * @brief ˢ���Ϸ��ļ��б�
     */
    static void Flush_Drop_Files();

    /**
     * @brief ��ȡ�Ϸ��ļ�
     * @param index ����
     * @return �ļ���
     */
    static const char* Get_Drop_File(int index);

    /**
     * @brief ���캯��
     */
    F_File();

    /**
     * @brief ��������
     */
    ~F_File();
};

/**
 * @class F_Ini
 * @ingroup FloatApi
 *
 * @brief �ṩINI�ļ�������
 *
 * @note ʹ��ʾ����
 * @code
 * F_Ini ini;
 * int value = ini.Get_Int_Value_From_Name("Settings", "Volume");
 * @endcode
 */
class F_Ini {
    std::map<std::string, std::map<std::string, std::string>> sections;
    std::string currentFile;
    std::string currentSection;

    void parse_line(std::string& line);
public:
    F_Ini(const std::string& filename = "");

    bool Load(const std::string& filename);
    bool Save(const std::string& filename = "");
    /**
     * @brief ��ȡ����ֵ
     * @param Class ����
     * @param name ����
     * @return ����ֵ
     */
    int Get_Int_Value_From_Name(const std::string& Class,const std::string& name);

    /**
     * @brief ��ȡ�ַ���ֵ
     * @param Class ����
     * @param name ����
     * @return �ַ���ֵ
     */
    std::string Get_String_Value_From_Name(const std::string& Class, const std::string& name);

    /**
     * @brief ��ȡ˫����ֵ
     * @param Class ����
     * @param name ����
     * @return ˫����ֵ
     */
    double Get_Double_Value_From_Name(const std::string& Class,const std::string& name);

    /**
     * @brief д������ֵ
     * @param Class ����
     * @param name ����
     * @param value ����ֵ
     * @return �ɹ�����true�����򷵻�false
     */
    bool Write_Int_Value(const std::string& Class,const std::string& name, int value);

    /**
     * @brief д���ַ���ֵ
     * @param Class ����
     * @param name ����
     * @param value �ַ���ֵ
     * @return �ɹ�����true�����򷵻�false
     */
    bool Write_String_Value(const std::string& Class,const std::string& name,const std::string& value);
    /**
    * @brief д��˫���ȸ�����ֵ
    * @param Class ����
    * @param name ����
    * @param value �ַ���ֵ
    * @return �ɹ�����true�����򷵻�false
    */
    bool Write_Double_Value(const std::string& Class, const std::string& name,double value);
};
/**
 * @namespace F_Json
 * @brief ���ڴ���JSON���ݵ������ռ䣬�ṩJSONֵ�ı�ʾ�����л��������Ͳ�������
 */
namespace F_Json {
    /**
     * @enum JsonType
     * @brief ��ʾJSONֵ������ö��
     */
    enum class JsonType {
        Null,    ///< ��ֵ����
        Boolean, ///< ��������
        Number,  ///< ��ֵ���ͣ�˫���ȸ��㣩
        String,  ///< �ַ�������
        Array,   ///< ��������
        Object   ///< �������ͣ���ֵ�Լ��ϣ�
    };

    /**
     * @class JsonValue
     * @brief JSONֵ�ĳ�����࣬����ͨ�ýӿ�
     */
    class JsonValue {
    public:
        virtual ~JsonValue() = default;

        /**
         * @brief ��ȡ��ǰֵ��JSON����
         * @return ��Ӧ��JsonTypeö��ֵ
         */
        virtual JsonType type() const = 0;

        /**
         * @brief ��JSONֵ���л�Ϊ�ַ���
         * @return ����JSON�淶���ַ�����ʾ
         */
        virtual std::string serialize() const = 0;
    };

    /**
     * @class JsonNull
     * @brief ��ʾJSON��ֵ�ľ�����
     */
    class JsonNull : public JsonValue {
    public:
        /**
         * @brief ��ȡ���ͣ�ʼ�շ���JsonType::Null��
         */
        JsonType type() const override;

        /**
         * @brief ���л�Ϊ"null"�ַ���
         */
        std::string serialize() const override;
    };

    /**
     * @class JsonBoolean
     * @brief ��ʾJSON����ֵ�ľ�����
     */
    class JsonBoolean : public JsonValue {
        bool val;
    public:
        /**
         * @brief ���캯��
         * @param v Ҫ�洢�Ĳ���ֵ
         */
        explicit JsonBoolean(bool v);

        JsonType type() const override;
        std::string serialize() const override;

        /**
         * @brief ��ȡ�洢�Ĳ���ֵ
         * @return ��ǰ����ֵ
         */
        bool value() const;
    };
    /**
     * @class JsonNumber
     * @brief ��ʾJSON��ֵ�ľ�����
     *
     * �������ڴ洢�����л�����JSON�淶����ֵ���ͣ�ʹ��˫���ȸ����������ڲ��洢��
     * ֧�ֱ�׼��������Χ�;��ȣ�ֱ��ӳ��JSON��ֵ���͡�
     */
    class JsonNumber : public JsonValue {
        double val;  ///< �洢��ԭʼ��ֵ����ѭIEEE-754˫���ȱ�׼��

    public:
        /**
         * @brief ���캯��������JSON��ֵ����
         * @param v Ҫ�洢����ֵ��֧�ֳ�����ֵ����ѧ��������ʾ�ȣ�
         *
         * @note ����ֵ����
         * - NaN�������ֵ���ܲ���������JSON�淶�����л����
         * - ʾ����
         *   - ���룺3.1415926
         *   - �洢ֵ��3.1415926
         *   - ���л������"3.1415926"
         */
        explicit JsonNumber(double v);

        /**
         * @brief ��ȡ��ǰֵ��JSON����
         * @return ʼ�շ���JsonType::Number
         */
        JsonType type() const override;

        /**
         * @brief ����ֵ���л�Ϊ����JSON��׼���ַ�����ʾ
         * @return ��ֵ���ַ�����ʾ�����ܵ���ʽ������
         *         - ������ʽ����"42"��
         *         - С����ʽ����"3.1415"��
         *         - ��ѧ����������"1.23e+7"��
         *
         * @note ������
         * - �����������С���㣨��5.0���л�Ϊ"5.0"��
         * - �Զ�ѡ������յı�ʾ��ʽ
         * - ������κ����ַָ���
         */
        std::string serialize() const override;

        /**
         * @brief ��ȡ�洢��ԭʼ��ֵ
         * @return �洢��˫������ֵ������������������
         *
         * @note ʾ����
         * - ���л��ַ�����"-12.5"
         * - ����ֵ��-12.5
         */
        double value() const;
    };
    /**
 * @class JsonString
 * @brief ��ʾJSON�ַ���ֵ�ľ�����
 *
 * �������ڴ洢�Ͳ�������JSON�淶���ַ���ֵ���Զ�����ת���ַ������ŷ�װ
 */
    class JsonString : public JsonValue {
        std::string val;  ///< �洢ԭʼ�ַ���ֵ��δת����ڲ���ʾ��

    public:
        /**
         * @brief ���캯��������JSON�ַ���ֵ
         * @param v Ҫ�洢��ԭʼ�ַ���ֵ�����Զ�����ת�崦��
         *
         * @note ʾ����
         * - ���룺Hello\nWorld
         * - �洢ֵ��Hello\nWorld
         * - ���л������"Hello\\nWorld"
         */
        explicit JsonString(std::string v);

        /**
         * @brief ��ȡ��ǰֵ��JSON����
         * @return ʼ�շ���JsonType::String
         */
        JsonType type() const override;

        /**
         * @brief ���ַ���ֵ���л�Ϊ����JSON��׼�ĸ�ʽ
         * @return ��˫���ŵ��ַ���������ȷ��������ת���ַ���
         *         " \\ \/ \b \f \n \r \t ��Unicodeת�壨\uXXXX��
         *
         * @note ʾ����
         * - �洢ֵ��Hello "World"
         * - ���л������"Hello \"World\""
         */
        std::string serialize() const override;

        /**
         * @brief ��ȡԭʼ�ַ���ֵ��δת����ڲ���ʾ��
         * @return �洢��ԭʼ�ַ���������������JSONת���ַ�
         *
         * @note ʾ����
         * - ���л��ַ�����"Hello\\nWorld"
         * - ����ֵ��Hello\nWorld
         */
        std::string value() const;
    };

    /**
     * @class JsonArray
     * @brief ��ʾJSON����ľ�����
     */
    class JsonArray : public JsonValue {
        std::vector<std::shared_ptr<JsonValue>> vals;
    public:
        JsonType type() const override;
        std::string serialize() const override;

        /**
         * @brief ���������Ԫ��
         * @param v Ҫ��ӵ�JSONֵ������Ȩת�Ƹ����飩
         */
        void add(std::shared_ptr<JsonValue> v);

        /**
         * @brief ��ȡ����Ԫ�ظ���
         * @return ��ǰ�����С
         */
        size_t size() const;

        /**
         * @brief ��ȡָ��������Ԫ��
         * @param i Ԫ����������0��ʼ��
         * @return ����ָ��ָ���ӦԪ�أ�������Чʱ���ؿ�ָ��
         */
        std::shared_ptr<JsonValue> get(size_t i) const;
    };
    /**
     * @class JsonObject
     * @brief ��ʾJSON����ľ����࣬�洢��ֵ�Լ���
     *
     * ����ʵ��JSON����Ĵ��������ʺ����л����ܣ���Ϊ�ַ������ͣ�
     * ֵ����������JsonValue�������ͣ��Զ�ά�������ֵ������С�
     */
    class JsonObject : public JsonValue {
        std::map<std::string, std::shared_ptr<JsonValue>> vals;  ///< �洢�����ֵ������

    public:
        /**
         * @brief ��ȡ��ǰֵ��JSON����
         * @return ʼ�շ���JsonType::Object
         */
        JsonType type() const override;

        /**
         * @brief ���л�Ϊ����JSON�淶�Ķ����ַ���
         * @return ��ʽ��Ϊ { "key1":value1, "key2":value2 } ���ַ���
         *
         * @note ������
         * - ������ĸ˳������
         * - �ַ������Զ�������ź�ת��
         * - ֵʹ�ø��Ե�serialize()����
         * - �ն������л�Ϊ"{}"
         */
        std::string serialize() const override;

        /**
         * @brief ����/���¼�ֵ��
         * @param k ���ַ����������JSON���淶��
         * @param v ֵ����Ĺ���ָ�루����Ȩת�Ƹ�JsonObject��
         *
         * @warning �����Ѵ���ʱ�Ḳ��ԭ��ֵ
         * @note ��ֵ����
         * - ����vΪ��ָ�룬��ʱ�ȼ��ڴ洢JsonNullֵ
         */
        void set(const std::string& k, std::shared_ptr<JsonValue> v);

        /**
         * @brief �����Ƿ����
         * @param k Ҫ���ҵļ�
         * @return true��ʾ���ڶ�Ӧ����false��ʾ������
         *
         * @note ִ�о�ȷƥ����ң���Сд����
         */
        bool has(const std::string& k) const;

        /**
         * @brief ��ȡָ������Ӧ��ֵ
         * @param k Ҫ���ҵļ�
         * @return �ҵ��򷵻ض�Ӧֵ�Ĺ���ָ�룬���򷵻�nullptr
         *
         * @warning ����ָ�������������JsonObject����
         * @note ����ʹ��ǰ���has()�������д����Լ��
         */
        std::shared_ptr<JsonValue> get(const std::string& k) const;
        /**
         * @brief ��ȡ����Ԫ�ظ���
         * @return ��ǰ�����С
         */
		size_t size() const;
    };
    /**
     * @class Json
     * @brief JSONֵ�ĸ߼���װ�࣬�ṩ���Ͱ�ȫ�Ĳ����ӿ�
     *
     * ͨ����������������֧����ʽ���ú�����ת������
     */
    class Json {
        std::shared_ptr<JsonValue> val;
        explicit Json(std::shared_ptr<JsonValue> v);

    public:
        /**
         * @brief Ĭ�Ϲ���ΪNull����
         */
        Json();

        /// @name ��������
        /// @{
        static Json Null();      ///< ����Null����JSON
        static Json Boolean(bool b);  ///< ����Boolean����JSON
        static Json Number(double d); ///< ����Number����JSON
        static Json String(const std::string& s); ///< ����String����JSON
        static Json Array();     ///< ����������
        static Json Object();    ///< �����ն���
        /// @}

        /**
         * @brief ��ȡ��ǰֵ������
         * @throw ���ֵΪ���׳��߼�����
         */
        JsonType type() const;

        /// @name ����ת������
        /// @{
        /**
         * @brief ת��Ϊ����ֵ
         * @throw std::logic_error ��ǰ���Ͳ���Booleanʱ�׳�
         */
        bool asBoolean() const;
        /**
         * @brief ����ǰJSONֵת��Ϊ˫������ֵ
         * @return ���ҽ���ֵΪJsonNumber����ʱ���ش洢����ֵ
         * @throw std::logic_error ��ֵ���Ͳ���JsonType::Numberʱ�׳�
         *
         * @note ʹ��ʾ����
         * \code
         * Json num = Json::Number(3.14);
         * double d = num.asNumber(); // ����3.14
         * \endcode
         *
         * @warning ��֧�ִ��������ͣ����������ַ���������ʽת����
         *          ��Ҫ��ȷ��������ȷ�ٵ���
         */
        double asNumber() const;

        /**
         * @brief ����ǰJSONֵת��Ϊ�ַ���
         * @return ���ҽ���ֵΪJsonString����ʱ���ش洢���ַ�������
         * @throw std::logic_error ��ֵ���Ͳ���JsonType::Stringʱ�׳�
         *
         * @note ����˵����
         * - ���ص��ַ���Ϊδת���ԭʼֵ
         * - ����ֵ������������޸Ĳ�Ӱ��ԭ����
         *
         * @example
         * Json str = Json::String("text");
         * std::string s = str.asString(); // ����"text"
         */
        std::string asString() const;
        /// @}

        /**
         * @brief ���������Ԫ�أ�������ǰΪ��������ʱ��Ч��
         * @param item Ҫ��ӵ�JSONԪ��
         * @return ����������֧����ʽ����
         * @throw std::logic_error ��ǰ���Ͳ���Arrayʱ�׳�
         */
        Json& add(const Json& item);

        /**
         * @brief ��ȡ����Ԫ�أ�������ǰΪ��������ʱ��Ч��
         * @param index Ԫ������
         * @return ��Ӧλ�õ�JSONԪ�أ���Ч��������Null����
         * @throw std::logic_error ��ǰ���Ͳ���Arrayʱ�׳�
         */
        Json get(size_t index) const;
        /**
         * @brief ��ȡ����Ԫ�أ�������ǰΪ��������ʱ��Ч��
         * @param index Ԫ������
         * @return ��Ӧλ�õ�JSONԪ�أ���Ч��������Null����
         * @throw std::logic_error ��ǰ���Ͳ���Arrayʱ�׳�
         */
        Json operator[](size_t index) const;

        /**
         * @brief ����/���¶����еļ�ֵ��
         * @param key Ҫ���õļ��������JSON���淶��������ַ�����
         * @param v Ҫ���õ�ֵ��֧������JSON���ͣ�
         * @return ��ǰJson��������ã�֧����ʽ����
         *
         * @note ��Ϊ������
         * - �����Ѵ���ʱ������ԭ��ֵ
         * - ֧����㿽����ֵ���޸Ĳ���Ӱ��ԭ����
         * - ʾ������ʽ���ã���
         * \code
         * Json::Object()
         *     .set("name", Json::String("Alice"))
         *     .set("age", Json::Number(25));
         * \endcode
         *
         * @warning ���ַ��������Զ�ת�壬��ȷ������JSON����ʽҪ��
         */
        Json& set(const std::string& key, const Json& v);

        /**
         * @brief ��ȡ������ָ������Ӧ��ֵ
         * @param key Ҫ���ҵļ�����Сд���У�
         * @return ���ڸü��򷵻ض�Ӧֵ��Json���󣬷��򷵻�Null���͵�Json����
         *
         * @note ʹ��ģʽ���飺
         * \code
         * if (obj.has("key")) {
         *     Json value = obj.get("key");
         *     // ��ȫʹ��value
         * }
         * \endcode
         *
         * @note ����ֵ�����������ɵ�ǰJson�������
         * @see has() �������ڼ����Ƿ����
         */
        Json get(const std::string& key) const;
        /**
         * @brief ��ȡ������ָ������Ӧ��ֵ
         * @param key Ҫ���ҵļ�����Сд���У�
         * @return ���ڸü��򷵻ض�Ӧֵ��Json���󣬷��򷵻�Null���͵�Json����
         *
         * @note ʹ��ģʽ���飺
         * \code
         * if (obj.has("key")) {
         *     Json value = obj["key"];
         *     // ��ȫʹ��value
         * }
         * \endcode
         *
         * @note ����ֵ�����������ɵ�ǰJson�������
         * @see has() �������ڼ����Ƿ����
         */
        Json operator[](const std::string& key) const;

        /**
         * @brief ���л�ΪJSON�ַ���
         * @return ����JSON�淶���ַ���
         * @throw ���л�ʧ��ʱ�����׳��쳣
         */
        std::string serialize() const;

        /**
         * @brief ����JSON�ַ���
         * @param s Ҫ������JSON�ַ���
         * @return �������Json����
         * @throw std::invalid_argument ����ʧ��ʱ�׳�
         */
        static Json parse(const std::string& s);

        /**
         * @brief �������Ƿ����ָ����������Object���ͣ�
         * @param key Ҫ���ļ���
         * @return bool ���ڷ���true������false
         * @throw std::runtime_error ��ǰֵ����Object����ʱ�׳�
         */
        bool has(const std::string& key) const;

        /**
         * @brief �жϵ�ǰֵ�Ƿ�ΪNull
         * @return bool ��Null����true������false
         */
        bool isNull() const;

        /**
         * @brief ��ȡ������С������Array/Object���ͣ�
         * @return size_t ����Array����Ԫ��������Object���ؼ�����
         * @throw std::runtime_error ��ǰֵ����Array��Objectʱ�׳�
         */
        size_t size() const;
    };
}

/**
 * @class F_Lua
 * @brief Lua�������װ�࣬�ṩ�ű�ִ�С������󶨡������Ե��õȺ��Ĺ���
 *
 * ��װLua C API��ͨ��RAII������Դ������LuaBridge��C++��Lua�Ľ�����
 * ֧���쳣��ȫ����������������������ơ�
 */
class F_Lua {
public:
    /**
     * @brief ���캯������ʼ��Lua�����
     * @throws std::runtime_error ���޷�����Lua״̬��ʱ�׳�
     */
    F_Lua();

    /**
     * @brief ������������ȫ�ر�Lua�����
     */
    ~F_Lua();

    // ���ÿ�������
    F_Lua(const F_Lua&) = delete;            ///< ���ÿ�������
    F_Lua& operator=(const F_Lua&) = delete; ///< ���ÿ�����ֵ

    /**
     * @brief ִ��Lua�ű��ļ�
     * @param filepath Lua�ű��ļ�·��
     * @return true ִ�гɹ���false ִ��ʧ��
     * @note ������Ϣ��ͨ��HandleError�����stderr
     */
    bool ExecuteScript(const std::string& filepath);

    /**
     * @brief ִ��Lua�����ַ���
     * @param code Ҫִ�е�Lua����
     * @return true ִ�гɹ���false ִ��ʧ��
     * @note ֧�ֶ��д���͸����߼�
     */
    bool ExecuteString(const std::string& code);

    /**
     * @brief ע��C++������Luaȫ�������ռ�
     * @tparam Func �ɵ��ö������ͣ�����ָ�롢lambda�ȣ�
     * @param name ��Lua��ʹ�õĺ�����
     * @param func Ҫ�󶨵�C++����
     * @code
     * lua.RegisterFunction("Add", [](int a, int b){ return a + b; });
     * @endcode
     */
    template <typename Func>
    void RegisterFunction(const std::string& name, Func func) {
        luabridge::getGlobalNamespace(L)
            .addFunction(name.c_str(), func);
    }

    /**
     * @brief ע��C++�ൽLua����
     * @tparam T Ҫע���������
     * @param name ��Lua��ʹ�õ�����
     * @note ��Ҫ����ͨ��addFunction/addProperty��ӳ�Ա
     * @see LuaBridge�ĵ�
     */
    template <typename T>
    void RegisterClass(const std::string& name) {
        luabridge::getGlobalNamespace(L)
            .beginClass<T>(name.c_str())
            .addConstructor<void(*)()>()
            .endClass();
    }

    /**
     * @brief ��ȡLuaȫ�ֱ���
     * @tparam T �������ͣ��Զ��Ƶ���
     * @param name ȫ�ֱ�����
     * @return ������C++����ֵ
     * @throws std::runtime_error �����������ڻ����Ͳ�ƥ��ʱ
     */
    template <typename T>
    T GetGlobal(const std::string& name) {
        return luabridge::getGlobal(L, name.c_str());
    }

    /**
     * @brief �����޷���ֵ��Lua����
     * @tparam Args �������Ͱ�
     * @param funcName Lua������
     * @param args ���ò����б�
     * @note ��Ĭ���Բ����ڵĺ���
     */
    template <typename... Args>
    void CallVoidFunction(const std::string& funcName, Args... args) {
        auto func = luabridge::getGlobal(L, funcName.c_str());
        if (func.isFunction()) {
            func(args...);
        }
    }

    /**
     * @brief ���ô�����ֵ��Lua����
     * @tparam Ret ����ֵ����
     * @tparam Args �������Ͱ�
     * @param funcName Lua������
     * @param args ���ò����б�
     * @return ��������ֵ���纯�������ڷ���Ĭ�Ϲ����Ret����ֵ
     */
    template <typename Ret, typename... Args>
    Ret CallFunction(const std::string& funcName, Args... args) {
        auto func = luabridge::getGlobal(L, funcName.c_str());
        if (func.isFunction()) {
            return func(args...);
        }
        return Ret();
    }

    /**
     * @brief ��ȡ�ײ�Lua״̬��
     * @return ԭʼlua_Stateָ��
     * @warning ����������ײ�API
     */
    lua_State* GetState() const { return L; }

private:
    lua_State* L; ///< Lua�����״ָ̬��

    /**
     * @brief ͳһ������
     * @param status Lua API���÷���״̬��
     * @note �Զ����������Ϣ��stderr���������ջ
     */
    void HandleError(int status);
};