/**
 * @file FloatApi.h
 * @brief 浮动引擎API
 * @defgroup FloatApi 浮动引擎模块
 *
 * 提供浮动引擎的各种功能：
 * - 调试功能
 * - 输入处理
 * - 颜色处理
 * - 渲染功能
 * - 摄像机管理
 * - 音频播放
 * - 文件处理
 * - INI/JSON文件处理(1.4.2)
 * - Lua支持(1.4.3)
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
  * @brief 提供调试功能
  *
  * @note 使用示例：
  * @code
  * F_Debug::Init(true);
  * F_Debug::Start();
  * @endcode
  */
class F_Debug {
    static int m_debug;
public:
    /**
     * @brief 初始化调试功能
     * @param debug 是否开启调试
     */
    static void Init(bool debug = true);

    /**
     * @brief 开启调试
     */
    static void Start();

    /**
     * @brief 关闭调试
     */
    static void Close();

    /**
     * @brief 判断调试是否开启
     * @return 如果开启返回true，否则返回false
     */
    static bool IsOpen();

};

/**
 * @brief 打印调试日志
 * @param lv 日志级别
 * @param text 日志内容
 * @param english 是否使用英文
 * @param auto_enter 是否自动换行
 */
void DEBUG_LOG(int lv, const char* text, bool english = 1, bool auto_enter = 1);

#define repeat(n) for(int __r_i__=0;__r_i__<int(n);__r_i__++)
typedef float Float;

/**
 * @brief 判断数组中是否含有某数据
 * @tparam T 数组类型
 * @param a 数组
 * @param b 要查找的数据
 * @return 如果包含返回true，否则返回false
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
 * @brief 从文件加载字体
 * @param filePath 文件路径
 * @param size 字体大小
 * @param m_CodePoint 码点
 * @return 加载的字体
 */
Font Load_Font(const char* filePath, int size, const char* m_CodePoint);

/**
 * @brief 从文件加载字体（使用码点文件）
 * @param filePath 文件路径
 * @param size 字体大小
 * @param codePoints_File 码点文件
 * @return 加载的字体
 */
Font Load_Font_File_Codepoints(const char* filePath, int size, const char* codePoints_File);

/**
 * @class F_Input
 * @ingroup FloatApi
 *
 * @brief 提供输入处理功能
 *
 * @note 使用示例：
 * @code
 * if (F_Input::keyboard_pressed(F_Input::key_a)) {
 *     // 处理按键事件
 * }
 * @endcode
 */
class F_Input {
public:
    /**
 * @brief 检查鼠标按钮是否按下
 * @param mb 鼠标按钮
 * @return 如果按下返回true，否则返回false
 */
    static bool mb_down(int mb);

    /**
     * @brief 检查鼠标按钮是否刚刚按下
     * @param mb 鼠标按钮
     * @return 如果刚刚按下返回true，否则返回false
     */
    static bool mb_pressed(int mb);

    /**
     * @brief 检查鼠标按钮是否刚刚释放
     * @param mb 鼠标按钮
     * @return 如果刚刚释放返回true，否则返回false
     */
    static bool mb_released(int mb);

    /**
     * @brief 检查鼠标按钮是否未按下
     * @param mb 鼠标按钮
     * @return 如果未按下返回true，否则返回false
     */
    static bool mb_up(int mb);

    /**
     * @brief 检查键盘按键是否刚刚按下
     * @param key 键盘按键
     * @return 如果刚刚按下返回true，否则返回false
     */
    static bool keyboard_pressed(int key);

    /**
     * @brief 检查键盘按键是否按下
     * @param key 键盘按键
     * @return 如果按下返回true，否则返回false
     */
    static bool keyboard_down(int key);

    /**
     * @brief 检查键盘按键是否刚刚释放
     * @param key 键盘按键
     * @return 如果刚刚释放返回true，否则返回false
     */
    static bool keyboard_released(int key);

    /**
     * @brief 检查键盘按键是否重复按下
     * @param key 键盘按键
     * @return 如果重复按下返回true，否则返回false
     */
    static bool keyboard_pressed_repeat(int key);


    /**
     * @brief 字母转换为键值
     * @param letter 字母
     * @return 键值
     */
    static int to_value(char letter);

    /**
     * @brief 数字转换为键值
     * @param number 数字
     * @return 键值
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

    // 鼠标按钮
    static const int mouse_button_left;
    static const int mouse_button_right;
    static const int mouse_button_middle;
    static const int mouse_button_side;
    static const int mouse_button_extra;
    static const int mouse_button_forward;
    static const int mouse_button_back;
};

/**
 * @brief 字母转换为键值
 * @param letter 字母
 * @return 键值
 */
int letter_to_kv(char letter);

/**
 * @brief 数字转换为键值
 * @param number 数字
 * @return 键值
 */
int number_to_kv(int number);

/**
 * @class F_Color
 * @ingroup FloatApi
 *
 * @brief 提供颜色处理功能
 *
 * @note 使用示例：
 * @code
 * F_Color color = Make_FColor_RGB(255, 0, 0, 1.0f);
 * @endcode
 */
class F_Color {
public:
    int R, G, B;
    float A;

