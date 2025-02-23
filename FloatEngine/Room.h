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
	int _counts;
public:
	Room();
	~Room();
	virtual Room *reset() { return new Room; }
	void RunInstance(const char* event);
	template<class T>
	inline T* CreateFromTemplate() {
		// ʹ�ô���ʵ�����п�������
		T* inst = new T;
		inst->_ins_id = _counts;
		objects.push_back(inst);
			// �����¶���
		_counts++;
		return static_cast<T*>(objects.back());
	}

	Object* Create(Object* instance);
	Object* AddIns(Object* instance);
	int FindOne(const char* name);
	int Find(int id);
	int GetCount(const char* name) const;
	int GetCount(int id) const;
	int Delete(const char*name,int num);
	int Delete(int index);
	int DeleteID(int id);
	int GetCount() const;
	auto GetObjList() { return objects; }
	std::string GetName(int id);
};

void Room_Goto(Room* room);
void Room_Set_Next(Room* nextRoom);
void Room_Goto_Next(Room* nextRoom);
void Room_Reset();
Room* Room_Get_Now();
const Room* Room_Get_NowC();
bool Room_Is_Ready();
int Room_Run_Now(const char*event);

template<class T>
inline T* Create_InstanceTemplate(float x, float y, const char* name = "NONE") {
	Room* room = Room_Get_Now();

	// ��������ʹ�ô���� instance ��ʼ��
	T* t = room->CreateFromTemplate<T>();
	// ���ö�������
	t->setObjName(name);
	t->onEnter();
	t->x = x;
	t->y = y;

	// ��������ӵ����䲢���ش洢��ָ��
	return t;
}

Object* Create_Instance(float x, float y, Object* instance,const char*name);
template<class T>
inline T* Create_InstanceTemplate(const char* name = "NONE") {
	Room* room = Room_Get_Now();

	// ��������ʹ�ô���� instance ��ʼ��
	T* t = room->CreateFromTemplate<T>();
	// ���ö�������
	t->setObjName(name);
	t->onEnter();

	// ��������ӵ����䲢���ش洢��ָ��
	return t;
}


Object* Create_Instance(Object* instance, const char* name);

int Destroy_Instance(const char* name, int num = 0);
int Destroy_Instance(int id);
bool IsExist_Instance(const char* name);
bool IsExist_Instance(int id);
int Count_Instance(const char* name);
int Count_Instance(int id);
int Count_Instance();
std::string Get_Instance_Name(int id);

//debug
void Show_Instance_Info();