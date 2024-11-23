#include "Block.h"
#include "Enum.h"
Block::Block()
{
	m_block = { 0,0 };
	m_angle = 0;
	setObjPro(OBJPRO.Block);
	setObjName("NONE_BLOCK");
}
Block::~Block() {
	
}

void Block::Event_User(int index)
{
	switch (index) {
	case Collision_With: {
		//write your collision event

		break;
	}
	case Collision_None:
		//no collision
		break;
	}
}

