#pragma once

#include "FMath.h"
#include "F_Easings.h"
class ANIM_TWEEN{
public:
	const static int LINEAR = 0,
		SINE = 1,
		QUAD = 2,
		CUBIC = 3,
		QUART = 4,
		QUINT = 5,
		EXPO = 6,
		CIRC = 7,
		BACK = 8,
		ELASTIC = 9,
		BOUNCE = 10,
		IN = 11,
		OUT = 12,
		IN_OUT = 13;
};

class ANIM_EASE{
public:
	const static int IN = 0,
		OUT = 1,
		IN_OUT = 2;
};

class F_Anim{	
	int _tween;
	int _ease;
	int _time;
	int _duration;
	float _start;
	float _change;
	int _delay;
	float _arg1;
	float _arg2;
	float* ft_value=nullptr;
	void _event();
	int _id;
	bool is_end = 0;
	std::string _anim_name;
public:
	void Init(float* _val, int tween, int ease, float start, float change, int duration, int delay = 0, float arg1 = 0, float arg2 = 0);
	void Start();
	int Get_ID();
	void Set_ID(int id);
	bool Is_End();
	std::string Get_Name();
	void Set_Name(std::string name);
};
void F_Anim_Create(float* _val, F_Anim anim);
void F_Anim_Create(float* _val, int tween, int ease, float start, float change, int duration, int delay = 0, float arg1 = 0, float arg2 = 0);
void F_Anim_Update();
void F_Anim_Delete(int id);
void F_Anim_Delete(std::string name);
//filter=0表示全部清除，filter=1表示清除结束的动画
void F_Anim_Clear(int filter=0);
F_Anim* F_Anim_Get(int id);
F_Anim* F_Anim_Get(std::string name);
//-1表示不存在
int F_Anim_Find(int id);
//-1表示不存在
int F_Anim_Find(std::string name);

//返回动画数量,filter=0表示全部，filter=1表示没结束动画
int F_Anim_Count(bool filter);