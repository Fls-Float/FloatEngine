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
	bool Is_Meeting(float _x, float _y, const Object* other);
public:
	int image_index;
	Texture* sprite_index;
	int depth;
	Size m_block;
	float m_angle;
	Vector2 m_origin;
	float x;
	float y;
	Object();
	Object(const char* name, int pro = OBJPRO.Normal);
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


