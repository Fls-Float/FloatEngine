#pragma once
#include "Object.h"
#include "raymath.hpp"
class Actor :
    public Object
{
protected:
public:
    //¹¹Ôì£¬Îö¹¹
    Actor();
    ~Actor();
    //mode
    bool active;
    bool moveable;
    bool jumpable;
    bool isJump;
    //values
    FVec2 gravity;
    
};
