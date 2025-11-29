#pragma once
#include "Object.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <memory>
static void EventInstance(const std::shared_ptr<Object>& inst, const char* event);

class Room
{
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::weak_ptr<Object>> objects_persistent;  // weak_ptr to prevent circular refs
	int _counts;
public:
	Room();
	~Room();
	virtual Room *reset() { return new Room; }
	void RunInstance(const char* event);
	template<class T>
	inline std::shared_ptr<T> CreateFromTemplate(bool persistent= false) {
		auto inst = std::make_shared<T>();
		inst->_ins_id = _counts;
		objects.emplace_back(inst);
		if(persistent) {
			objects_persistent.emplace_back(std::weak_ptr<Object>(inst));
		}
		_counts++;
		return std::static_pointer_cast<T>(objects.back());
	}
	std::shared_ptr<Object> Create(Object* instance, bool persistent=false);
	std::shared_ptr<Object> AddIns(std::shared_ptr<Object> instance, bool persistent=false);
	int FindOne(const char* name);
	int Find(int id);
	int GetCount(const char* name) const;
	int GetCount(int id) const;
	int Delete(const char*name,int num);
	int Delete(int index);
	int DeleteID(int id);
	int GetCount() const;
	int DeletePersistent(const char* name, int num);
	int DeletePersistentID(int id);
	void AddRunningInstance(std::shared_ptr<Object> instance);
	// Transfer persistent objects to new room
	void TransferPersistentObjects(Room* newRoom);
	std::vector<std::shared_ptr<Object>> GetObjListPersistent();
	std::vector<std::shared_ptr<Object>> GetObjList() const;
	const std::vector<std::shared_ptr<Object>>& GetObjListRef() const {
		return objects;
	}
	std::string GetName(int id);
};

void Room_Goto(Room* room, bool keep_persistent=true);
void Room_Set_Next(Room* nextRoom);
void Room_Goto_Next(Room* nextRoom);
void Room_Reset();
Room* Room_Get_Now();
const Room* Room_Get_NowC();
bool Room_Is_Ready();
int Room_Run_Now(const char*event);

template<class T>
inline std::shared_ptr<T> Create_InstanceTemplate(float x, float y, const char* name = "NONE", bool persistent=false) {
	Room* room = Room_Get_Now();
	if (!room) return nullptr;

	auto t = room->CreateFromTemplate<T>(persistent);
	if (t) {
		t->onEnter();
		t->setObjName(name);
		t->x = x;
		t->y = y;
	}
	return t;
}

std::shared_ptr<Object> Create_Instance(float x, float y, Object* instance, const char* name, bool persistent=false);

template<class T>
inline std::shared_ptr<T> Create_InstanceTemplate(const char* name = "NONE", bool persistent=false) {
	Room* room = Room_Get_Now();
	if (!room) return nullptr;

	auto t = room->CreateFromTemplate<T>(persistent);
	if (t) {
		t->onEnter();
		t->setObjName(name);
	}
	return t;
}

std::shared_ptr<Object> Create_Instance(Object* instance, const char* name, bool persistent=false);

int Destroy_Instance(const char* name, int num = 0);
int Destroy_Persistent_Instance(const char* name, int num = 0);
int Destroy_Instance(int id);
int Destroy_Persistent_Instance(int id);
bool IsExist_Instance(const char* name);
bool IsExist_Instance(int id);
int Count_Instance(const char* name);
int Count_Instance(int id);
int Count_Instance();
std::string Get_Instance_Name(int id);

// debug
void Show_Instance_Info();