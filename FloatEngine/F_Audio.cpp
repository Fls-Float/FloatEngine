#include "F_Audio.h"
#include "FloatApi.h"
F_Audio::F_Audio()
{
	m_Audio_Mode = AudioMode::Audio_Unknown;
	m_sound = Sound();
	m_music = Music();
	x = 0.5f;
	y = 1.0f;
	target_direction = 0.0f;
	target = { 0,0 };
	start_direction_mode = false;
}
F_Audio::F_Audio(AudioMode Audio_Mode) {
	m_Audio_Mode = Audio_Mode;
	m_sound = Sound();
	m_music = Music();
	x = 0.5f;
	y = 1.0f;
	target_direction = 0.0f;
	target = { 0,0 };
	start_direction_mode = false;
}

void F_Audio::LoadFromFile(const char* filename,AudioMode am)
{
	m_Audio_Mode = am;
	switch (m_Audio_Mode) {
		case 0: {
			DEBUG_LOG(LOG_WARNING, "F_Audio:未知的audio_mode,请明确");
			break;
		}
		case 1: {
			m_sound = LoadSound(filename);
			break;
		}
		case 2: {
			m_music = LoadMusicStream(filename);
			break;
		}
		default:
			DEBUG_LOG(LOG_ERROR, "F_Audio:错误的audio_mode,请明确");
			break;
	}
}

void F_Audio::LoadFromSound(Sound snd)
{
	if (m_Audio_Mode == AudioMode::Audio_Sound) {
		m_sound = snd;
	}
}

void F_Audio::LoadFromMusic(Music mus)
{
	if (m_Audio_Mode == AudioMode::Audio_Music) {
		m_music = mus;
	}
}

Sound F_Audio::GetSound()
{
	if (m_Audio_Mode == AudioMode::Audio_Sound)
		return m_sound;
	else {
		DEBUG_LOG(LOG_WARNING, TextFormat("F_Audio:此时不是sound模式"), false);
		return Sound();
	}
}
Music F_Audio::GetMusic()
{
	if (m_Audio_Mode == AudioMode::Audio_Music)
		return m_music;
	else {
		DEBUG_LOG(LOG_WARNING, TextFormat("F_Audio:此时不是music模式"), false);
		return Music();
	}
}

AudioMode F_Audio::GetAudioMode()
{
	return m_Audio_Mode;
}

bool F_Audio::IsTheAudioMode(AudioMode am)
{
	return am==GetAudioMode();
}

void F_Audio::SetX(float _x)
{
	float pan = 0.5f;
	switch (m_Audio_Mode) {
	case AudioMode::Audio_Unknown:
		DEBUG_LOG(LOG_WARNING, TextFormat("F_Audio.SetX:未初始化的audio!"), false);
		break;
	case AudioMode::Audio_Sound:
		x = _x;
		if (x < target.x) {
			pan = 0.5 + Clamp(sqrt(target.x - x) * GetScreenWidth() / 32000, 0, 1);
		}
		else {
			pan = 0.5 - Clamp(sqrt(x - target.x) * GetScreenWidth() / 32000, 0, 1);
		}
		SetSoundPan(m_sound, pan);
		break;
	case AudioMode::Audio_Music:
		x = _x;
		if (x < target.x) {
			pan = pan + Clamp(sqrt(target.x - x) * GetScreenWidth() / 32000, 0, 1);
		}
		else {
			pan = pan - Clamp(sqrt(x - target.x) * GetScreenWidth() / 32000, 0, 1);
		}
		SetMusicPan(m_music, pan);
		break;
	default:
		DEBUG_LOG(LOG_ERROR, "F_Audio.SetX:错误的audio_mode,请明确");
		break;
	}
}

void F_Audio::SetY(float _y)
{
	float vol = 1.0f;
	float cha = 0.0f;
	float distance = 0.0f;
	float val = 0.0f;
	//vol = 0.001*distance
	switch (m_Audio_Mode) {
	case AudioMode::Audio_Unknown:
		DEBUG_LOG(LOG_WARNING, TextFormat("F_Audio.SetY:未初始化的audio!"), false);
		break;
	case AudioMode::Audio_Sound:
		y = _y;
		distance = Vector2Distance(Vector2{ x,y }, Vector2{ target.x,target.y });
		val = target_direction / 90;
		if (y < target.y) {
			if (target_direction > 180) {
				cha = 360 - target_direction;
			}
			else {
				cha = target_direction;
			}
		}
		else {
			if (target_direction > 180) {
				cha = target_direction - 180;
			}
			else {
				cha = target_direction;
			}
		}
		if(start_direction_mode)
			vol =1.0f - 0.0018 * distance - cha / 3600;
		else {
			vol = 1.0f - 0.0018 * distance;
		}
		SetSoundVolume(m_sound, vol);
		break;
	case AudioMode::Audio_Music:
		y = _y;
		distance = Vector2Distance(Vector2{ x,y }, Vector2{ target.x,target.y });
		val = target_direction / 90;
		if (y < target.y) {
			if (target_direction > 180) {
				cha = 360 - target_direction;
			}
			else {
				cha = target_direction;
			}
		}
		else {
			if (target_direction > 180) {
				cha = target_direction - 180;
			}
			else {
				cha = target_direction;
			}
		}
		if (start_direction_mode)
			vol = 1.0f - 0.0018 * distance - cha / 3600;
		else {
			vol = 1.0f - 0.0018 * distance;
		}
		DEBUG_LOG(
			
			LOG_DEBUG, TextFormat("F_Audio.SetY:distance=%0.1f,vol=%.6f", distance,vol), false);
		SetMusicVolume(m_music, vol);
		break;
	default:
		DEBUG_LOG(LOG_ERROR, "F_Audio.SetY:错误的audio_mode,请明确");
		break;
	}
}

void F_Audio::SetPos(Vector2 pos)
{
	SetX(pos.x);
	SetY(pos.y);
}

void F_Audio::SetTarget(float _x_target, float _y_target)
{
	target.x = _x_target;
	target.y = _y_target;
}

void F_Audio::SetTarget(Vector2 _target)
{
	SetTarget(_target.x, _target.y);
}

void F_Audio::SetTargetDirection(float dir)
{
	target_direction = fmod((dir+360),360);
}

void F_Audio::ResetAudioMode()
{
	SetMusicPan(m_music, 0.5f);
	SetMusicVolume(m_music, 1.0f);
	SetSoundPan(m_sound, 0.5f);
	SetSoundVolume(m_sound, 1.0f);
}

void F_Audio::FlushAll()
{
	SetX(x);
	SetY(y);
}