    /**
     * @brief 转换为raylib的Color对象
     * @return Color对象
     */
    struct Color To_RlCol();

    /**
     * @brief 类型转换操作符重载
     * @return Color对象
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
 * @brief 创建RGB颜色
 * @param r 红色分量
 * @param g 绿色分量
 * @param b 蓝色分量
 * @param a 透明度
 * @return 颜色对象
 */
Color Make_Color_RGB(int r, int g, int b, float a = 1.0f);

/**
 * @brief 创建FColor对象
 * @param r 红色分量
 * @param g 绿色分量
 * @param b 蓝色分量
 * @param a 透明度
 * @return FColor对象
 */
F_Color Make_FColor_RGB(int r, int g, int b, float a = 1.0f);

/**
 * @class F_Render
 * @ingroup FloatApi
 *
 * @brief 提供渲染功能
 *
 * @note 使用示例：
 * @code
 * F_Render::Draw_Sprite_Ex(&texture, 0, 100, 100, 1.0f, 1.0f, 0.5f, 0.5f, 0.0f, 1.0f, RED);
 * @endcode
 */
class F_Render {
    static float _draw_line_thick;
public:
    /**
     * @brief 设置线条粗细
     * @param thick 粗细值
     */
    static void Draw_Set_Line_Thick(float thick);
    /**
     * @brief 绘制精灵
     * @param img 图片纹理
     * @param ind 帧索引
     * @param x x坐标
     * @param y y坐标
     * @param x_scale x轴缩放
     * @param y_scale y轴缩放
     * @param x_origin x轴原点
     * @param y_origin y轴原点
     * @param angle 旋转角度
     * @param alpha 透明度
     * @param col 颜色
     */
    static void Draw_Sprite_Ex(struct Texture* img, int ind, float x, float y, float x_scale, float y_scale, float x_origin, float y_origin, float angle, float alpha, struct Color col);
    /**
    * @brief 绘制精灵
    * @param spr 精灵对象
    * @param x x坐标
    * @param y y坐标
    * @param x_scale x轴缩放
    * @param y_scale y轴缩放
    * @param x_origin x轴原点
    * @param y_origin y轴原点
    * @param angle 旋转角度
    * @param alpha 透明度
    * @param col 颜色
    */
    static void Draw_Sprite(Sprite spr, float x, float y, float x_scale, float y_scale, float x_origin, float y_origin, float angle, float alpha, struct Color col);
    /**
     * @brief 绘制文本
     * @param font 字体
     * @param text 文本内容
     * @param x x坐标
     * @param y y坐标
     * @param o_x x轴原点
     * @param o_y y轴原点
     * @param rot 旋转角度
     * @param spacing 字符间距
     * @param size 字体大小
     * @param col 颜色
     * @param alpha 透明度
     */
    static void Draw_Text_Ex(struct Font font, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float size, struct Color col, float alpha = 1.0f);

    /**
     * @brief 绘制形状
     * @param shape 形状对象
     * @param col 颜色
     */
    static void Draw_Shape(F_Shape shape, struct Color col);

    /**
     * @brief 绘制矩形
     * @param rect 矩形对象
     * @param col 颜色
     */
    static void Draw_Rectangle(F_Rectangle rect, struct Color col);

    /**
     * @brief 绘制圆形
     * @param circle 圆形对象
     * @param col 颜色
     */
    static void Draw_Circle(F_Circle circle, struct Color col);

    /**
     * @brief 绘制椭圆
     * @param ellipse 椭圆对象
     * @param col 颜色
     */
    static void Draw_Ellipse(F_Ellipse ellipse, struct Color col);

    /**
     * @brief 填充形状
     * @param shape 形状对象
     * @param col 颜色
     */
    static void Draw_Shape_Fill(F_Shape shape, struct Color col);

    /**
     * @brief 绘制多边形
     * @param polygon 多边形对象
     * @param col 颜色
     */
    static void Draw_Polygon(F_Polygon polygon, struct Color col);

    /**
     * @brief 绘制多边形
     * @param points 多边形顶点
     * @param color 颜色
     */
    static void Draw_Polygon(std::vector<FVec2> points, Color color);

    /**
     * @brief 绘制三角形
     * @param triangle 三角形对象
     * @param col 颜色
     */
    static void Draw_Triangle(F_Triangle triangle, struct Color col);

    /**
     * @brief 绘制线条
     * @param line 线条对象
     * @param col 颜色
     */
    static void Draw_Line(F_Line line, Color col);

