#include "Actor.h"
#include "Room.h"
#include "Block.h"


Actor::Actor()
{
	change_pro(OBJPRO.Actor);
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

void Actor::UpdatePhysics(float deltaTime)
{
    if (enableGravity)
    {
        float timeInSeconds = deltaTime;

        // �����ʼ�ٶ�
        float initialVelocityX = cos(moveDirection * PI / 180) * moveVelocity;
        float initialVelocityY = sin(moveDirection * PI / 180) * moveVelocity;

        // �����µ�λ��
        float newX = initialVelocityX * timeInSeconds;
        float newY = initialVelocityY * timeInSeconds + 0.5f * gravity.y * timeInSeconds * timeInSeconds;

        // ���½�ɫ��λ��
        x += newX;
        y += newY;
    }
}



