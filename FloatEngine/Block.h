#pragma once
#include "Object.h"

class Block :
    public Object
{
public:
    Block();
    ~Block();
    virtual void Event_User(int index) override;
};