    /**
     * @brief 绘制带轮廓的文本
     * @param fnt 字体
     * @param text 文本内容
     * @param x x坐标
     * @param y y坐标
     * @param o_x x轴原点
     * @param o_y y轴原点
     * @param rot 旋转角度
     * @param spacing 字符间距
     * @param font_size 字体大小
     * @param thick 轮廓厚度
     * @param col 颜色
     * @param outline_color 轮廓颜色
     * @param alpha 透明度
     * @param fill_all 是否填充所有字符
     */
    static void Draw_Text_Outline(struct Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color outline_color, float alpha = 1.0f, bool fill_all = false);

    /**
     * @brief 绘制带阴影的文本
     * @param fnt 字体
     * @param text 文本内容
     * @param x x坐标
     * @param y y坐标
     * @param o_x x轴原点
     * @param o_y y轴原点
     * @param rot 旋转角度
     * @param spacing 字符间距
     * @param font_size 字体大小
     * @param thick 阴影厚度
     * @param col 颜色
     * @param shadow_color 阴影颜色
     * @param alpha 透明度
     * @param fill_all 是否填充所有字符
     */
    static void Draw_Text_Shadow(struct Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color shadow_color, float alpha = 1.0f, bool fill_all = false);

    /**
     * @brief 绘制带轮廓的文本
     * @param fnt 字体
     * @param text 文本内容
     * @param x x坐标
     * @param y y坐标
     * @param o_x x轴原点
     * @param o_y y轴原点
     * @param rot 旋转角度
     * @param spacing 字符间距
     * @param font_size 字体大小
     * @param thick 轮廓厚度
     * @param col 颜色
     * @param outline_color 轮廓颜色
     * @param alpha 透明度
     * @param fill_all 是否填充所有字符
     */
    static void Draw_Text_Outline(F_Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color outline_color, float alpha = 1.0f, bool fill_all = false);

    /**
     * @brief 绘制带阴影的文本
     * @param fnt 字体
     * @param text 文本内容
     * @param x x坐标
     * @param y y坐标
     * @param o_x x轴原点
     * @param o_y y轴原点
     * @param rot 旋转角度
     * @param spacing 字符间距
     * @param font_size 字体大小
     * @param thick 阴影厚度
     * @param col 颜色
     * @param shadow_color 阴影颜色
     * @param alpha 透明度
     * @param fill_all 是否填充所有字符
     */
    static void Draw_Text_Shadow(F_Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color shadow_color, float alpha = 1.0f, bool fill_all = false);

    /**
     * @brief 绘制文本
     * @param font 字体
     * @param text 文本内容
     * @param x x坐标
     * @param y y坐标
     * @param o_x x轴原点
     * @param o_y y轴原点
     * @param rot 旋转角度
     * @param spacing 字符间距
     * @param size 字体大小
     * @param col 颜色
     * @param alpha 透明度
     */
    static void Draw_Text_Ex(F_Font font, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float size, struct Color col, float alpha = 1.0f);

    /**
     * @brief 绘制文本
     * @param text 文本内容
     * @param x x坐标
     * @param y y坐标
     * @param o_x x轴原点
     * @param o_y y轴原点
     * @param rot 旋转角度
     * @param size 字体大小
     * @param col 颜色
     * @param alpha 透明度
     */
    static void Draw_Text(const char* text, float x, float y, float o_x, float o_y, float rot, float size, struct Color col, float alpha = 1.0f);

    /**
     * @brief 绘制线条
     * @param x1 起点x坐标
     * @param y1 起点y坐标
     * @param x2 终点x坐标
     * @param y2 终点y坐标
     * @param col 颜色
     */
    static void Draw_Line(float x1, float y1, float x2, float y2, Color col);

    /**
     * @brief 绘制矩形
     * @param x x坐标
     * @param y y坐标
     * @param w 宽度
     * @param h 高度
     * @param col 颜色
     * @param line_mode 是否为线框模式
     */
    static void Draw_Rectangle(float x, float y, float w, float h, Color col, bool line_mode = false);

    /**
     * @brief 绘制旋转矩形
     * @param x x坐标
     * @param y y坐标
     * @param w 宽度
     * @param h 高度
     * @param rot 旋转角度
     * @param origin 旋转原点
     * @param col 颜色
     * @param line_mode 是否为线框模式
     */
    static void Draw_Rectangle(float x, float y, float w, float h, float rot, FVec2 origin, Color col, bool line_mode = false);

    /**
     * @brief 绘制圆形
     * @param x x坐标
     * @param y y坐标
     * @param radius 半径
     * @param col 颜色
     * @param line_mode 是否为线框模式
     */
    static void Draw_Circle(float x, float y, float radius, Color col, bool line_mode = false);

    /**
     * @brief 绘制圆弧
     * @param x x坐标
     * @param y y坐标
     * @param radius 半径
     * @param startAngle 起始角度
     * @param angle 角度
     * @param col 颜色
     * @param line_mode 是否为线框模式
     */
    static void Draw_Circle(float x, float y, float radius, float startAngle, float angle, Color col, bool line_mode = false);

