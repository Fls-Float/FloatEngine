/**
 * @file Game.h
 * @brief ��Ϸ������
 * @defgroup Game ��Ϸģ��
 *
 * �ṩ��Ϸ���ڵĴ���������Ϳ��ƹ��ܣ�
 * - ���������ٴ���
 * - ���ô������ԣ����⡢ͼ�ꡢ������ɫ�ȣ�
 * - ������Ϸ֡��
 */

#pragma once
#include "FloatApi.h"
#include "F_Console.h"

 /**
  * @class Game
  * @ingroup Game
  *
  * @brief �ṩ��Ϸ���ڵĴ���������Ϳ��ƹ���
  *
  * @note ʹ��ʾ����
  * @code
  * Game game;
  * game.CreateWindow(800, 600, "My Game");
  * game.Play(60);
  * @endcode
  */
class Game {
    int _fps; ///< ��Ϸ֡��
    int _w; ///< ���ڿ��
    int _h; ///< ���ڸ߶�
    char* _title; ///< ���ڱ���

    bool _load_icon; ///< �Ƿ����ͼ��
    bool _load_icon_pre; ///< �Ƿ�Ԥ����ͼ��
    int _icon_number; ///< ͼ������
    Image _icons[100]; ///< ͼ������
    FilePathList _icon_path; ///< ͼ��·���б�
    Color background_col; ///< ������ɫ
public:
    /**
     * @brief ���캯��
     */
    Game();

    /**
     * @brief ������Ϸ����
     * @param w ���ڿ��
     * @param h ���ڸ߶�
     * @param title ���ڱ���
     * @param debug �Ƿ�������ģʽ
     * @param flags ���ڱ�־
     */
    void CreateWindow(int w, int h, const char* title, bool debug = true, int flags = 8608);

    /**
     * @brief �ж���Ϸ�Ƿ���Կ�ʼ
     * @return ������Կ�ʼ����true�����򷵻�false
     */
    bool CanStart();

    /**
     * @brief ��ʼ��Ϸѭ��
     * @param fps ֡��
     */
    void Play(int fps);

    /**
     * @brief ������Ϸ֡��
     * @param fps ֡��
     */
    void SetFPS(int fps);

    /**
     * @brief ��ȡ��Ϸ֡��
     * @return ֡��
     */
    int GetFPS() const;

    /**
     * @brief ���ô�������
     * @param x x���꣨-1�����䣩
     * @param y y���꣨-1�����䣩
     */
    void Set_Window_Pos(int x = -1, int y = -1);

    /**
     * @brief ���ô���͸����
     * @param op ͸����
     */
    void Set_Window_Opacity(float op);

    /**
     * @brief ������Ϸ����
     */
    void Destroy();

    /**
     * @brief ���ñ�����ɫ
     * @param bk_col ������ɫ
     */
    void Set_Background_Color(Color bk_col);

    /**
     * @brief ���ÿ���̨ͼ��
     * @param id ͼ��ID
     */
    void Set_Console_Icon(int id);

    /**
     * @brief ���ô��ڱ���
     * @param title ���ڱ���
     */
    void Set_Title(const char* title);

    /**
     * @brief ���ô���ͼ��
     * @param icons ͼ������
     * @param n ͼ������
     */
    void Set_Icons(Image* icons, int n);

    /**
     * @brief ���ô���ͼ��
     * @param icon ͼ��
     */
    void Set_Icon(Image icon);

    /**
     * @brief Ԥ���ش���ͼ��
     * @param filePath ͼ���ļ�·��
     */
    void Set_Icon_Pre(const char* filePath);

    /**
     * @brief Ԥ���ش���ͼ��
     * @param filePathList ͼ���ļ�·���б�
     */
    void Set_Icons_Pre(FilePathList filePathList);
};

