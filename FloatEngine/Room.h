#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "Object.h"    // 假设 Object.h 包含 Object 基类, getObjName/getObjPro, _ins_id
#include "FloatApi.h"  // 假设包含 DEBUG_LOG, TextFormat, WinFuns::MessageBox, TextIsEqual

// ----------------------------------------------------------------------------
// 异常和辅助函数声明
// ----------------------------------------------------------------------------

class InstanceException : public std::exception {
protected:
	std::string context;
public:
	InstanceException(const std::string& ctx);
	const char* what() const noexcept override;
};

class RoomException : public std::exception {
protected:
	std::string context;
public:
	RoomException(const std::string& ctx);
	const char* what() const noexcept override;
};

static void EventInstance(const std::shared_ptr<Object>& inst, const char* event);

// ----------------------------------------------------------------------------
// Room 类定义
// ----------------------------------------------------------------------------

class Room
{
private:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::weak_ptr<Object>> objects_persistent;
	std::unordered_map<int, bool> object_is_persistent;
	int _counts;

public:
	Room();
	virtual ~Room();

	// 工厂/重置
	virtual Room* reset() { return new Room; }

	// 核心逻辑
	void RunInstance(const char* event);

	/**
	 * @brief 完美转发模板：创建对象并初始化
	 */
	template<class T>
	inline std::shared_ptr<T> CreateFromTemplate(bool persistent = false) {
		auto inst = std::make_shared<T>();
		inst->_ins_id = _counts;
		objects.emplace_back(inst);
		if (persistent) {
			objects_persistent.emplace_back(std::weak_ptr<Object>(inst));
			object_is_persistent[inst->_ins_id] = true;
		}
		_counts++;
		// 必须返回 static_pointer_cast<T>，因为 objects.back() 是 shared_ptr<Object>
		return std::static_pointer_cast<T>(objects.back());
	}

	// 对象操作
	std::shared_ptr<Object> Create(Object* instance, bool persistent = false); // <--- 注意：此函数不安全，已在 cpp 中修正
	std::shared_ptr<Object> AddIns(std::shared_ptr<Object> instance, bool persistent = false);
	void AddRunningInstance(std::shared_ptr<Object> instance);

	// 查找
	int FindOne(const char* name);
	int Find(int id);
	int GetCount(const char* name) const;
	int GetCount(int id) const;
	std::string GetName(int id);

	// 删除
	int Delete(const char* name, int num);
	int Delete(int index);
	int DeleteID(int id);
	void DeleteAll();
	int DeletePersistent(const char* name, int num);
	int DeletePersistentID(int id);
	int GetCount() const;

	// 持久化
	/**
	 * @brief 将一个持久对象及其记录从当前房间转移到新房间
	 */
	void TransferPersistentObjects(std::shared_ptr<Object> obj, Room* newRoom);
	std::vector<std::shared_ptr<Object>> GetObjListPersistent();
	std::vector<std::shared_ptr<Object>> GetObjList() const;
	const std::vector<std::shared_ptr<Object>>& GetObjListRef() const {
		return objects;
	}
};

// ----------------------------------------------------------------------------
// 全局房间管理器函数声明
// ----------------------------------------------------------------------------
void Set_Room_Auto_Destroy(bool mode = true);
void Room_Goto(Room* room, bool keep_persistent = true);
void Room_Set_Next(Room* nextRoom);
void Room_Goto_Next(Room* nextRoom);
void Room_Reset();
Room* Room_Get_Now();
const Room* Room_Get_NowC();
bool Room_Is_Ready();
int Room_Run_Now(const char* event);

// 模板实例化函数 (为了保持代码整洁，只保留最通用的模板)
template<class T>
inline std::shared_ptr<T> Create_InstanceTemplate(float x, float y, const char* name = "NONE", bool persistent = false) {
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

template<class T>
inline std::shared_ptr<T> Create_InstanceTemplate(const char* name = "NONE", bool persistent = false) {
	Room* room = Room_Get_Now();
	if (!room) return nullptr;

	auto t = room->CreateFromTemplate<T>(persistent);
	if (t) {
		t->onEnter();
		t->setObjName(name);
	}
	return t;
}

std::shared_ptr<Object> Create_Instance(float x, float y, Object* instance, const char* name, bool persistent = false);
std::shared_ptr<Object> Create_Instance(Object* instance, const char* name, bool persistent = false);

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
void Show_Instance_Info();