    /**
     * @brief 绘制椭圆
     * @param x x坐标
     * @param y y坐标
     * @param a 长轴
     * @param b 短轴
     * @param col 颜色
     * @param line_mode 是否为线框模式
     */
    static void Draw_Ellipse(float x, float y, float a, float b, Color col, bool line_mode = false);

    /**
     * @brief 绘制三角形
     * @param x1 顶点1 x坐标
     * @param y1 顶点1 y坐标
     * @param x2 顶点2 x坐标
     * @param y2 顶点2 y坐标
     * @param x3 顶点3 x坐标
     * @param y3 顶点3 y坐标
     * @param col 颜色
     * @param line_mode 是否为线框模式
     */
    static void Draw_Triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color col, bool line_mode = false);

    /**
     * @brief 绘制旋转三角形
     * @param x1 顶点1 x坐标
     * @param y1 顶点1 y坐标
     * @param x2 顶点2 x坐标
     * @param y2 顶点2 y坐标
     * @param x3 顶点3 x坐标
     * @param y3 顶点3 y坐标
     * @param rot 旋转角度
     * @param col 颜色
     * @param line_mode 是否为线框模式
     */
    static void Draw_Triangle(float x1, float y1, float x2, float y2, float x3, float y3, float rot, Color col, bool line_mode = false);
};

/**
 * @class F_Camera
 * @ingroup FloatApi
 *
 * @brief 提供摄像机管理功能
 *
 * @note 使用示例：
 * @code
 * F_Camera camera;
 * camera.Load(800, 600);
 * Begin_Mode_FCamera(camera, BLACK);
 * // 渲染内容
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
     * @brief 默认构造函数
     */
    F_Camera();

    /**
     * @brief 拷贝构造函数
     * @param c F_Camera对象
     */
    F_Camera(const F_Camera* c);

    /**
     * @brief 加载摄像机
     * @param w 宽度
     * @param h 高度
     * @param _auto_free 是否自动释放
     */
    void Load(float w, float h, bool _auto_free = true);

    /**
     * @brief 卸载摄像机
     */
    void Unload();

    /**
     * @brief 获取摄像机宽度
     * @return 宽度
     */
    float Get_Width() const;

    /**
     * @brief 获取摄像机高度
     * @return 高度
     */
    float Get_Height() const;

    /**
     * @brief 获取摄像机渲染纹理
     * @return 渲染纹理
     */
    RenderTexture Get();

    /**
     * @brief 析构函数
     */
    ~F_Camera();
};

/**
 * @brief 加载摄像机
 * @param w 宽度
 * @param h 高度
 * @param camera 摄像机对象
 */
void Load_FCamera(float w, float h, F_Camera* camera);

/**
 * @brief 卸载摄像机
 * @param camera 摄像机对象
 */
void Unload_FCamera(F_Camera* camera);

/**
 * @brief 开始摄像机模式
 * @param _camera 摄像机对象
 * @param Background_Color 背景颜色
 */
void Begin_Mode_FCamera(F_Camera& _camera, Color Background_Color);

/**
 * @brief 结束摄像机模式
 * @param _camera 摄像机对象
 * @param Background_Color 背景颜色
 * @param reverse 是否反转
 */
void End_Mode_FCamera(F_Camera& _camera, Color Background_Color, int reverse = 0);

/**
 * @brief 开始摄像机模式
 * @param _camera 摄像机对象
 * @param Background_Color 背景颜色
 */
void Begin_Mode_FCamera(F_Camera* _camera, Color Background_Color);

/**
 * @brief 结束摄像机模式
 * @param _camera 摄像机对象
 * @param Background_Color 背景颜色
 * @param reverse 是否反转
 */
void End_Mode_FCamera(F_Camera* _camera, Color Background_Color, int reverse = 0);

/**
 * @brief 播放音频
 * @param audio 音频对象
 */
void Audio_Play(F_Audio audio);

/**
 * @brief 停止音频
 * @param audio 音频对象
 */
void Audio_Stop(F_Audio audio);

/**
 * @brief 更新音乐
 * @param audio 音频对象
 */
void Audio_Music_Update(F_Audio audio);

/**
 * @class F_File
 * @ingroup FloatApi
 *
 * @brief 提供文件处理功能
 *
 * @note 使用示例：
 * @code
 * std::string filename = F_File::Get_Open_File_Name("*.txt");
 * @endcode
 */
class F_File {
    static FilePathList drop_list;
    static bool loaded;
public:
    /**
     * @brief 选择文件
     * @param strFilter 文件过滤器
     * @return 文件名（失败返回空字符串）
     */
    static std::string Get_Open_File_Name(std::string strFilter);

    /**
     * @brief 选择文件
     * @param strFilter 文件过滤器
     * @param flag 标志
     * @return 文件名（失败返回空字符串）
     */
    static std::string Get_Open_File_Name(std::string strFilter, unsigned int flag);

