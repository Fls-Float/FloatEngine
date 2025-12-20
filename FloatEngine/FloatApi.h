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
 * - F_Resource 支持加密算法,项目使用vcpkg进行包管理(1.5.0)
 */

#pragma once
#define FLOAT_API_VERSION "1.5.0"
#define _CRT_SECURE_NO_WARNINGS
#define LINK_WINFUNS  

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <stdexcept>
#include <chrono>
#include <unordered_set>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <type_traits>
#include <any>
#include <queue>
#ifdef _MSC_VER
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#endif
#define SOL_ALL_SAFETIES_ON 1
#define SOL_STRINGS_ARE_NUMBERS 1
#define SOL_PRINT_ERRORS 1
#include "sol/sol.hpp"
#include "raylib.h"
#include "nlohmann/json.hpp"


#include "F_Resource.h"
#include "FMath.h"
#include "F_Audio.h"
#include "winfuns.h"
#include "F_Shader.h"
#include "F_Font.h"
#include "F_Anim.h"
#include "Enum.h"
#include "Sprite.h"
#include "F_Http.h"
#include "F_Network.h"
struct ImGuiInputTextCallbackData;
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
    static bool m_showConsole;
    
    struct Log_Info{
        std::string log;
        int lv;
    };
    // 控制台状态
    static std::vector<Log_Info> m_logs;
    static std::vector<std::string> m_commandHistory;
    static char m_inputBuffer[256];
    static int m_historyPos;
    static bool m_scrollToBottom;
    
    // 命令系统
    struct DebugCommand {
        std::string name;
        std::string description;
        std::function<void(const std::vector<std::string>&)> action;
    };
    static std::vector<DebugCommand> m_commands;
    static std::unordered_map<std::string, std::string> m_commandAliases; // 命令别名
