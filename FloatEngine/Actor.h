#pragma once
#include "Object.h"
#include "raymath.hpp"
typedef bool(*ActorMoveable)();
typedef bool(*ActorIsOnWall)();
class Actor :
    public Object
{
protected:
    bool Met_Before;
    int Met_Timer;
    //用户事件
    void Event_User(int index) override;

public:
    //mode
    bool active;
    bool moveable;
    bool jumpable;
    bool isJump;
    //values
    
    
    Vector2 gravity;
    
    //构造，析构
    Actor();
    ~Actor();
};

