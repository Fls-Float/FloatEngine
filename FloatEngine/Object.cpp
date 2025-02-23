#include "Object.h"
Object::Object()
{
	m_block = { 0,0 };
	m_angle = 0.0f;
	m_origin = { 0,0 };
	x = y = 0;
	_m_obj_name = "NONE";
	_m_obj_pro = OBJPRO.Normal;
	depth = 0;
	image_index = 0;
	sprite_index = new Texture[0];
	_ins_id = -1;
	for (int i = 0; i < ALARM_EVENT_COUNT; i++) {
		alarm_time_node[i] = -1;
	} 
	__object_alarm_clock_timer_ = 0; alarmEvents = {
		&Object::onAlarmEvent0,
		&Object::onAlarmEvent1,
		&Object::onAlarmEvent2,
		&Object::onAlarmEvent3,
		&Object::onAlarmEvent4,
		&Object::onAlarmEvent5,
		&Object::onAlarmEvent6,
		&Object::onAlarmEvent7,
		&Object::onAlarmEvent8,
		&Object::onAlarmEvent9,
		&Object::onAlarmEvent10
	};
}

Object::Object(const char* name, int ins_id,int pro)
{
	m_block = Size{ 0,0 };
	m_angle = 0.0f;
	x = y = 0;
	m_origin = { 0,0 };
	_m_obj_name = name;
	_m_obj_pro = pro;
	depth = 0;
	image_index = 0;
	sprite_index = new Texture[0];
	_ins_id = ins_id; 
	for (int i = 0; i < ALARM_EVENT_COUNT; i++) {
		alarm_time_node[i] = -1;
	}
	__object_alarm_clock_timer_ = 0; alarmEvents = {
		&Object::onAlarmEvent0,
		&Object::onAlarmEvent1,
		&Object::onAlarmEvent2,
		&Object::onAlarmEvent3,
		&Object::onAlarmEvent4,
		&Object::onAlarmEvent5,
		&Object::onAlarmEvent6,
		&Object::onAlarmEvent7,
		&Object::onAlarmEvent8,
		&Object::onAlarmEvent9,
		&Object::onAlarmEvent10
	};
}

Object::~Object()
{
	_m_obj_name.clear();
}

void Object::reset_alarm_clock()
{
	__object_alarm_clock_timer_ = 0;
}

void Object::show_object_alarm_clock_timer(Font font,float x, float y, Color col)
{
	F_Render::Draw_Text_Ex(font,TextFormat("timer=%d", __object_alarm_clock_timer_), x,y, 0, 0, 0, 0, 20, col);
}

void Object::Draw_Block(Color col)
{
	DrawRectanglePro({ x,y,m_block.width,m_block.height }, { m_block.width * m_origin.x,m_block.height * m_origin.y }, m_angle, col);
}
bool Object::Is_Meeting(float _x, float _y, const Object* other)
{
	F_Rectangle a, b;
	a.width = m_block.width;
	a.height = m_block.height;
	a.x = _x;
	a.y = _y;
	a.angle = m_angle;
	a.rot_origin = m_origin;
	a.Shape_Set();
	b.width = other->m_block.width;
	b.height = other->m_block.height;
	b.x = other->x;
	b.y = other->y;
	b.angle = other->m_angle;
	b.rot_origin = other->m_origin;
	b.Shape_Set();

	return floatapi_math::GJK_Collision(a, b);
}
void Object::onEvent_User(int index)
{
	switch (index) {
	case 0:
		break;
	case Collision_With:
		break;
	default:
		break;
	}
}
void Object::onEnter(){}
void Object::onUpdate(){}
void Object::onRenderBefore(){}
void Object::onRender(){}
void Object::onRenderNext(){}
void Object::onBeginCamera(){}
void Object::onEndCamera(){}
void Object::onAlarmEvent()
{
	__object_alarm_clock_timer_++;
	for (int index = 0; index < ALARM_EVENT_COUNT; index++) {
		if (alarm_time_node[index] != -1 &&
			__object_alarm_clock_timer_ == alarm_time_node[index]) {
			(this->*alarmEvents[index])();
		}
	}
}
void Object::onAlarmEvent0() {}
void Object::onAlarmEvent1() {}
void Object::onAlarmEvent2() {}
void Object::onAlarmEvent3() {}
void Object::onAlarmEvent4() {}
void Object::onAlarmEvent5() {}
void Object::onAlarmEvent6() {}
void Object::onAlarmEvent7() {}
void Object::onAlarmEvent8() {}
void Object::onAlarmEvent9() {}
void Object::onAlarmEvent10() {}
void Object::onDestroy(){}