public:
    // 日志过滤系统
    static std::unordered_set<std::string> m_logFilters;
    static bool m_showFilteredLogsOnly;

    // 变量监控系统
    struct WatchedVariable {
        std::string name;
        std::string expression; // 原始表达式
        std::function<std::string()> getter;
        bool showInConsole;
        std::string lastValue;
    };
    static std::vector<WatchedVariable> m_watchedVariables;
    static std::unordered_map<std::string, std::function<std::string(const std::string&)>> m_expressionHandlers;

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

    /**
     * @brief 启用imgui的console窗口
    */
    static void ShowDebugConsole();

    /**
     * @brief 添加日志消息
     * @param message 日志内容
     * @param level 日志级别 (0=none,1=info, 2=warn, 3=error)
     */
    static void Log(const std::string& message, int level = 0);
    /**
     * @brief 清空日志消息
     */
    static void ClearLog();
    /**
     * @brief 注册调试命令
     * @param name 命令名称
     * @param description 命令描述
     * @param action 命令执行函数
     */
    static void RegisterCommand(const std::string& name, const std::string& description,
        std::function<void(const std::vector<std::string>&)> action);
    /**
    * @brief 获取所有调试命令
    */
    static std::vector<DebugCommand> GetCommands();

    /**
    * @brief 获取指定调试命令
    */
    static DebugCommand GetCommand(const std::string& name);
    /**
     * @brief 执行调试命令
     * @param command 完整命令字符串
     */
    static void ExecuteCommand(const std::string& command);
    
    /**
    * @brief 初始化命令
    */
    static void InitCommand();

    /**
    * @brief 输入框回调函数 (处理自动补全和历史记录)
    * @param data ImGui回调数据
    */
    static int TextEditCallback(ImGuiInputTextCallbackData* data);

    /**
    * @brief 命令分词
    * @param input 输入命令
    * @param tokens 输出分词结果
    */

    static void TokenizeCommand(const std::string& input, std::vector<std::string>& tokens);

    /**
     * @brief 添加日志过滤关键词
     * @param keyword 过滤关键词（大小写敏感）
     */
    static void AddLogFilter(const std::string& keyword);

    /**
     * @brief 移除日志过滤关键词
     * @param keyword 要移除的关键词
     */
    static void RemoveLogFilter(const std::string& keyword);

    /**
     * @brief 判断日志是否应该显示
     * @param log 日志内容
     * @return 如果日志匹配任何过滤关键词则返回true
     */
    static bool ShouldShowLog(const std::string& log);
    

    /**
     * @brief 保存日志到文件
     * @param filename 文件名
     * @param filtered 是否只保存过滤后的日志
     */
    static void SaveLogsToFile(const std::string& filename, bool filtered = false);

    /**
     * @brief 注册命令别名
     * @param alias 别名
     * @param command 实际执行的命令
     */
    static void RegisterAlias(const std::string& alias, const std::string& command);

    /**
     * @brief 注册变量监控
     * @param name 变量名称（显示用）
     * @param getter 获取变量值的函数
     * @param showInConsole 是否在控制台显示
     */
    static void WatchVariable(const std::string& name,
        std::function<std::string()> getter,
        bool showInConsole = true);
    /**
     * @brief 注册变量监控
     * @param name 变量名称（唯一标识）
     * @param expression 表达式或变量名
     * @param showInConsole 是否在控制台显示
     */
    static void WatchVariable(const std::string& name,
        const std::string& expression,
        bool showInConsole = true);
    /**
     * @brief 移除变量监控
     * @param name 变量名称
     */
    static void UnwatchVariable(const std::string& name);
    /**
    * @brief 设置变量在控制台的显示状态
    * @param name 变量名称
    * @param show 是否显示
    */
    static void SetVariableVisibility(const std::string& name, bool show);
    /**
     * @brief 更新所有监控变量（应在每帧调用）
     */
    static void UpdateWatchedVariables();
    /**
    * @brief 注册表达式处理函数
    * @param prefix 表达式前缀
    * @param handler 处理函数
    * @code
    *  RegisterExpressionHandler("player.", [](const std::vector<std::string>& tokens) {
        // 示例：player.health, player.position.x
        if (tokens.empty()) return std::string("Invalid player expression");
        
        if (tokens[0] == "health") {
            return std::to_string(GetPlayerHealth());
        }
        else if (tokens[0] == "position") {
            Vector3 pos = GetPlayerPosition();
            if (tokens.size() > 1) {
                if (tokens[1] == "x") return std::to_string(pos.x);
                if (tokens[1] == "y") return std::to_string(pos.y);
                if (tokens[1] == "z") return std::to_string(pos.z);
            }
            return "(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + ")";
        }
        return "Unknown player property: " + tokens[0];
    });
    */
    static void RegisterExpressionHandler(const std::string& prefix,
        std::function<std::string(const std::vector<std::string>&)> handler);

    /**
     * @brief 解析并执行表达式
     * @param expression 表达式字符串
     * @return 表达式结果
     */
    static std::string EvaluateExpression(const std::string& expression);

};


/**
 * @brief 打印调试日志
 * @param lv 日志级别
 * @param text 日志内容
 * @param english 是否使用英文
 * @param auto_enter 是否自动换行
 */
void DEBUG_LOG(int lv, const char* text, bool english = 1, bool auto_enter = 1,bool with_imgui_console = 1);


