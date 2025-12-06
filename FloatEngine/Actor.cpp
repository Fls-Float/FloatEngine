#include "Actor.h"
#include "Room.h"
#include "Block.h"


Actor::Actor()
{
	change_pro(F_ObjPro::OBJPRO::Actor);
	moveable = true;
	jumpable = false;
	active = true;
	gravity = {0.0f,-9.8f};
	isJump = false;
    enableGravity = false;
    moveDirection = 0;
    moveVelocity = 0;
	change_name("NONE_ACTOR");
}
Actor::~Actor()
{
}

void Actor::onUpdate()
{
    _timer;
    if (enableGravity)
    {
        float timeInSeconds = _timer*GetFrameTime()*_time_scale;

        // 计算初始速度
        float initialVelocityX = cos(moveDirection * PI / 180) * moveVelocity;
        float initialVelocityY = sin(moveDirection * PI / 180) * moveVelocity;

        // 计算新的位置
        float newX = initialVelocityX * timeInSeconds;
        float newY = initialVelocityY * timeInSeconds + 0.5f * gravity.y * timeInSeconds * timeInSeconds;

        // 更新角色的位置
        x += newX;
        y += newY;
    }
}



