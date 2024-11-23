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
	change_name("NONE_ACTOR");
}
Actor::~Actor()
{
}
