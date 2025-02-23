#pragma once
#include "Object.h"

/**
 * @file Block.h
 * @brief ������
 * @defgroup Block ����ģ��
 *
 * �ṩ����Ļ������Ժ���Ϊ��
 * - ������¼�����
 * - ע:��ʱ��δ���
 */

 /**
  * @class Block
  * @ingroup Block
  *
  * @brief �ṩ����Ļ������Ժ���Ϊ
  *
  * @note ʹ��ʾ����
  * @code
  * Block block;
  * block.onEvent_User(0);
  * @endcode
  */
class Block : public Object
{
public:
    /**
     * @brief ���캯��
     */
    Block();

    /**
     * @brief ��������
     */
    ~Block();

    /**
     * @brief �û��¼�����
     * @param index �¼�����
     */
    virtual void onEvent_User(int index) override;
};
