/**
 * @file Game.h
 * @brief 游戏管理类
 * @defgroup Game 游戏模块
 *
 * 提供游戏窗口的创建、管理和控制功能：
 * - 创建和销毁窗口
 * - 设置窗口属性（标题、图标、背景颜色等）
 * - 控制游戏帧率
 */

#pragma once
#include "FloatApi.h"
#include "F_Console.h"

 /**
  * @class Game
  * @ingroup Game
  *
  * @brief 提供游戏窗口的创建、管理和控制功能
  *
  * @note 使用示例：
  * @code
  * Game game;
  * game.CreateWindow(800, 600, "My Game");
  * game.Play(60);
  * @endcode
  */
class Game {
    int _fps; ///< 游戏帧率
    int _w; ///< 窗口宽度
    int _h; ///< 窗口高度
    char* _title; ///< 窗口标题

    bool _load_icon; ///< 是否加载图标
    bool _load_icon_pre; ///< 是否预加载图标
    int _icon_number; ///< 图标数量
    Image _icons[100]; ///< 图标数组
    FilePathList _icon_path; ///< 图标路径列表
    Color background_col; ///< 背景颜色
public:
    /**
     * @brief 构造函数
     */
    Game();

    /**
     * @brief 创建游戏窗口
     * @param w 窗口宽度
     * @param h 窗口高度
     * @param title 窗口标题
     * @param debug 是否开启调试模式
     * @param flags 窗口标志
     */
    void CreateWindow(int w, int h, const char* title, bool debug = true, int flags = 8608);

    /**
     * @brief 判断游戏是否可以开始
     * @return 如果可以开始返回true，否则返回false
     */
    bool CanStart();

    /**
     * @brief 开始游戏循环
     * @param fps 帧率
     */
    void Play(int fps);

    /**
     * @brief 设置游戏帧率
     * @param fps 帧率
     */
    void SetFPS(int fps);

    /**
     * @brief 获取游戏帧率
     * @return 帧率
     */
    int GetFPS() const;

    /**
     * @brief 设置窗口坐标
     * @param x x坐标（-1代表不变）
     * @param y y坐标（-1代表不变）
     */
    void Set_Window_Pos(int x = -1, int y = -1);

    /**
     * @brief 设置窗口透明度
     * @param op 透明度
     */
    void Set_Window_Opacity(float op);

    /**
     * @brief 销毁游戏窗口
     */
    void Destroy();

    /**
     * @brief 设置背景颜色
     * @param bk_col 背景颜色
     */
    void Set_Background_Color(Color bk_col);

    /**
     * @brief 设置控制台图标
     * @param id 图标ID
     */
    void Set_Console_Icon(int id);

    /**
     * @brief 设置窗口标题
     * @param title 窗口标题
     */
    void Set_Title(const char* title);

    /**
     * @brief 设置窗口图标
     * @param icons 图标数组
     * @param n 图标数量
     */
    void Set_Icons(Image* icons, int n);

    /**
     * @brief 设置窗口图标
     * @param icon 图标
     */
    void Set_Icon(Image icon);

    /**
     * @brief 预加载窗口图标
     * @param filePath 图标文件路径
     */
    void Set_Icon_Pre(const char* filePath);

    /**
     * @brief 预加载窗口图标
     * @param filePathList 图标文件路径列表
     */
    void Set_Icons_Pre(FilePathList filePathList);
};

