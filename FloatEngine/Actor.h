#pragma once
#include "Object.h"
#include "raymath.hpp"
/**
 * @file Actor.h
 * @brief ��ɫ��
 * @defgroup Actor ��ɫģ��
 *
 * �ṩ��ɫ�Ļ������Ժ���Ϊ��
 * - ��ɫ���ƶ�����Ծ������
 * - ��ɫ����������
 * - ע:��ʱ��δ���
 */

 /**
  * @class Actor
  * @ingroup Actor
  *
  * @brief �ṩ��ɫ�Ļ������Ժ���Ϊ
  *
  * @note ʹ��ʾ����
  * @code
  * Actor actor;
  * actor.active = true;
  * actor.gravity = FVec2(0, 9.8);
  * @endcode
  */
class Actor : public Object
{
protected:
	float _timer; ///< ��ʱ��
	float _time_scale; ///< ʱ������
public:
    /**
     * @brief ���캯��
     */
    Actor();

    /**
     * @brief ��������
     */
    ~Actor();

    bool active; ///< ��ɫ�Ƿ񼤻�
    bool moveable; ///< ��ɫ�Ƿ���ƶ�
    bool jumpable; ///< ��ɫ�Ƿ����Ծ
    bool isJump; ///< ��ɫ�Ƿ�����Ծ��
    float moveVelocity; ///< ��ɫ�ٶ�
    float moveDirection; ///< ��ɫ���� 

    FVec2 gravity; ///< ��ɫ������
    bool enableGravity; ///< �Ƿ���������
    
    void onUpdate() override; 
};
