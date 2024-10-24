#pragma once
#include "Object.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
static void EventInstance(Object* inst, const char* event);

struct object_cmp
{
	bool operator () (const Object& a, const Object& b)
	{
		return a.depth < b.depth;
	}
};
class Room
{
	std::vector<Object*> objects;
	void sort_objects();
public:
	bool auto_sort;
	Room();
	~Room();
	virtual Room *reset() { return new Room; }
	void RunInstance(const char* event);
	template<class T>
	inline T* CreateFromTemplate(T instance)
	{
		T* inst = new T;
		objects.push_back(inst);
		return inst;
	}
	void Create(Object* instance);
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
inline T* Create_InstanceTemplate(float x, float y, T instance,const char*name="NONE")
{
	Room* room = Room_Get_Now();
	T* t = new T;
	t->setObjName(name);
	t->onEnter();
	t->x = x;
	t->y = y;
	room->Create(t);
	return t;
}
Object* Create_Instance(float x, float y, Object* instance,const char*name);

void Destroy_Instance(const char* name, int num = 0, ...);
bool IsExist_Instance(const char* name);
int Count_Instance(const char* name);
int GetNum_Instance();
