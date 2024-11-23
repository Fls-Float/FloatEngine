#pragma once
#include "Object.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
static void EventInstance(Object* inst, const char* event);

class Room
{
	std::vector<Object*> objects;
public:
	Room();
	~Room();
	virtual Room *reset() { return new Room; }
	void RunInstance(const char* event);
	template<class T>
	inline T* CreateFromTemplate(const T& instance) {
		// 使用传入实例进行拷贝构造
		T* inst = new T(instance);

		// 将新对象存储到对象容器
		objects.push_back(inst);

		// 返回新对象
		return inst;
	}

	Object* Create(Object* instance);
	int FindOne(const char* name);
	int GetNum(const char* name) const;
	int Delete(const char*name,int num);
	void Delete(int index);
	int GetAllNum() const;
	auto GetObjList() { return objects; }
};

static Room* _Game_Main_Room;
static bool _Game_Room_Is_Enter = false;
static Room* _Game_Next_Room;
void Room_Goto(Room* room);
void Room_Set_Next(Room* nextRoom);
void Room_Goto_Next(Room* nextRoom);
void Room_Reset();
Room* Room_Get_Now();
const Room* Room_Get_NowC();
bool Room_Is_Ready();
int Room_Run_Now(const char*event);

template<class T>
inline T* Create_InstanceTemplate(float x, float y, const T& instance, const char* name = "NONE") {
	Room* room = Room_Get_Now();

	// 创建对象并使用传入的 instance 初始化
	T* t = new T(instance);

	// 设置额外属性
	t->setObjName(name);
	t->onEnter();
	t->x = x;
	t->y = y;

	// 将对象添加到房间并返回存储的指针
	return room->Create(t);
}

Object* Create_Instance(float x, float y, Object* instance,const char*name);
template<class T>
inline T* Create_InstanceTemplate(const T& instance, const char* name = "NONE") {
	Room* room = Room_Get_Now();

	// 创建对象并使用传入的 instance 初始化
	T* t = new T(instance);

	// 设置额外属性
	t->setObjName(name);
	t->onEnter();

	// 将对象添加到房间并返回存储的指针
	return room->Create(t);
}

Object* Create_Instance(Object* instance, const char* name);

void Destroy_Instance(const char* name, int num = 0, ...);
bool IsExist_Instance(const char* name);
int Count_Instance(const char* name);
int GetNum_Instance();
