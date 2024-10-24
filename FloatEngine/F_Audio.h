#pragma once
#include <raylib.hpp>
#include <iostream>
#include "Enum.h"
class F_Audio
{
	AudioMode m_Audio_Mode; // 1 -- sound,2 -- music...
	Sound m_sound;
	
	Music m_music;
	float x; // 声源x
	float y; // 声源y
	float target_direction;
	Vector2 target;
	bool start_direction_mode;
public:
	F_Audio();
	F_Audio(AudioMode audio_mode);
	void LoadFromFile(const char*filename,AudioMode am);
	void LoadFromSound(Sound snd);
   	void LoadFromMusic(Music mus);
	Sound GetSound();
	Music GetMusic();
	AudioMode GetAudioMode();
	bool IsTheAudioMode(AudioMode am);
	// 声源x
	void SetX(float _x);
	// 声源y
	void SetY(float _y);
	// 声源坐标
	void SetPos(Vector2 pos);
	// 目标坐标
	void SetTarget(float _x_target, float _y_target);
	// 目标坐标
	void SetTarget(Vector2 _target);
	//声源方向(0°~360°)
	void SetTargetDirection(float dir);
	//重置
	void ResetAudioMode();
	//刷新
	void FlushAll();
};

