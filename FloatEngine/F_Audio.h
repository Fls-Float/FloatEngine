#pragma once
#include <raylib.h>
#include "Enum.h"
/**
 * @file F_Audio.h
 * @brief 音频类
 * @defgroup Audio 音频模块
 *
 * 提供音频的加载、播放和管理功能：
 * - 支持声音和音乐模式
 * - 支持音频位置和方向设置
 */

 /**
  * @class F_Audio
  * @ingroup Audio
  *
  * @brief 提供音频的加载、播放和管理功能
  *
  * @note 使用示例：
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
    float x; // 声源x
    float y; // 声源y
    float target_direction;
    Vector2 target;
    bool start_direction_mode;
public:
    /**
     * @brief 默认构造函数
     */
    F_Audio();

    /**
     * @brief 构造函数
     * @param audio_mode 音频模式
     */
    F_Audio(AudioMode audio_mode);

    /**
     * @brief 从文件加载音频
     * @param filename 文件名
     * @param am 音频模式
     */
    void LoadFromFile(const char* filename, AudioMode am);

    /**
     * @brief 从Sound对象加载音频
     * @param snd Sound对象
     */
    void LoadFromSound(Sound snd);

    /**
     * @brief 从Music对象加载音频
     * @param mus Music对象
     */
    void LoadFromMusic(Music mus);

    /**
     * @brief 获取Sound对象
     * @return Sound对象
     */
    Sound GetSound();

    /**
     * @brief 获取Music对象
     * @return Music对象
     */
    Music GetMusic();

    /**
     * @brief 获取音频模式
     * @return 音频模式
     */
    AudioMode GetAudioMode();

    /**
     * @brief 判断音频模式是否匹配
     * @param am 音频模式
     * @return 如果匹配返回true，否则返回false
     */
    bool IsTheAudioMode(AudioMode am);

    /**
     * @brief 设置声源x坐标
     * @param _x x坐标
     */
    void SetX(float _x);

    /**
     * @brief 设置声源y坐标
     * @param _y y坐标
     */
    void SetY(float _y);

    /**
     * @brief 设置声源坐标
     * @param pos 坐标
     */
    void SetPos(Vector2 pos);

    /**
     * @brief 设置目标坐标
     * @param _x_target 目标x坐标
     * @param _y_target 目标y坐标
     */
    void SetTarget(float _x_target, float _y_target);

    /**
     * @brief 设置目标坐标
     * @param _target 目标坐标
     */
    void SetTarget(Vector2 _target);

    /**
     * @brief 设置声源方向
     * @param dir 方向（0°~360°）
     */
    void SetTargetDirection(float dir);

    /**
     * @brief 重置音频模式
     */
    void ResetAudioMode();

    /**
     * @brief 刷新音频
     */
    void FlushAll();
};