    /**
     * @brief 选择文件
     * @param strFilter 文件过滤器
     * @param flag 标志
     * @return 文件名（失败返回空字符串）
     */
    static std::wstring Get_Open_File_NameW(std::wstring strFilter, unsigned int flag);

    /**
     * @brief 刷新拖放文件列表
     */
    static void Flush_Drop_Files();

    /**
     * @brief 获取拖放文件
     * @param index 索引
     * @return 文件名
     */
    static const char* Get_Drop_File(int index);

    /**
     * @brief 构造函数
     */
    F_File();

    /**
     * @brief 析构函数
     */
    ~F_File();
};

/**
 * @class F_Ini
 * @ingroup FloatApi
 *
 * @brief 提供INI文件处理功能
 *
 * @note 使用示例：
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
     * @brief 获取整数值
     * @param Class 类名
     * @param name 名称
     * @return 整数值
     */
    int Get_Int_Value_From_Name(const std::string& Class,const std::string& name);

    /**
     * @brief 获取字符串值
     * @param Class 类名
     * @param name 名称
     * @return 字符串值
     */
    std::string Get_String_Value_From_Name(const std::string& Class, const std::string& name);

    /**
     * @brief 获取双精度值
     * @param Class 类名
     * @param name 名称
     * @return 双精度值
     */
    double Get_Double_Value_From_Name(const std::string& Class,const std::string& name);

    /**
     * @brief 写入整数值
     * @param Class 类名
     * @param name 名称
     * @param value 整数值
     * @return 成功返回true，否则返回false
     */
    bool Write_Int_Value(const std::string& Class,const std::string& name, int value);

    /**
     * @brief 写入字符串值
     * @param Class 类名
     * @param name 名称
     * @param value 字符串值
     * @return 成功返回true，否则返回false
     */
    bool Write_String_Value(const std::string& Class,const std::string& name,const std::string& value);
    /**
    * @brief 写入双精度浮点数值
    * @param Class 类名
    * @param name 名称
    * @param value 字符串值
    * @return 成功返回true，否则返回false
    */
    bool Write_Double_Value(const std::string& Class, const std::string& name,double value);
};
/**
 * @namespace F_Json
 * @brief 用于处理JSON数据的命名空间，提供JSON值的表示、序列化、解析和操作功能
 */
namespace F_Json {
    /**
     * @enum JsonType
     * @brief 表示JSON值的类型枚举
     */
    enum class JsonType {
        Null,    ///< 空值类型
        Boolean, ///< 布尔类型
        Number,  ///< 数值类型（双精度浮点）
        String,  ///< 字符串类型
        Array,   ///< 数组类型
        Object   ///< 对象类型（键值对集合）
    };

    /**
     * @class JsonValue
     * @brief JSON值的抽象基类，定义通用接口
     */
    class JsonValue {
    public:
        virtual ~JsonValue() = default;

        /**
         * @brief 获取当前值的JSON类型
         * @return 对应的JsonType枚举值
         */
        virtual JsonType type() const = 0;

        /**
         * @brief 将JSON值序列化为字符串
         * @return 符合JSON规范的字符串表示
         */
        virtual std::string serialize() const = 0;
    };

    /**
     * @class JsonNull
     * @brief 表示JSON空值的具体类
     */
    class JsonNull : public JsonValue {
    public:
        /**
         * @brief 获取类型（始终返回JsonType::Null）
         */
        JsonType type() const override;

        /**
         * @brief 序列化为"null"字符串
         */
        std::string serialize() const override;
    };

    /**
     * @class JsonBoolean
     * @brief 表示JSON布尔值的具体类
     */
    class JsonBoolean : public JsonValue {
        bool val;
    public:
        /**
         * @brief 构造函数
         * @param v 要存储的布尔值
         */
        explicit JsonBoolean(bool v);

        JsonType type() const override;
        std::string serialize() const override;

        /**
         * @brief 获取存储的布尔值
         * @return 当前布尔值
         */
        bool value() const;
    };
    /**
     * @class JsonNumber
     * @brief 表示JSON数值的具体类
     *
     * 该类用于存储和序列化符合JSON规范的数值类型，使用双精度浮点数进行内部存储。
     * 支持标准浮点数范围和精度，直接映射JSON数值类型。
     */
    class JsonNumber : public JsonValue {
        double val;  ///< 存储的原始数值（遵循IEEE-754双精度标准）

    public:
        /**
         * @brief 构造函数，创建JSON数值对象
         * @param v 要存储的数值（支持常规数值、科学计数法表示等）
         *
         * @note 特殊值处理：
         * - NaN和无穷大值可能产生不符合JSON规范的序列化结果
         * - 示例：
         *   - 输入：3.1415926
         *   - 存储值：3.1415926
         *   - 序列化结果："3.1415926"
         */
        explicit JsonNumber(double v);

