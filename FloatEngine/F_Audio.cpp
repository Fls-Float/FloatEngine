#include "F_Audio.h"
#include <cmath>
#include <iostream>

// ----------------------------------------------------------------------------
// 构造函数/析构函数
// ----------------------------------------------------------------------------

F_Audio::F_Audio() {}

F_Audio::~F_Audio() {
    Unload();
}

// ----------------------------------------------------------------------------
// 加载/卸载
// ----------------------------------------------------------------------------

void F_Audio::Load(const char* filename, AudioMode mode) {
    Unload(); // 先卸载旧资源

    m_mode = mode;
    if (m_mode == AudioMode::SOUND) {
        m_sound = LoadSound(filename);
        if (m_sound.frameCount == 0) {
            std::cerr << "Error loading sound: " << filename << std::endl;
            m_mode = AudioMode::NONE;
        }
    }
    else if (m_mode == AudioMode::MUSIC) {
        m_music = LoadMusicStream(filename);
        if (m_music.frameCount == 0) {
            std::cerr << "Error loading music: " << filename << std::endl;
            m_mode = AudioMode::NONE;
        }
    }
}

void F_Audio::Unload() {
    if (m_mode == AudioMode::SOUND && m_sound.frameCount != 0) {
        UnloadSound(m_sound);
        m_sound = { 0 };
    }
    else if (m_mode == AudioMode::MUSIC && m_music.frameCount != 0) {
        UnloadMusicStream(m_music);
        m_music = { 0 };
    }
    m_mode = AudioMode::NONE;
}

// ----------------------------------------------------------------------------
// 播放/控制
// ----------------------------------------------------------------------------

void F_Audio::Play(bool loop) {
    if (m_mode == AudioMode::SOUND) {
        rlPlaySound(m_sound);
    }
    else if (m_mode == AudioMode::MUSIC) {
        m_music.looping = loop;
        PlayMusicStream(m_music);
    }
}

void F_Audio::Stop() {
    if (m_mode == AudioMode::SOUND) {
        rlStopSound(m_sound);
    }
    else if (m_mode == AudioMode::MUSIC) {
        StopMusicStream(m_music);
    }
}

// ----------------------------------------------------------------------------
// 空间音频核心逻辑
// ----------------------------------------------------------------------------

void F_Audio::UpdateSpatialPosition(Vector2 listenerPos) {
    if (m_mode == AudioMode::NONE) return;

    m_config.ListenerPos = listenerPos;

    // 1. 计算距离 (Distance)
    float distance = Vector2Distance(m_config.SourcePos, m_config.ListenerPos);

    // 2. 计算方向向量 (Direction Vector)
    Vector2 directionVector = Vector2Subtract(m_config.SourcePos, m_config.ListenerPos);

    // 3. 应用空间效果
    ApplySpatialEffect(distance, directionVector);

}

void F_Audio::ApplySpatialEffect(float distance, Vector2 directionVector) {
    // === 1. 音量衰减 (Volume Attenuation) ===
    float volume = m_config.BaseVolume;

    if (distance >= m_config.MaxDistance) {
        volume = 0.0f; // 超过最大距离，音量为零
    }
    else if (distance > 0.0f) {
        float attenuation = 1.0f - (distance / m_config.MaxDistance);
        volume *= attenuation;
    }

    // === 2. 声相 (Pan) 计算 ===
    float pan = 0.0f;
    if (distance > 0.0f) {
        Vector2 dirNorm = Vector2Normalize(directionVector);
        pan = dirNorm.x;
        // 降低中央声音的Pan，让它更集中
        pan *= 0.8f;
    }

    // === 3. 应用到 Raylib API ===
    if (m_mode == AudioMode::SOUND) {
        SetSoundVolume(m_sound, volume);
        SetSoundPan(m_sound, pan);
        SetSoundPitch(m_sound, 1.0f); // 保持不变
    }
    else if (m_mode == AudioMode::MUSIC) {
        SetMusicVolume(m_music, volume);
        SetMusicPan(m_music, pan);
        SetMusicPitch(m_music, 1.0f); // 保持不变
    }
}

// ----------------------------------------------------------------------------
// Setters
// ----------------------------------------------------------------------------

void F_Audio::SetSourcePosition(Vector2 pos) {
    m_config.SourcePos = pos;
}

void F_Audio::SetMaxDistance(float distance) {
    m_config.MaxDistance = distance;
    if (m_config.MaxDistance < 10.0f) m_config.MaxDistance = 10.0f; // 最小距离保护
}

void F_Audio::LoadFromSound(const Sound& sound) {
    Unload();
    m_sound = sound;
    m_mode = AudioMode::SOUND;
}

void F_Audio::LoadFromMusic(const Music& music) {
    Unload();
    m_music = music;
    m_mode = AudioMode::MUSIC;
}