template<typename... Args>
inline void DEBUG_LOGF(int lv, bool english, const char* fmt, Args&&... args) {
    if (!fmt) {
        DEBUG_LOG(lv, "(null fmt)", english, 1,1);
        return;
    }
    const char* formatted = TextFormat(fmt, std::forward<Args>(args)...);
    if (!formatted) formatted = "(format error)";

    DEBUG_LOG(lv, formatted, english, 1, 1);
}
#define FLOG_ALLF(fmt, ...)   DEBUG_LOGF(LOG_ALL,    true,  fmt, ##__VA_ARGS__)
#define FLOG_DEBUGF(fmt, ...) DEBUG_LOGF(LOG_DEBUG,  true,  fmt, ##__VA_ARGS__)
#define FLOG_INFOF(fmt, ...)  DEBUG_LOGF(LOG_INFO,   true,  fmt, ##__VA_ARGS__)
#define FLOG_WARNF(fmt, ...)  DEBUG_LOGF(LOG_WARNING,true,  fmt, ##__VA_ARGS__)
#define FLOG_ERRORF(fmt, ...) DEBUG_LOGF(LOG_ERROR,  true,  fmt, ##__VA_ARGS__)
#define FLOG_FATALF(fmt, ...) DEBUG_LOGF(LOG_FATAL,  true,  fmt, ##__VA_ARGS__)

#define FLOG_INFOF_CN(fmt, ...) DEBUG_LOGF(LOG_INFO, false, fmt, ##__VA_ARGS__)

#define FLOG_ALL(msg)   DEBUG_LOG(LOG_ALL,    msg, true, 1, 1)
#define FLOG_DEBUG(msg) DEBUG_LOG(LOG_DEBUG,  msg, true, 1, 1)
#define FLOG_INFO(msg)  DEBUG_LOG(LOG_INFO,   msg, true, 1, 1)
#define FLOG_WARN(msg)  DEBUG_LOG(LOG_WARNING,msg, true, 1, 1)
#define FLOG_ERROR(msg) DEBUG_LOG(LOG_ERROR,  msg, true, 1, 1)
#define FLOG_FATAL(msg) DEBUG_LOG(LOG_FATAL,  msg, true, 1, 1)
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
     * @brief 设置默认字体
     */
    static void Set_Default_Font(Font font);

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
 * @class F_File
 * @ingroup FloatApi
 *
 * @brief 提供文件处理功能
 *
 * @note 使用示例：
 * @code
 * std::string filename = F_File::Get_Open_File_Name("*.txt");
 * //filter = Text Files (*.txt)\0*.txt\0   
 * @endcode
 */
class F_File {
    static FilePathList drop_list;
    static bool loaded;
public:
    /**
     * @brief 选择打开文件
     * @param strFilter 文件过滤器
     * @return 文件名（失败返回空字符串）
     */
    static std::string Get_Open_File_Name(const char* strFilter);

    /**
    * @brief 保存文件
    * @param strFilter 文件过滤器
	* @param d_name 默认文件名
    * @return 文件名（失败返回空字符串）
    */
    std::string Get_Save_File_Name(const char* strFilter,const char*d_name=nullptr);

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
    using nlohmann::json;
    enum class JsonType {
        Null, Boolean, Number, String, Array, Object
    };

    
    struct JsonValueImpl {
        json data;
        explicit JsonValueImpl(const json& j) : data(j) {}
        explicit JsonValueImpl(json&& j) noexcept : data(std::move(j)) {}
        JsonValueImpl() = default;
    };
    class Json {
    private:
        std::shared_ptr<JsonValueImpl> val;
        bool no_name = false; 

        explicit Json(std::shared_ptr<JsonValueImpl> v);

    public:
        Json(); // 默认构造为 Null
        Json(std::initializer_list<Json> list);
        Json(std::initializer_list<std::pair<const std::string, Json>> list);

        template<typename T>
        Json& operator=(const std::vector<T>& vec) {
            val->data = json(vec);
            return *this;
        }

        template<typename K, typename V>
        Json& operator=(const std::map<K, V>& map) {
            val->data = json::object();

            // 遍历传入的 map
            for (const auto& pair : map) {
                std::string key = pair.first;

                if constexpr (std::is_same_v<V, Json>) {
                    val->data[key] = pair.second.val->data;
                }
                else {
                    val->data[key] = pair.second;
                }
            }
            return *this;
        }
        class iterator;
        class const_iterator;

        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;

        static Json Null();
        static Json Boolean(bool b);
        static Json Number(double d);
        static Json String(const std::string& s);
        static Json Array();
        static Json Object();