        /**
         * @brief 获取当前值的JSON类型
         * @return 始终返回JsonType::Number
         */
        JsonType type() const override;

        /**
         * @brief 将数值序列化为符合JSON标准的字符串表示
         * @return 数值的字符串表示，可能的形式包括：
         *         - 整数形式（如"42"）
         *         - 小数形式（如"3.1415"）
         *         - 科学计数法（如"1.23e+7"）
         *
         * @note 特征：
         * - 不包含多余的小数点（如5.0序列化为"5.0"）
         * - 自动选择最紧凑的表示形式
         * - 不添加任何数字分隔符
         */
        std::string serialize() const override;

        /**
         * @brief 获取存储的原始数值
         * @return 存储的双精度数值副本，保留完整精度
         *
         * @note 示例：
         * - 序列化字符串："-12.5"
         * - 返回值：-12.5
         */
        double value() const;
    };
    /**
 * @class JsonString
 * @brief 表示JSON字符串值的具体类
 *
 * 该类用于存储和操作符合JSON规范的字符串值，自动处理转义字符和引号封装
 */
    class JsonString : public JsonValue {
        std::string val;  ///< 存储原始字符串值（未转义的内部表示）

    public:
        /**
         * @brief 构造函数，创建JSON字符串值
         * @param v 要存储的原始字符串值（将自动进行转义处理）
         *
         * @note 示例：
         * - 输入：Hello\nWorld
         * - 存储值：Hello\nWorld
         * - 序列化结果："Hello\\nWorld"
         */
        explicit JsonString(std::string v);

        /**
         * @brief 获取当前值的JSON类型
         * @return 始终返回JsonType::String
         */
        JsonType type() const override;

        /**
         * @brief 将字符串值序列化为符合JSON标准的格式
         * @return 带双引号的字符串，已正确处理以下转义字符：
         *         " \\ \/ \b \f \n \r \t 和Unicode转义（\uXXXX）
         *
         * @note 示例：
         * - 存储值：Hello "World"
         * - 序列化结果："Hello \"World\""
         */
        std::string serialize() const override;

        /**
         * @brief 获取原始字符串值（未转义的内部表示）
         * @return 存储的原始字符串副本，不包含JSON转义字符
         *
         * @note 示例：
         * - 序列化字符串："Hello\\nWorld"
         * - 返回值：Hello\nWorld
         */
        std::string value() const;
    };

    /**
     * @class JsonArray
     * @brief 表示JSON数组的具体类
     */
    class JsonArray : public JsonValue {
        std::vector<std::shared_ptr<JsonValue>> vals;
    public:
        JsonType type() const override;
        std::string serialize() const override;

        /**
         * @brief 向数组添加元素
         * @param v 要添加的JSON值（所有权转移给数组）
         */
        void add(std::shared_ptr<JsonValue> v);

        /**
         * @brief 获取数组元素个数
         * @return 当前数组大小
         */
        size_t size() const;

        /**
         * @brief 获取指定索引的元素
         * @param i 元素索引（从0开始）
         * @return 共享指针指向对应元素，索引无效时返回空指针
         */
        std::shared_ptr<JsonValue> get(size_t i) const;
    };
    /**
     * @class JsonObject
     * @brief 表示JSON对象的具体类，存储键值对集合
     *
     * 该类实现JSON对象的创建、访问和序列化功能，键为字符串类型，
     * 值可以是任意JsonValue派生类型，自动维护键的字典序排列。
     */
    class JsonObject : public JsonValue {
        std::map<std::string, std::shared_ptr<JsonValue>> vals;  ///< 存储有序键值对容器

    public:
        /**
         * @brief 获取当前值的JSON类型
         * @return 始终返回JsonType::Object
         */
        JsonType type() const override;

        /**
         * @brief 序列化为符合JSON规范的对象字符串
         * @return 格式化为 { "key1":value1, "key2":value2 } 的字符串
         *
         * @note 特征：
         * - 键按字母顺序排列
         * - 字符串键自动添加引号和转义
         * - 值使用各自的serialize()方法
         * - 空对象序列化为"{}"
         */
        std::string serialize() const override;

        /**
         * @brief 设置/更新键值对
         * @param k 键字符串（需符合JSON键规范）
         * @param v 值对象的共享指针（所有权转移给JsonObject）
         *
         * @warning 当键已存在时会覆盖原有值
         * @note 空值处理：
         * - 接受v为空指针，此时等价于存储JsonNull值
         */
        void set(const std::string& k, std::shared_ptr<JsonValue> v);

        /**
         * @brief 检查键是否存在
         * @param k 要查找的键
         * @return true表示存在对应键，false表示不存在
         *
         * @note 执行精确匹配查找，大小写敏感
         */
        bool has(const std::string& k) const;

