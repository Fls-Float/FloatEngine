#include "F_Anim.h"

void F_Anim::_event()
{
	float& r = *ft_value;
	int t = _time;
	float b = _start;
	float c = _change;
	float d = _duration;
	float a = _arg1;
	float p = _arg2;
	float s = 0;
	switch (_tween) {
	case ANIM_TWEEN::LINEAR:
		r = (t / d) * c + b;
		break;

	case ANIM_TWEEN::SINE:
		switch (_ease) {
		case ANIM_EASE::IN:
			r = -c * cos(t / d * (PI / 2)) + c + b;
			break;
		case ANIM_EASE::OUT:
			r = c * sin(t / d * (PI / 2)) + b;
			break;
		case ANIM_EASE::IN_OUT:
			r = -c / 2 * (cos(PI * t / d) - 1) + b;
			break;
		}
		break;

	case ANIM_TWEEN::QUAD:
		switch (_ease) {
		case ANIM_EASE::IN:
			t /= d;
			r = c * t * t + b;
			break;
		case ANIM_EASE::OUT:
			t /= d;
			r = -c * t * (t - 2) + b;
			break;
		case ANIM_EASE::IN_OUT:
			t /= d / 2;
			if (t < 1) {
				r = c / 2 * t * t + b;
			}
			else {
				r = -c / 2 * ((--t) * (t - 2) - 1) + b;
			}
			break;
		}
		break;

	case ANIM_TWEEN::CUBIC:
		switch (_ease) {
		case ANIM_EASE::IN:
			t /= d;
			r = c * t * t * t + b;
			break;
		case ANIM_EASE::OUT:
			t = t / d - 1;
			r = c * (t * t * t + 1) + b;
			break;
		case ANIM_EASE::IN_OUT:
			t /= d / 2;
			if (t < 1) {
				r = c / 2 * t * t * t + b;
			}
			else {
				t -= 2;
				r = c / 2 * (t * t * t + 2) + b;
			}
			break;
		}
		break;

	case ANIM_TWEEN::QUART:
		switch (_ease) {
		case ANIM_EASE::IN:
			t /= d;
			r = c * t * t * t * t + b;
			break;
		case ANIM_EASE::OUT:
			t = t / d - 1;
			r = -c * (t * t * t * t - 1) + b;
			break;
		case ANIM_EASE::IN_OUT:
			t /= d / 2;
			if (t < 1) {
				r = c / 2 * t * t * t * t + b;
			}
			else {
				t -= 2;
				r = -c / 2 * (t * t * t * t - 2) + b;
			}
			break;
		}
		break;

	case ANIM_TWEEN::QUINT:
		switch (_ease) {
		case ANIM_EASE::IN:
			t /= d;
			r = c * t * t * t * t * t + b;
			break;
		case ANIM_EASE::OUT:
			t = t / d - 1;
			r = c * (t * t * t * t * t + 1) + b;
			break;
		case ANIM_EASE::IN_OUT:
			t /= d / 2;
			if (t < 1) {
				r = c / 2 * t * t * t * t * t + b;
			}
			else {
				t -= 2;
				r = c / 2 * (t * t * t * t * t + 2) + b;
			}
			break;
		}
		break;

	case ANIM_TWEEN::EXPO:
		switch (_ease) {
		case ANIM_EASE::IN:
			if (t == 0) {
				r = b;
			}
			else {
				r = c * pow(2, 10 * (t / d - 1)) + b;
			}
			break;
		case ANIM_EASE::OUT:
			if (t == d) {
				r = b + c;
			}
			else {
				r = c * (-pow(2, -10 * t / d) + 1) + b;
			}
			break;
		case ANIM_EASE::IN_OUT:
			if (t == 0) {
				r = b;
			}
			if (t == d) {
				r = b + c;
			}
			t /= d / 2;
			if (t < 1) {
				r = c / 2 * pow(2, 10 * (t - 1)) + b;
			}
			else {
				r = c / 2 * (-pow(2, -10 * --t) + 2) + b;
			}
			break;
		}
		break;

	case ANIM_TWEEN::CIRC:
		switch (_ease) {
		case ANIM_EASE::IN:
			t /= d;
			r = -c * (sqrt(1 - t * t) - 1) + b;
			break;
		case ANIM_EASE::OUT:
			t = t / d - 1;
			r = c * sqrt(1 - t * t) + b;
			break;
		case ANIM_EASE::IN_OUT:
			t /= d / 2;
			if (t < 1) {
				r = -c / 2 * (sqrt(1 - t * t) - 1) + b;
			}
			else {
				t -= 2;
				r = c / 2 * (sqrt(1 - t * t) + 1) + b;
			}
			break;
		}
		break;
	case ANIM_TWEEN::BACK:
		if (_arg1 > 0) {
			s = _arg1;
		}
		else {
			s = 1.70158;
		}
		switch (_ease) {
		case ANIM_EASE::IN:
			t /= d;
			r = c * t * t * ((s + 1) * t - s) + b;
			break;
		case ANIM_EASE::OUT:
			t = t / d - 1;
			r = c * (t * t * ((s + 1) * t + s) + 1) + b;
			break;
		case ANIM_EASE::IN_OUT:
			t /= d / 2;
			s *= 1.525;
			if (t < 1) {
				r = c / 2 * (t * t * ((s + 1) * t - s)) + b;
			}
			else {
				t -= 2;
				r = c / 2 * (t * t * ((s + 1) * t + s) + 2) + b;
			}
			break;
		}
		break;

	case ANIM_TWEEN::ELASTIC:
		a = _arg1;
		p = _arg2;
		switch (_ease) {
		case ANIM_EASE::IN:
			if (t == 0) {
				r = b;
				break;
			}
			t /= d;
			if (t == 1) {
				r = b + c;
				break;
			}
			if (p == 0) {
				p = d * 0.3;
			}
			if (a == 0 || a < abs(c)) {
				a = c;
				s = p / 4;
			}
			else {
				s = p / (2 * PI) * asin(c / a);
			}
			t -= 1;
			r = -(a * pow(2, 10 * t) * sin((t * d - s) * (2 * PI) / p)) + b;
			break;
		case ANIM_EASE::OUT:
			if (t == 0) {
				r = b;
				break;
			}
			t /= d;
			if (t == 1) {
				r = b + c;
				break;
			}
			if (p == 0) {
				p = d * 0.3;
			}
			s = 0;
			if (a == 0 || a < abs(c)) {
				a = c;
				s = p / 4;
			}
			else {
				s = p / (2 * PI) * asin(c / a);
			}
			r = a * pow(2, -10 * t) * sin((t * d - s) * (2 * PI) / p) + c + b;
			break;
		case ANIM_EASE::IN_OUT:
			if (t == 0) {
				r = b;
				break;
			}
			t /= d / 2;
			if (t == 2) {
				r = b + c;
				break;
			}
			if (p == 0) {
				p = d * (0.3 * 1.5);
			}
			s = 0;
			if (a == 0 || a < abs(c)) {
				a = c;
				s = p / 4;
			}
			else {
				s = p / (2 * PI) * asin(c / a);
			}
			t -= 1;
			if (t < 1) {
				r = -0.5 * (a * pow(2, 10 * t) * sin((t * d - s) * (2 * PI) / p)) + b;
			}
			else {
				r = a * pow(2, -10 * t) * sin((t * d - s) * (2 * PI) / p) * 0.5 + c + b;
			}
			break;
		}
		break;

	case ANIM_TWEEN::BOUNCE:
		switch (_ease) {
		case ANIM_EASE::IN:
			t = d - t;
			t /= d;
			if (t < 1 / 2.75) {
				r = c * (7.5625 * t * t);
			}
			else if (t < 2 / 2.75) {
				t -= 1.5 / 2.75;
				r = c * (7.5625 * t * t + 0.75);
			}
			else if (t < 2.5 / 2.75) {
				t -= 2.25 / 2.75;
				r = c * (7.5625 * t * t + 0.9375);
			}
			else {
				t -= 2.625 / 2.75;
				r = c * (7.5625 * t * t + 0.984375);
			}
			r = c - r + b;
			break;
		case ANIM_EASE::OUT:
			t /= d;
			if (t < 1 / 2.75) {
				r = c * (7.5625 * t * t) + b;
			}
			else if (t < 2 / 2.75) {
				t -= 1.5 / 2.75;
				r = c * (7.5625 * t * t + 0.75) + b;
			}
			else if (t < 2.5 / 2.75) {
				t -= 2.25 / 2.75;
				r = c * (7.5625 * t * t + 0.9375) + b;
			}
			else {
				t -= 2.625 / 2.75;
				r = c * (7.5625 * t * t + 0.984375) + b;
			}
			break;
		case ANIM_EASE::IN_OUT:
			bool ease_in;
			if (t < d / 2) {
				t *= 2;
				ease_in = true;
			}
			else {
				t *= 2;
				t -= d;
				ease_in = false;
			}

			t /= d;
			if (t < 1 / 2.75) {
				r = c * (7.5625 * t * t);
			}
			else if (t < 2 / 2.75) {
				t -= 1.5 / 2.75;
				r = c * (7.5625 * t * t + 0.75);
			}
			else if (t < 2.5 / 2.75) {
				t -= 2.25 / 2.75;
				r = c * (7.5625 * t * t + 0.9375);
			}
			else {
				t -= 2.625 / 2.75;
				r = c * (7.5625 * t * t + 0.984375);
			}

			if (ease_in) {
				r = r * 0.5 + b;
			}
			else {
				r = r * 0.5 + c * 0.5 + b;
			}
			break;
		}
		break;
	}
}