        Json& operator=(double n);
        Json& operator=(const char* str);
        Json& operator=(const std::string& s);
        Json& operator=(bool b);
        Json& operator=(const Json& j);

        // ----------------------------------------------------------------------
        // 状态/类型/转换
        // ----------------------------------------------------------------------
        JsonType type() const;
        bool isNull() const;
        bool has(const std::string& key) const;
        size_t size() const;

        bool asBoolean() const;
        double asNumber() const;
        std::string asString() const;

        // ----------------------------------------------------------------------
        // 容器/操作
        // ----------------------------------------------------------------------
        Json& add(const Json& item);
        Json get(size_t index) const;
        Json operator[](size_t index) const;
        Json& set(const std::string& key, const Json& v);
        Json get(const std::string& key) const;
        Json operator[](const std::string& key) const;

        // ----------------------------------------------------------------------
        // 序列化/解析
        // ----------------------------------------------------------------------
        std::string serialize() const;
        std::string serialize_with_format() const;
        static Json parse(const std::string& s);

        // 宏方法
        void setNoName() { no_name = true; }
        void setHasName() { no_name = false; }

        const JsonValueImpl& getInternalValue() const;
    };
} // namespace F_Json


/**
 * @class F_Lua
 * @brief 基于sol2的Lua虚拟机封装类，提供增强的脚本执行、函数绑定、跨语言调用等核心功能
 *
 * 封装sol2现代C++ Lua绑定库，通过RAII管理资源，提供类型安全的C++与Lua交互。
 * 支持异常安全操作，包含完整的错误处理机制，自动内存管理和高级特性。
 */
class F_Lua {
public:
    /**
     * @brief 类注册器，用于链式注册C++类到Lua环境
     */
    template <typename T>
    class ClassRegistrar {
    public:
        ClassRegistrar(sol::state& lua, const std::string& name, bool is_derived = false)
            : lua_(lua), class_name_(name), is_derived_(is_derived) {
        }

        /**
         * @brief 启用std::shared_ptr智能指针管理
         */
        ClassRegistrar<T>& enableSharedPtr() {
            use_shared_ptr_ = true;
            return *this;
        }

        /**
         * @brief 添加构造函数
         */
        template <typename... Args>
        ClassRegistrar& addConstructor() {
            constructors_ = std::make_tuple<Args...>();
            return *this;
        }

        /**
         * @brief 添加成员函数
         */
        template <typename Func>
        ClassRegistrar& addFunction(const std::string& name, Func func) {
            functions_[name] = func;
            return *this;
        }

        /**
         * @brief 添加属性
         */
        template <typename Prop>
        ClassRegistrar& addProperty(const std::string& name, Prop prop) {
            properties_[name] = prop;
            return *this;
        }

        /**
         * @brief 添加静态函数
         */
        template <typename Func>
        ClassRegistrar& addStaticFunction(const std::string& name, Func func) {
            static_functions_[name] = func;
            return *this;
        }

        /**
         * @brief 添加析构函数定制
         */
        template <typename Destructor>
        ClassRegistrar& addDestructor(Destructor dtor) {
            destructor_ = dtor;
            return *this;
        }

        /**
         * @brief 完成类注册
         */
        void endClass() {
            // 实际的注册逻辑在析构函数中执行
        }

        ~ClassRegistrar() {
            registerClass();
        }

    private:
        void registerClass() {
            if (is_derived_) {
                // 派生类注册
                auto usertype = lua_.new_usertype<T>(
                    class_name_,
                    sol::base_classes, sol::bases<std::remove_const_t<std::remove_pointer_t<decltype(getBaseClass<T>())>>>()...
                );
                setupUsertype(usertype);
            }
            else {
                // 基类注册
                auto usertype = lua_.new_usertype<T>(class_name_);
                setupUsertype(usertype);
            }
        }

