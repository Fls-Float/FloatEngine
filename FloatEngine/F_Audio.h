#pragma once
#include <raylib.hpp>
#include <iostream>
#include "Enum.h"
class F_Audio
{
	AudioMode m_Audio_Mode; // 1 -- sound,2 -- music...
	Sound m_sound;
	
	Music m_music;
	float x; // ��Դx
	float y; // ��Դy
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
	// ��Դx
	void SetX(float _x);
	// ��Դy
	void SetY(float _y);
	// ��Դ����
	void SetPos(Vector2 pos);
	// Ŀ������
	void SetTarget(float _x_target, float _y_target);
	// Ŀ������
	void SetTarget(Vector2 _target);
	//��Դ����(0��~360��)
	void SetTargetDirection(float dir);
	//����
	void ResetAudioMode();
	//ˢ��
	void FlushAll();
};

