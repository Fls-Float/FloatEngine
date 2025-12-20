#pragma once

#include <raylib.h>
#include <raymath.h> // 引入 Raylib 的数学库 Vector2Distance, Vector2Normalize, etc.
#include <string>

// ----------------------------------------------------------------------------
// 1. 结构体和枚举
// ----------------------------------------------------------------------------

/**
 * @enum AudioMode
 * @brief 音频模式枚举，明确区分 Sound (非流式) 和 Music (流式)
 */
enum class AudioMode {
    NONE = 0,
    SOUND,  ///< 声音模式 (非流式，整个加载到内存)
    MUSIC   ///< 音乐模式 (流式，适用于长文件)
};

/**
 * @class F_Audio
 * @brief 提供 2D 空间音频定位 (Spatial Audio) 和管理功能
 *
 * 在 Raylib 基础上，通过手动计算音量(Volume)和声相(Pan)来实现 2D 空间效果。
 */
class F_Audio {
public:
    /**
     * @brief 空间音频配置结构体
     */
    struct SpatialConfig {
        Vector2  SourcePos = { 0.0f, 0.0f };    ///< 声源的绝对坐标
        Vector2  ListenerPos = { 0.0f, 0.0f };  ///< 听者的绝对坐标 (通常是玩家)
        float    MaxDistance = 500.0f;          ///< 声音能被听到的最大距离
        float    BaseVolume = 1.0f;             ///< 声音的基础音量 (0.0f ~ 1.0f)
    };

    F_Audio();
    ~F_Audio();

    // 禁用拷贝/移动，防止 Raylib 的内部 Sound/Music 资源被意外拷贝
    F_Audio(const F_Audio&) = delete;
    F_Audio& operator=(const F_Audio&) = delete;
    F_Audio(F_Audio&&) = default;
    F_Audio& operator=(F_Audio&&) = default;

    // ------------------------------------------------------------------------
    // 加载/卸载
    // ------------------------------------------------------------------------

    /**
     * @brief 从文件加载音频资源
     * @param filename 文件路径
     * @param mode 音频模式 (SOUND 或 MUSIC)
     */
    void Load(const char* filename, AudioMode mode);

    /**
     * @brief 卸载音频资源
     */
    void Unload();

    // ------------------------------------------------------------------------
    // 播放/控制
    // ------------------------------------------------------------------------

    /**
     * @brief 播放音频
     * @param loop 是否循环播放 (仅对 Music 有效)
     */
    void Play(bool loop = false);

    /**
     * @brief 停止播放
     */
    void Stop();

    /**
     * @brief 刷新空间音频定位 (必须每帧调用)
     * @param listenerPos 听者的当前位置
     */
    void UpdateSpatialPosition(Vector2 listenerPos);

    /**
	 * @brief 更新音频流 (仅对 Music 有效，必须每帧调用)
     */
    void UpdateMusic() const{
        if (m_mode == AudioMode::MUSIC) {
            ::UpdateMusicStream(m_music);
        }
	}
    // ------------------------------------------------------------------------
    // Setters/Getters
    // ------------------------------------------------------------------------

    /**
     * @brief 设置声源的绝对坐标
     */
    void SetSourcePosition(Vector2 pos);

    /**
     * @brief 设置声音能被听到的最大距离 (用于计算衰减)
     */
    void SetMaxDistance(float distance);

    /**
     * @brief 获取当前音频模式
     */
    AudioMode GetMode() const { return m_mode; }

    /**
     * @brief 获取声源位置
     */
    Vector2 GetSourcePosition() const { return m_config.SourcePos; }

    /**
     * @brief 从Sound加载 
    */
    void LoadFromSound(const Sound& sound);
    /**
     * @brief 从Music加载 
    */
    void LoadFromMusic(const Music& music);

    /*
	 * @brief 获取当前音频
     */
	Sound GetSound() { return m_sound; }

    /*
	 * @brief 获取当前音乐
     */
	Music GetMusic() { return m_music; }
private:
    AudioMode m_mode = AudioMode::NONE;
    SpatialConfig m_config;

    // Raylib 资源
    Sound m_sound = { 0 };
    Music m_music = { 0 };

    /**
     * @brief 计算并应用音量和声相 (Pan)
     * @param distance 声源与听者的距离
     * @param directionVector 声源相对于听者的方向向量
     */
    void ApplySpatialEffect(float distance, Vector2 directionVector);
};