        /**
         * @brief 获取指定键对应的值
         * @param k 要查找的键
         * @return 找到则返回对应值的共享指针，否则返回nullptr
         *
         * @warning 返回指针的生命周期由JsonObject管理
         * @note 建议使用前配合has()方法进行存在性检查
         */
        std::shared_ptr<JsonValue> get(const std::string& k) const;
        /**
         * @brief 获取对象元素个数
         * @return 当前对象大小
         */
		size_t size() const;
    };
    /**
     * @class Json
     * @brief JSON值的高级封装类，提供类型安全的操作接口
     *
     * 通过工厂方法创建，支持链式调用和类型转换操作
     */
    class Json {
        std::shared_ptr<JsonValue> val;
        explicit Json(std::shared_ptr<JsonValue> v);

    public:
        /**
         * @brief 默认构造为Null类型
         */
        Json();

        /// @name 工厂方法
        /// @{
        static Json Null();      ///< 创建Null类型JSON
        static Json Boolean(bool b);  ///< 创建Boolean类型JSON
        static Json Number(double d); ///< 创建Number类型JSON
        static Json String(const std::string& s); ///< 创建String类型JSON
        static Json Array();     ///< 创建空数组
        static Json Object();    ///< 创建空对象
        /// @}

        /**
         * @brief 获取当前值的类型
         * @throw 如果值为空抛出逻辑错误
         */
        JsonType type() const;

        /// @name 类型转换方法
        /// @{
        /**
         * @brief 转换为布尔值
         * @throw std::logic_error 当前类型不是Boolean时抛出
         */
        bool asBoolean() const;
        /**
         * @brief 将当前JSON值转换为双精度数值
         * @return 当且仅当值为JsonNumber类型时返回存储的数值
         * @throw std::logic_error 当值类型不是JsonType::Number时抛出
         *
         * @note 使用示例：
         * \code
         * Json num = Json::Number(3.14);
         * double d = num.asNumber(); // 返回3.14
         * \endcode
         *
         * @warning 不支持从其他类型（包括数字字符串）的隐式转换，
         *          需要先确认类型正确再调用
         */
        double asNumber() const;

        /**
         * @brief 将当前JSON值转换为字符串
         * @return 当且仅当值为JsonString类型时返回存储的字符串副本
         * @throw std::logic_error 当值类型不是JsonType::String时抛出
         *
         * @note 特性说明：
         * - 返回的字符串为未转义的原始值
         * - 返回值是深拷贝副本，修改不影响原数据
         *
         * @example
         * Json str = Json::String("text");
         * std::string s = str.asString(); // 返回"text"
         */
        std::string asString() const;
        /// @}

        /**
         * @brief 向数组添加元素（仅当当前为数组类型时有效）
         * @param item 要添加的JSON元素
         * @return 自身引用以支持链式调用
         * @throw std::logic_error 当前类型不是Array时抛出
         */
        Json& add(const Json& item);

        /**
         * @brief 获取数组元素（仅当当前为数组类型时有效）
         * @param index 元素索引
         * @return 对应位置的JSON元素，无效索引返回Null类型
         * @throw std::logic_error 当前类型不是Array时抛出
         */
        Json get(size_t index) const;
        /**
         * @brief 获取数组元素（仅当当前为数组类型时有效）
         * @param index 元素索引
         * @return 对应位置的JSON元素，无效索引返回Null类型
         * @throw std::logic_error 当前类型不是Array时抛出
         */
        Json operator[](size_t index) const;

        /**
         * @brief 设置/更新对象中的键值对
         * @param key 要设置的键（需符合JSON键规范，允许空字符串）
         * @param v 要设置的值（支持所有JSON类型）
         * @return 当前Json对象的引用，支持链式调用
         *
         * @note 行为特征：
         * - 当键已存在时，覆盖原有值
         * - 支持深层拷贝，值的修改不会影响原对象
         * - 示例（链式调用）：
         * \code
         * Json::Object()
         *     .set("name", Json::String("Alice"))
         *     .set("age", Json::Number(25));
         * \endcode
         *
         * @warning 键字符串不会自动转义，需确保符合JSON键格式要求
         */
        Json& set(const std::string& key, const Json& v);

        /**
         * @brief 获取对象中指定键对应的值
         * @param key 要查找的键（大小写敏感）
         * @return 存在该键则返回对应值的Json对象，否则返回Null类型的Json对象
         *
         * @note 使用模式建议：
         * \code
         * if (obj.has("key")) {
         *     Json value = obj.get("key");
         *     // 安全使用value
         * }
         * \endcode
         *
         * @note 返回值的生命周期由当前Json对象管理
         * @see has() 方法用于检查键是否存在
         */
        Json get(const std::string& key) const;
        /**
         * @brief 获取对象中指定键对应的值
         * @param key 要查找的键（大小写敏感）
         * @return 存在该键则返回对应值的Json对象，否则返回Null类型的Json对象
         *
         * @note 使用模式建议：
         * \code
         * if (obj.has("key")) {
         *     Json value = obj["key"];
         *     // 安全使用value
         * }
         * \endcode
         *
         * @note 返回值的生命周期由当前Json对象管理
         * @see has() 方法用于检查键是否存在
         */
        Json operator[](const std::string& key) const;