void F_Anim::Init(float* _val,  int tween, int ease, float start, float change, int duration, int delay, float arg1, float arg2)
{
	is_end = false;
	ft_value = _val;
	_tween = tween;
	_ease = ease;
	_start = start;
	_change = change;
	_duration = duration;
	_delay = delay;
	_arg1 = arg1;
	_arg2 = arg2;
}

void F_Anim::Start()
{
	if (is_end) return;
	if (_delay <= 0) {
		_time += 1;
		if (_time < _duration) {
			_event();
		}
		else {
			is_end = 1;
		}
	}
	else {
		_delay -= 1;
	}
}


std::string F_Anim::Get_Name()
{
	return _anim_name;
}

void F_Anim::Set_Name(std::string name)
{
	_anim_name = name;
}

int F_Anim::Get_ID()
{
	return _id;
}

void F_Anim::Set_ID(int id)
{
	_id = id;
}

bool F_Anim::Is_End()
{
	return is_end;
}

static std::vector<F_Anim> anims= std::vector<F_Anim>();
static int _anim_count=0;
void F_Anim_Create(float* _val, F_Anim anim)
{
	anims.push_back(anim);
	anim.Set_ID(_anim_count);
	_anim_count++;
}
void F_Anim_Create(float* _val, int tween, int ease, float start, float change, int duration, int delay, float arg1, float arg2)
{
	F_Anim anim;
	anim.Init(_val, tween, ease, start, change, duration, delay, arg1, arg2);
	anims.push_back(anim);
	anim.Set_ID(_anim_count);
	_anim_count++;
}

