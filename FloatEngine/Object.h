#pragma once
#include "FloatApi.h"
#include <string>
#include "Enum.h"
class Object
{
	int _m_obj_pro;
	std::string _m_obj_name;
protected:
	void change_name(const char* name) {
		_m_obj_name = name;
	}
	void change_pro(int pro) {
		_m_obj_pro = pro;
	}
	void Draw_Block(Color col);
	void Block_Chase_Pos(float offset_x, float offset_y);
	bool Is_Meeting(float _x, float _y, Object* other);
public:
	int image_index;
	Texture *sprite_index;
	int depth;
	F_Rectangle m_block;
	float m_angle;
	Vector2 m_origin;
	float x;
	float y;
	Object();
	Object(const char* name,int pro=OBJPRO.Normal);
	~Object();
	inline void setObjName(const std::string& name) { _m_obj_name = name; }
	inline void setObjPro(const int& pro) { _m_obj_pro = pro; }
	inline const char* getObjName() const { return _m_obj_name.c_str(); }
	inline const int& getObjPro() const { return _m_obj_pro; }
	virtual void Event_User(int index);
	virtual void onEnter();
	virtual void onUpdate();
	virtual void onRenderBefore();
	virtual void onRender();
	virtual void onRenderNext();
	virtual void onDestroy();
};

inline bool Is_Collision(Object* o1, Object* o2) {
	F_Rectangle r1, r2;
	r1.Set_Rectangle(Rectangle{ o1->x,o1->y,o1->m_block.width,o1->m_block.height });
	r1.Set_Angle(o1->m_angle);
	r2.Set_Rectangle(Rectangle{ o2->x,o2->y,o2->m_block.width,o2->m_block.height });
	r2.Set_Angle(o2->m_angle);
	bool is = GJK_Collision(r1, r2);
	return is;
}