        /**
         * @brief 序列化为JSON字符串
         * @return 符合JSON规范的字符串
         * @throw 序列化失败时可能抛出异常
         */
        std::string serialize() const;

        /**
         * @brief 解析JSON字符串
         * @param s 要解析的JSON字符串
         * @return 解析后的Json对象
         * @throw std::invalid_argument 解析失败时抛出
         */
        static Json parse(const std::string& s);

        /**
         * @brief 检查对象是否包含指定键（仅限Object类型）
         * @param key 要检查的键名
         * @return bool 存在返回true，否则false
         * @throw std::runtime_error 当前值不是Object类型时抛出
         */
        bool has(const std::string& key) const;

        /**
         * @brief 判断当前值是否为Null
         * @return bool 是Null返回true，否则false
         */
        bool isNull() const;

        /**
         * @brief 获取容器大小（仅限Array/Object类型）
         * @return size_t 对于Array返回元素数量，Object返回键数量
         * @throw std::runtime_error 当前值不是Array或Object时抛出
         */
        size_t size() const;
    };
}

/**
 * @class F_Lua
 * @brief Lua虚拟机封装类，提供脚本执行、函数绑定、跨语言调用等核心功能
 *
 * 封装Lua C API，通过RAII管理资源，集成LuaBridge简化C++与Lua的交互。
 * 支持异常安全操作，包含基础错误处理机制。
 */
class F_Lua {
public:
    /**
     * @brief 构造函数，初始化Lua虚拟机
     * @throws std::runtime_error 当无法创建Lua状态机时抛出
     */
    F_Lua();

    /**
     * @brief 析构函数，安全关闭Lua虚拟机
     */
    ~F_Lua();

    // 禁用拷贝语义
    F_Lua(const F_Lua&) = delete;            ///< 禁用拷贝构造
    F_Lua& operator=(const F_Lua&) = delete; ///< 禁用拷贝赋值

    /**
     * @brief 执行Lua脚本文件
     * @param filepath Lua脚本文件路径
     * @return true 执行成功，false 执行失败
     * @note 错误信息会通过HandleError输出到stderr
     */
    bool ExecuteScript(const std::string& filepath);

    /**
     * @brief 执行Lua代码字符串
     * @param code 要执行的Lua代码
     * @return true 执行成功，false 执行失败
     * @note 支持多行代码和复杂逻辑
     */
    bool ExecuteString(const std::string& code);

    /**
     * @brief 注册C++函数到Lua全局命名空间
     * @tparam Func 可调用对象类型（函数指针、lambda等）
     * @param name 在Lua中使用的函数名
     * @param func 要绑定的C++函数
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
     * @brief 注册C++类到Lua环境
     * @tparam T 要注册的类类型
     * @param name 在Lua中使用的类名
     * @note 需要后续通过addFunction/addProperty添加成员
     * @see LuaBridge文档
     */
    template <typename T>
    void RegisterClass(const std::string& name) {
        luabridge::getGlobalNamespace(L)
            .beginClass<T>(name.c_str())
            .addConstructor<void(*)()>()
            .endClass();
    }

    /**
     * @brief 获取Lua全局变量
     * @tparam T 变量类型（自动推导）
     * @param name 全局变量名
     * @return 变量的C++类型值
     * @throws std::runtime_error 当变量不存在或类型不匹配时
     */
    template <typename T>
    T GetGlobal(const std::string& name) {
        return luabridge::getGlobal(L, name.c_str());
    }

    /**
     * @brief 调用无返回值的Lua函数
     * @tparam Args 参数类型包
     * @param funcName Lua函数名
     * @param args 调用参数列表
     * @note 静默忽略不存在的函数
     */
    template <typename... Args>
    void CallVoidFunction(const std::string& funcName, Args... args) {
        auto func = luabridge::getGlobal(L, funcName.c_str());
        if (func.isFunction()) {
            func(args...);
        }
    }

    /**
     * @brief 调用带返回值的Lua函数
     * @tparam Ret 返回值类型
     * @tparam Args 参数类型包
     * @param funcName Lua函数名
     * @param args 调用参数列表
     * @return 函数返回值，如函数不存在返回默认构造的Ret类型值
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
     * @brief 获取底层Lua状态机
     * @return 原始lua_State指针
     * @warning 需谨慎操作底层API
     */
    lua_State* GetState() const { return L; }

private:
    lua_State* L; ///< Lua虚拟机状态指针

    /**
     * @brief 统一错误处理
     * @param status Lua API调用返回状态码
     * @note 自动输出错误信息到stderr，并清理堆栈
     */
    void HandleError(int status);
};