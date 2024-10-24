#pragma once
#include "Object.h"
#include "raymath.hpp"
class Actor :
    public Object
{
    std::vector<class Block*> _blocks;
protected:
    bool Met_Before;
    int Met_Timer;
public:
    //构造，析构
    Actor();
    ~Actor();
    //mode
    bool active;
    bool moveable;
    bool jumpable;
    bool isJump;
    //values
    FVec2 gravity;
    void Add_New_Block(class Block* block);
    template<class BLOCK_T>
    void Add_New_Block(BLOCK_T block);
    void Delete_Back_Block();
    void Delete_Block(int index);
    void Move(float _x, float _y);
    //限制
    void Limit_Actor();
};

template<class BLOCK_T>
inline void Actor::Add_New_Block(BLOCK_T block)
{
    BLOCK_T* block = new BLOCK_T;
    _blocks.push_back(block);
}