void F_Anim_Update()
{
	for (auto anim : anims)
	{
		anim.Start();
	}
}

void F_Anim_Delete(int id)
{
	for (int i = 0; i < anims.size(); i++)
	{
		if (anims[i].Get_ID() == id)
		{
			anims.erase(anims.begin() + i);
			break;
		}
	}
}

void F_Anim_Delete(std::string name)
{
	for (int i = 0; i < anims.size(); i++)
	{
		if (anims[i].Get_Name() == name)
		{
			anims.erase(anims.begin() + i);
			break;
		}
	}
	anims.shrink_to_fit();
}

void F_Anim_Clear(int filter)
{
	switch (filter) {
	case 0:
		anims.clear();
		break;
	case 1:
		for (int i = 0; i < anims.size(); i++)
		{
			if (anims[i].Is_End())
			{
				anims.erase(anims.begin() + i);
			}
		}
		break;
	default:
		break;
	}

	anims.shrink_to_fit();
	_anim_count = 0;
}

F_Anim* F_Anim_Get(int id)
{
	F_Anim* anim = nullptr;
	if (anims.size() == 0) return anim;
	if (id >= anims.size()) return anim;
	if (F_Anim_Find(id) == -1) return anim;
	for (auto a : anims)
	{
		if (a.Get_ID() == id)
		{
			anim = &a;
			break;
		}
	}
	return anim;
}

F_Anim* F_Anim_Get(std::string name)
{
	F_Anim* anim = nullptr;
	if (F_Anim_Find(name) == -1) return anim;
	for (auto a : anims)
	{
		if (a.Get_Name() == name)
		{
			anim = &a;
			break;
		}
	}
	return anim;
}

int F_Anim_Find(int id)
{
	int index = -1;
	for (int i = 0; i < anims.size(); i++)
	{
		if (anims[i].Get_ID() == id)
		{
			index = i;
			break;
		}
	}
	return index;
}

int F_Anim_Find(std::string name)
{
	int index = -1;
	for (int i = 0; i < anims.size(); i++)
	{
		if (anims[i].Get_Name() == name)
		{
			index = i;
			break;
		}
	}
	return index;
}

int F_Anim_Count(bool filter)
{
	if(filter==0)	
		return _anim_count;
	else {
		int t = 0;
		for (int i = 0; i < anims.size(); i++)
		{
			if (anims[i].Is_End()) t++;
		}
		return _anim_count - t;
	}
}
