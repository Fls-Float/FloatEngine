#include "F_Anim.h"

F_Anim::F_Anim()
{
    _time = 0;
    _velocity = 0;
    *_x = *_y =  0;
    __timer = 0;
    _x_start = _y_start = 0;
    _func = F_Easings::EaseNone;
}

F_Anim::F_Anim(float& x, float& y, float v, float time)
{
    _x = &x;
    _y = &y;
    _velocity = v;
    _time = time;
    _func = F_Easings::EaseNone;
    _x_start = x;
    _y_start = y;
}

F_Anim::F_Anim(float* x, float* y, float v,float time)
{
    _x = x;
    _y = y;
    _velocity = v;
    _time = time;
    _func = F_Easings::EaseNone;
    _x_start = *x;
    _y_start = *y;
}

void F_Anim::setFunc(float(*func)(float, float, float, float))
{
    _func = func;
}



void F_Anim::start()
{
    float value_x = _func(__timer, _x_start, _velocity, _time);
    float value_y = _func(__timer, _y_start, _velocity, _time);
    _x = &value_x;
    _y = &value_y;
    __timer++;
}

void F_Anim::reset()
{
    __timer = 0;
    *_x = _x_start;
    *_y = _y_start;
}

