/**
 * @file Object.h
 * @brief ��Ϸ������
 * @defgroup Object ����ģ��
 *
 * �ṩ��Ϸ����Ĵ�����������¼������ܣ�
 * - �������Թ���
 * - �¼������û��¼��������¼��������¼�����Ⱦ�¼��ȣ�
 * - ��ʱ������
 */

#pragma once
#include "FloatApi.h"
#include <string>
#include "Enum.h"
#include <array>
#include <functional>

#define FOBJECT_BODY() float x, y; float image_index; Sprite sprite_index; int depth; Size m_block; float m_angle; Vector2 m_origin;float image_xscale;float image_yscale;float image_alpha;Color image_color;

 /**
  * @class Object
  * @ingroup Object
  *
  * @brief �ṩ��Ϸ����Ĵ�����������¼�������
  *
  * @note ʹ��ʾ����
  * @code
  * Object obj("Player", 1);
  * obj.onUpdate();
  * obj.onRender();
  * @endcode
  */
class Object {
    int _m_obj_pro; ///< ��������
    std::string _m_obj_name; ///< ��������
    int __object_alarm_clock_timer_; ///< ��ʱ��

protected:
    static const int ALARM_EVENT_COUNT = 11; ///< ��ʱ���¼�����

    /**
     * @brief ���ü�ʱ��
     */
    void reset_alarm_clock();

    std::array<void (Object::*)(), ALARM_EVENT_COUNT> alarmEvents; ///< ��ʱ���¼�����

    /**
     * @brief ��ʾ�����ʱ��
     * @param font ����
     * @param x x����
     * @param y y����
     * @param col ��ɫ
     */
    void show_object_alarm_clock_timer(Font font, float x, float y, Color col);

    int alarm_time_node[ALARM_EVENT_COUNT]; ///< ��ʱ���ڵ�����

    /**
     * @brief �ı��������
     * @param name ������
     */
    void change_name(const char* name) {
        _m_obj_name = name;
    }

    /**
     * @brief �ı��������
     * @param pro ������
     */
    void change_pro(int pro) {
        _m_obj_pro = pro;
    }

    /**
     * @brief ���ƶ����
     * @param col ��ɫ
     */
    void Draw_Block(Color col);

    /**
     * @brief �ж϶����Ƿ�����
     * @param _x x����
     * @param _y y����
     * @param other ��һ������
     * @return �����������true�����򷵻�false
     */
    bool Is_Meeting(float _x, float _y, const Object* other);

    /**
	* @brief ��������
    */
    void Draw_Self();
public:
    int _ins_id; ///< ʵ��ID
    FOBJECT_BODY()

    /**
        * @brief ���캯��
    */
    Object();

    /**
     * @brief ���캯��
     * @param name ��������
     * @param ins_id ʵ��ID
     * @param pro ��������
     */
    Object(const char* name, int ins_id, int pro = OBJPRO.Normal);
  
    /**
      * @brief �ƶ����캯��
      * @param other Ҫ�ƶ��Ķ���
     */
    Object(Object&& other) noexcept;

    /**
     * @brief �������캯��
     * @param other Ҫ�����Ķ���
     */
    Object(const Object& other) ;

    /**
     * @brief ������ֵ�����
     * @param other Ҫ�����Ķ���
     * @return ������Ķ�������
     */
    Object& operator=(const Object& other);

    /**
     * @brief ��������
     */
    ~Object();

    /**
     * @brief ���ö�������
     * @param name ��������
     */
    inline void setObjName(const std::string& name) { _m_obj_name = name; }

    /**
     * @brief ���ö�������
     * @param pro ��������
     */
    inline void setObjPro(const int& pro) { _m_obj_pro = pro; }

    /**
     * @brief ��ȡ��������
     * @return ��������
     */
    inline const char* getObjName() const { return _m_obj_name.c_str(); }

    /**
     * @brief ��ȡ��������
     * @return ��������
     */
    inline const int& getObjPro() const { return _m_obj_pro; }

    /**
     * @brief �û��¼�����
     * @param index �¼�����
     */
    virtual void onEvent_User(int index);

    /**
     * @brief �����¼�����
     */
    virtual void onEnter();

    /**
     * @brief �����¼�����
     */
    virtual void onUpdate();

    /**
     * @brief ��Ⱦǰ�¼�����
     */
    virtual void onRenderBefore();

    /**
     * @brief ��Ⱦ�¼�����
     */
    virtual void onRender();

    /**
     * @brief ��Ⱦ���¼�����
     */
    virtual void onRenderNext();

    /**
     * @brief �������ʼ�¼�����
     */
    virtual void onBeginCamera();

    /**
     * @brief ����������¼�����
     */
    virtual void onEndCamera();

    /**
     * @brief ��ȡ��ʱ���¼�����
     * @return ��ʱ���¼�����
     */
    int getAlarmCount() const {
        return ALARM_EVENT_COUNT;
    }

    /**
     * @brief ��ʱ���¼�����
     */
    void onAlarmEvent();

    /**
     * @brief ��ʱ���¼�0����
     */
    virtual void onAlarmEvent0();

    /**
     * @brief ��ʱ���¼�1����
     */
    virtual void onAlarmEvent1();

    /**
     * @brief ��ʱ���¼�2����
     */
    virtual void onAlarmEvent2();

    /**
     * @brief ��ʱ���¼�3����
     */
    virtual void onAlarmEvent3();

    /**
     * @brief ��ʱ���¼�4����
     */
    virtual void onAlarmEvent4();

    /**
     * @brief ��ʱ���¼�5����
     */
    virtual void onAlarmEvent5();

    /**
     * @brief ��ʱ���¼�6����
     */
    virtual void onAlarmEvent6();

    /**
     * @brief ��ʱ���¼�7����
     */
    virtual void onAlarmEvent7();

    /**
     * @brief ��ʱ���¼�8����
     */
    virtual void onAlarmEvent8();

    /**
     * @brief ��ʱ���¼�9����
     */
    virtual void onAlarmEvent9();

    /**
     * @brief ��ʱ���¼�10����
     */
    virtual void onAlarmEvent10();

    /**
     * @brief �����¼�����
     */
    virtual void onDestroy();
};