        template <typename U>
        void setupUsertype(sol::usertype<U>& usertype) {
            // 注册构造函数
            std::apply([&](auto... args) {
                usertype.set(sol::call_constructor, sol::constructors<args...>());
                }, constructors_);

            // 注册成员函数
            for (const auto& [name, func] : functions_) {
                usertype.set(name, func);
            }

            // 注册属性
            for (const auto& [name, prop] : properties_) {
                usertype.set(name, prop);
            }

            // 注册静态函数
            for (const auto& [name, func] : static_functions_) {
                usertype.set(name, func);
            }

            // 注册析构函数
            if (destructor_) {
                usertype.set(sol::meta_function::garbage_collect, destructor_);
            }

            // 如果启用shared_ptr，添加相应的工厂函数
            if (use_shared_ptr_) {
                lua_[class_name_]["create"] = [](auto&&... args) {
                    return std::make_shared<T>(std::forward<decltype(args)>(args)...);
                    };
            }
        }

        // 辅助函数用于获取基类类型
        template <typename U>
        static auto getBaseClass() -> void;

    private:
        sol::state& lua_;
        std::string class_name_;
        bool is_derived_;
        bool use_shared_ptr_ = false;
        std::tuple<> constructors_;
        std::unordered_map<std::string, std::function<void()>> functions_;
        std::unordered_map<std::string, std::function<void()>> properties_;
        std::unordered_map<std::string, std::function<void()>> static_functions_;
        std::function<void(T*)> destructor_;
    };

public:
    /**
     * @brief 构造函数，初始化Lua虚拟机并加载标准库
     * @throws std::runtime_error 当无法创建Lua状态机时抛出
     */
    F_Lua();

    /**
     * @brief 析构函数，安全关闭Lua虚拟机
     */
    ~F_Lua();

    // 禁用拷贝语义
    F_Lua(const F_Lua&) = delete;
    F_Lua& operator=(const F_Lua&) = delete;

    // 支持移动语义
    F_Lua(F_Lua&&) = default;
    F_Lua& operator=(F_Lua&&) = default;

    /**
     * @brief 执行Lua脚本文件
     * @param filepath Lua脚本文件路径
     * @return true 执行成功，false 执行失败
     * @note 自动处理错误并输出详细信息
     */
    bool ExecuteScript(const std::string& filepath);

    /**
     * @brief 执行Lua代码字符串
     * @param code 要执行的Lua代码
     * @return true 执行成功，false 执行失败
     * @note 支持多行代码和复杂逻辑，提供详细错误信息
     */
    bool ExecuteString(const std::string& code);

    /**
     * @brief 安全执行Lua代码字符串，返回执行结果
     * @param code 要执行的Lua代码
     * @return sol::protected_function_result 包含执行结果和错误信息
     */
    sol::protected_function_result ExecuteStringSafe(const std::string& code);

    /**
     * @brief 注册C++函数到Lua全局命名空间
     * @tparam Func 可调用对象类型
     * @param name 在Lua中使用的函数名
     * @param func 要绑定的C++函数
     */
    template <typename Func>
    void RegisterFunction(const std::string& name, Func func) {
        lua_.set_function(name, func);
    }

    /**
     * @brief 注册C++类到Lua环境
     * @tparam T 要注册的类类型
     * @param name 在Lua中使用的类名
     * @param is_derived 是否为派生类
     * @return ClassRegistrar<T> 用于链式添加成员的对象
     */
    template <typename T>
    ClassRegistrar<T> RegisterClass(const std::string& name, bool is_derived = false) {
        return ClassRegistrar<T>(lua_, name, is_derived);
    }

    /**
     * @brief 获取Lua全局变量
     * @tparam T 变量类型
     * @param name 全局变量名
     * @return 变量的C++类型值
     * @throws sol::error 当变量不存在或类型不匹配时
     */
    template <typename T>
    T GetGlobal(const std::string& name) {
        return lua_[name];
    }

