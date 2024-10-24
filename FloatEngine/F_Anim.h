#pragma once

#include "FMath.h"
#include "F_Easings.h"
class F_Anim {
	float __timer;
	float *_x, *_y;
	float _x_start, _y_start;
	float _time;
	float _velocity;
    float (*_func)(float, float, float, float);
public:
	F_Anim();
	F_Anim(float& x, float& y, float v,float time);
	F_Anim(float* x, float* y, float v,float time);
    void setFunc(float (*func)(float, float, float, float));
    void start();
	void reset();
};