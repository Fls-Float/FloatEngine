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
	Met_Before = Met_Timer = 0;
	isJump = false;
	change_name("NONE_ACTOR");
	_blocks = std::vector<Block*>();
}
Actor::~Actor()
{
	for (auto b : _blocks) {
		b->onDestroy();
		delete b;
	}
}

void Actor::Add_New_Block(Block* block)
{
	_blocks.push_back(block);
}

void Actor::Delete_Back_Block()
{
	Block* a = _blocks.back();
	_blocks.pop_back();
	delete a;
}

void Actor::Delete_Block(int index)
{
	Block* a = _blocks.at(index);
	_blocks.erase(_blocks.begin() + index);
	delete a;
}

void Actor::Move(float _x, float _y)
{
	float x_ed = x;
	float y_ed = y;
	x += _x;
	y += _y;

	float dx = x_ed - x;
	float dy = y_ed - y;
	float value = (dx * dx) + (dy * dy);

	float dist = sqrtf(value);
	float x_add =  dx / dist;
	float y_add =  dy / dist ;
	for (auto b : _blocks)
	{
		while (Is_Meeting(x, y,b)) {
			x += x_add;
			y += y_add;
		}
	}
}


void Actor::Limit_Actor()
{
	
}