    /**
     * @brief 设置Lua全局变量
     * @tparam T 变量类型
     * @param name 全局变量名
     * @param value 要设置的值
     */
    template <typename T>
    void SetGlobal(const std::string& name, T&& value) {
        lua_[name] = std::forward<T>(value);
    }

    /**
     * @brief 调用无返回值的Lua函数
     * @tparam Args 参数类型包
     * @param funcName Lua函数名
     * @param args 调用参数列表
     * @return bool 是否成功调用
     */
    template <typename... Args>
    bool CallVoidFunction(const std::string& funcName, Args&&... args) {
        sol::protected_function func = lua_[funcName];
        if (func.valid()) {
            auto result = func(std::forward<Args>(args)...);
            return result.valid();
        }
        return false;
    }

    /**
     * @brief 安全调用Lua函数，返回保护结果
     * @tparam Args 参数类型包
     * @param funcName Lua函数名
     * @param args 调用参数列表
     * @return sol::protected_function_result 调用结果
     */
    template <typename... Args>
    sol::protected_function_result CallFunctionSafe(const std::string& funcName, Args&&... args) {
        sol::protected_function func = lua_[funcName];
        if (func.valid()) {
            return func(std::forward<Args>(args)...);
        }
        return sol::protected_function_result();
    }

    /**
     * @brief 调用带返回值的Lua函数
     * @tparam Ret 返回值类型
     * @tparam Args 参数类型包
     * @param funcName Lua函数名
     * @param args 调用参数列表
     * @return std::optional<Ret> 函数返回值，如调用失败返回std::nullopt
     */
    template <typename Ret, typename... Args>
    std::optional<Ret> CallFunction(const std::string& funcName, Args&&... args) {
        auto result = CallFunctionSafe(funcName, std::forward<Args>(args)...);
        if (result.valid() && result.return_count() > 0) {
            return result.template get<Ret>();
        }
        return std::nullopt;
    }

    /**
     * @brief 检查全局变量是否存在
     * @param name 变量名
     * @return bool 是否存在
     */
    bool HasGlobal(const std::string& name) {
        sol::object obj = lua_[name];
        return obj.valid() && obj.get_type() != sol::type::nil;
    }

    /**
     * @brief 检查全局函数是否存在
     * @param name 函数名
     * @return bool 是否存在且为函数
     */
    bool HasFunction(const std::string& name) {
        sol::object obj = lua_[name];
        return obj.valid() && obj.is<sol::function>();
    }

    /**
     * @brief 获取底层Lua状态机
     * @return sol::state& Lua状态引用
     */
    sol::state& GetState() { return lua_; }

    /**
     * @brief 获取底层Lua状态机（const版本）
     * @return const sol::state& Lua状态引用
     */
    const sol::state& GetState() const { return lua_; }

    /**
     * @brief 获取原生lua_State指针
     * @return lua_State* 原始Lua状态指针
     */
    lua_State* GetRawState() { return lua_; }

    /**
     * @brief 清空Lua环境
     */
    void ClearEnvironment() {
        lua_.collect_garbage();
    }

    /**
 * @brief 设置Lua内存管理参数
 * @param limit_kb 内存限制提示（目前仅用于调整垃圾回收频率）
 */
    void SetMemoryLimit(size_t limit_kb) {
        // 使用参数来调整垃圾回收策略
        if (limit_kb > 0) {
            // 小内存限制：更频繁的垃圾回收
            int pause = std::max(10, 100 - static_cast<int>(limit_kb / 100));
            lua_["collectgarbage"]("setpause", pause);
            lua_["collectgarbage"]("setstepmul", 500);
        }
        else {
            // 默认设置
            lua_["collectgarbage"]("setpause", 100);
            lua_["collectgarbage"]("setstepmul", 200);
        }
    }

    /**
     * @brief 加载但不执行Lua代码
     * @param code Lua代码
     * @return sol::load_result 加载结果
     */
    sol::load_result LoadString(const std::string& code) {
        return lua_.load(code);
    }

