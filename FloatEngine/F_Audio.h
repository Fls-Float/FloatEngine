#pragma once
#include <raylib.h>
#include "Enum.h"
/**
 * @file F_Audio.h
 * @brief ��Ƶ��
 * @defgroup Audio ��Ƶģ��
 *
 * �ṩ��Ƶ�ļ��ء����ź͹����ܣ�
 * - ֧������������ģʽ
 * - ֧����Ƶλ�úͷ�������
 */

 /**
  * @class F_Audio
  * @ingroup Audio
  *
  * @brief �ṩ��Ƶ�ļ��ء����ź͹�����
  *
  * @note ʹ��ʾ����
  * @code
  * F_Audio audio;
  * audio.LoadFromFile("sound.wav", AudioMode::Audio_Sound);
  * audio.SetPos(320,240);
  * audio.SetTarget(0,0);
  * audio.Play();
  * @endcode
  */
class F_Audio {
    AudioMode m_Audio_Mode; // 1 -- sound,2 -- music...
    Sound m_sound;
    Music m_music;
    float x; // ��Դx
    float y; // ��Դy
    float target_direction;
    Vector2 target;
    bool start_direction_mode;
public:
    /**
     * @brief Ĭ�Ϲ��캯��
     */
    F_Audio();

    /**
     * @brief ���캯��
     * @param audio_mode ��Ƶģʽ
     */
    F_Audio(AudioMode audio_mode);

    /**
     * @brief ���ļ�������Ƶ
     * @param filename �ļ���
     * @param am ��Ƶģʽ
     */
    void LoadFromFile(const char* filename, AudioMode am);

    /**
     * @brief ��Sound���������Ƶ
     * @param snd Sound����
     */
    void LoadFromSound(Sound snd);

    /**
     * @brief ��Music���������Ƶ
     * @param mus Music����
     */
    void LoadFromMusic(Music mus);

    /**
     * @brief ��ȡSound����
     * @return Sound����
     */
    Sound GetSound();

    /**
     * @brief ��ȡMusic����
     * @return Music����
     */
    Music GetMusic();

    /**
     * @brief ��ȡ��Ƶģʽ
     * @return ��Ƶģʽ
     */
    AudioMode GetAudioMode();

    /**
     * @brief �ж���Ƶģʽ�Ƿ�ƥ��
     * @param am ��Ƶģʽ
     * @return ���ƥ�䷵��true�����򷵻�false
     */
    bool IsTheAudioMode(AudioMode am);

    /**
     * @brief ������Դx����
     * @param _x x����
     */
    void SetX(float _x);

    /**
     * @brief ������Դy����
     * @param _y y����
     */
    void SetY(float _y);

    /**
     * @brief ������Դ����
     * @param pos ����
     */
    void SetPos(Vector2 pos);

    /**
     * @brief ����Ŀ������
     * @param _x_target Ŀ��x����
     * @param _y_target Ŀ��y����
     */
    void SetTarget(float _x_target, float _y_target);

    /**
     * @brief ����Ŀ������
     * @param _target Ŀ������
     */
    void SetTarget(Vector2 _target);

    /**
     * @brief ������Դ����
     * @param dir ����0��~360�㣩
     */
    void SetTargetDirection(float dir);

    /**
     * @brief ������Ƶģʽ
     */
    void ResetAudioMode();

    /**
     * @brief ˢ����Ƶ
     */
    void FlushAll();
};
