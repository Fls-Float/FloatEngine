/**
 * @file Enum.h
 * @brief ö�ٺͳ�������
 * @defgroup Enum ö��ģ��
 *
 * �ṩ������ײ״̬���������Ժ���Ƶģʽ��ö�ٺͳ������壺
 * - ������
 * - ��ײ״̬ö��
 * - �������Գ���
 * - ��Ƶģʽö��
 */
#pragma once
 /**
  * @enum Collision
  * @ingroup Enum
  *
  * @brief ��ײ״̬ö��
  */
enum Collision {
    Collision_None = 0, ///< ����ײ
    Collision_With = 1, ///< ��ײ
    Collision_WithBlock = 2, ///< �뷽����ײ
    Collision_WithActor = 3, ///< ���ɫ��ײ
};

/**
 * @struct DIR
 * @ingroup Enum
 *
 * @brief ������
 */
struct {
    static constexpr float LEFT = 270.0f; ///< ��
    static constexpr float RIGHT = 90.0f; ///< ��
    static constexpr float UP = 0.0f; ///< ��
    static constexpr float DOWN = 180.0f; ///< ��
} DIR;

/**
 * @struct OBJPRO
 * @ingroup Enum
 *
 * @brief �������Գ���
 */
struct {
    static constexpr int Normal = 0; ///< ��ͨ����
    static constexpr int Actor = 1; ///< ��ɫ����
    static constexpr int Block = 2; ///< �������
} OBJPRO;

/**
 * @enum AudioMode
 * @ingroup Enum
 *
 * @brief ��Ƶģʽö��
 */
enum AudioMode {
    Audio_Unknown = 0, ///< δ֪��Ƶģʽ
    Audio_Sound = 1, ///< ����ģʽ
    Audio_Music = 2, ///< ����ģʽ
};
