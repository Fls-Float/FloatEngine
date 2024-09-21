#include "Actor.h"
#include "Room.h"
void Actor::Event_User(int index)
{
	switch (index) {
	case Collision::Collision_WithBlock:{
		Met_Before = true;
		break;
	}
	case Collision_None:
		Met_Before = false;
		break;
	}
}



Actor::Actor()
{
	change_pro(OBJPRO.Actor);
	moveable = true;
	jumpable = false;
	active = false;
	gravity = {0.0f,-9.8f};
	Met_Before = Met_Timer = 0;
	isJump = false;
	change_name("NONE_ACTOR");
/*	b2BodyDef b2bdef;
	b2bdef.type = b2_dynamicBody;
	b2bdef.position.Set(x, y);
	b2bdef.angle = 0.0f;
	b2bdef.gravityScale = - gravity / 9.8f;
*/
}

Actor::~Actor()
{
	
}