    /**
     * @brief 创建新的Lua环境表
     * @return sol::table 新创建的表
     */
    sol::table CreateTable() {
        return lua_.create_table();
    }

    /**
     * @brief 创建带命名空间的全局表
     * @param namespace_path 命名空间路径（如 "Game.Player"）
     * @return sol::table 命名空间表
     */
    sol::table CreateNamespace(const std::string& namespace_path) {
        sol::table current = lua_.globals();
        std::string token;
        std::istringstream token_stream(namespace_path);

        while (std::getline(token_stream, token, '.')) {
            if (!current[token].valid()) {
                current[token] = lua_.create_table();
            }
            current = current[token];
        }

        return current;
    }

    /**
     * @brief 注册C++枚举到Lua
     * @tparam Enum 枚举类型
     * @param name 枚举在Lua中的名称
     * @param is_global 是否注册为全局变量
     */
    template <typename Enum>
    void RegisterEnum(const std::string& name, bool is_global = true) {
        sol::table enum_table = lua_.create_table();

        // 这里需要用户手动添加枚举值
        if (is_global) {
            lua_[name] = enum_table;
        }

        enum_tables_[name] = enum_table;
    }

    /**
     * @brief 为枚举添加值
     * @tparam Enum 枚举类型
     * @param enum_name 枚举名称
     * @param value_name 值名称
     * @param value 枚举值
     */
    template <typename Enum>
    void AddEnumValue(const std::string& enum_name, const std::string& value_name, Enum value) {
        auto it = enum_tables_.find(enum_name);
        if (it != enum_tables_.end()) {
            it->second[value_name] = value;
        }
    }

private:
    sol::state lua_;
    std::unordered_map<std::string, sol::table> enum_tables_;

    /**
     * @brief 统一错误处理
     * @param result 保护函数执行结果
     */
    void HandleError(const sol::protected_function_result& result);
};

namespace F_Gui {
    // 初始化与状态管理
    void Init();
    void Shutdown();
    void BeginFrame();
    void EndFrame();

    // 窗口管理
    void BeginWindow(const char* title, bool* p_open = nullptr, int flags = 0);
    void EndWindow();

    // 基础控件
    bool Button(const char* label, const Vector2& size = { 0,0 });
    void Text(const char* fmt, ...);
    bool Checkbox(const char* label, bool* v);
    bool SliderFloat(const char* label, float* v, float v_min, float v_max);
    bool ColorEdit3(const char* label, float col[3]);

    // 布局控件
    void BeginGroup(const char* id);
    void EndGroup();
    void SameLine(float offset_x = 0.0f, float spacing = -1.0f);

    // 高级控件
    bool TreeNode(const char* label);
    void TreePop();
    void PlotLines(const char* label, const float* values, int values_count,
        float scale_min = FLT_MAX, float scale_max = FLT_MAX,
        Vector2 size = { 0,0 });

    // 游戏引擎专用组件
    void Vector3Control(const char* label, Vector3& value, float resetValue = 0.0f);
    void TexturePreview(Texture2D texture, Vector2 size = { 100,100 });
    void FrameRateOverlay();

    // 样式系统
    void PushStyleColor(Color idx, Color color);
    void PopStyleColor(int count = 1);
    void LoadTheme(const char* name); // "dark", "light", "classic"

    // 自定义绘制
    void DrawLine(const Vector2& start, const Vector2& end, Color color, float thickness = 1.0f);
    void DrawRect(const rlRectangle& rect, Color color, float rounding = 0.0f);

    // 扩展系统
    using CustomWidgetFunc = std::function<void()>;
    void RegisterWidget(const char* name, CustomWidgetFunc func);
    void ShowWidget(const char* name);
};


template<typename T>
template<typename U>
inline auto F_Lua::ClassRegistrar<T>::getBaseClass() -> void
{
}



