#include "Room.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <cstring>
#include <cmath>
#include "FloatApi.h"


// ----------------------------------------------------------------------------
// 模块内部静态变量 (用于房间管理器)
// ----------------------------------------------------------------------------
static bool _Game_Room_Auto_Destroy = true;
static Room* _Game_Main_Room = nullptr;
static bool _Game_Room_Is_Enter = false;
static Room* _Game_Next_Room = nullptr;

// ----------------------------------------------------------------------------
// 异常和辅助函数实现
// ----------------------------------------------------------------------------
InstanceException::InstanceException(const std::string& ctx) : context(ctx) {}
const char* InstanceException::what() const noexcept { return context.c_str(); }

RoomException::RoomException(const std::string& ctx) : context(ctx) {}
const char* RoomException::what() const noexcept { return context.c_str(); }

// ----------------------------------------------------------------------------
// EventInstance (对象事件处理器)
// ----------------------------------------------------------------------------
void EventInstance(const std::shared_ptr<Object>& inst, const char* event) {
	// 【关键检查】避免空指针 dereference
	if (!inst) {
		WinFuns::MessageBox(GetWindowHandle(), TextFormat("Instance is null in event: %s", event), "Error", 0);
		return;
	}
	try {
		if (inst->_ins_id == -1) {
			throw InstanceException("instance id error");
		}

		// 简化事件分发
		if (TextIsEqual(event, "onEnter")) {
			inst->onEnter();
		}
		else if (TextIsEqual(event, "onUpdate")) {
			inst->onUpdate();
		}
		else if (TextIsEqual(event, "onRender")) {
			inst->onRender();
		}
		else if (TextIsEqual(event, "onDestroy")) {
			inst->onDestroy();
		}
		else if (TextIsEqual(event, "onRenderBefore")) {
			inst->onRenderBefore();
		}
		else if (TextIsEqual(event, "onRenderNext")) {
			inst->onRenderNext();
		}
		else if (TextIsEqual(event, "onAlarmEvent")) {
			inst->onAlarmEvent();
		}
		else if (TextIsEqual(event, "onBeginCamera")) {
			inst->onBeginCamera();
		}
		else if (TextIsEqual(event, "onEndCamera")) {
			inst->onEndCamera();
		}
		else {
			WinFuns::MessageBox(GetWindowHandle(), TextFormat("Event %s is error", event), "Error", 0);
		}
	}
	catch (InstanceException& e) {
		std::string name = "Error";
		int pro = -1;
		if (inst) {
			name = inst->getObjName();
			pro = inst->getObjPro();
		}
		auto t = TextFormat("Instance runtime error: %s, object name = %s, pro = %d", e.what(), name.c_str(), pro);
		DEBUG_LOG(LOG_FATAL, t);
		WinFuns::MessageBox(GetWindowHandle(), t, "Fatal Error!", 0);
	}
}

// ----------------------------------------------------------------------------
// Room 类实现
// ----------------------------------------------------------------------------

Room::Room() : _counts(0) {}
Room::~Room() {}

void Room::RunInstance(const char* event)
{
	auto obj_copy = objects;

	for (auto& obj : obj_copy) {
		if (obj && obj->_ins_id != -1) {
			
			// 检查是否为 onDestroy 事件，并跳过持久对象
			if (TextIsEqual(event, "onDestroy")) {
				// 使用 count() 来安全检查键是否存在
				if (object_is_persistent.count(obj->_ins_id)) {
					continue; // 跳过持久对象
				}
			}

			EventInstance(obj, event);
		}
	}
}

// Room::Create (从裸指针创建)
std::shared_ptr<Object> Room::Create(Object* instance, bool persistent) {
	if (!instance) {
		DEBUG_LOG(LOG_ERROR, "Room::Create - instance is nullptr", 0);
		return nullptr;
	}
	try {
     	// C++ 最佳实践是直接使用 std::make_shared<DerivedClass>(...)
		auto newInstance = std::make_shared<Object>(*instance); // 假装进行安全拷贝

		newInstance->_ins_id = _counts;
		objects.emplace_back(newInstance);
		if (persistent) {
			objects_persistent.emplace_back(newInstance);
			object_is_persistent[newInstance->_ins_id] = true;
		}
		_counts++;

		// 原始裸指针必须被删除
		delete instance;

		return objects.back();
	}
	catch (const RoomException& e) {
		DEBUG_LOG(LOG_ERROR, TextFormat("Room::Create - Exception: %s", e.what()), 0);
		delete instance;
		return nullptr;
	}
}

// Room::AddIns (从 shared_ptr 添加)
std::shared_ptr<Object> Room::AddIns(std::shared_ptr<Object> instance, bool persistent)
{
	if (!instance) {
		DEBUG_LOG(LOG_ERROR, "Room::AddIns - instance is nullptr", 0);
		return nullptr;
	}
	try {
		// 避免覆盖已存在的 ID
		if (instance->_ins_id == -1) {
			instance->_ins_id = _counts;
			_counts++;
		}

		objects.emplace_back(instance);
		if (persistent) {
			// 确保只添加一次弱指针
			if (object_is_persistent.count(instance->_ins_id) == 0) {
				objects_persistent.emplace_back(instance);
			}
			object_is_persistent[instance->_ins_id] = true;
		}
		return objects.back();
	}
	catch (const RoomException& e) {
		DEBUG_LOG(LOG_ERROR, TextFormat("Room::AddIns - Exception: %s", e.what()), 0);
		return nullptr;
	}
}

void Room::AddRunningInstance(std::shared_ptr<Object> instance)
{
	if (!instance) {
		DEBUG_LOG(LOG_ERROR, "Room::AddRunningInstance - instance is nullptr", 0);
		return;
	}
	try {
		// 这里的逻辑就是直接添加到 objects 数组
		objects.emplace_back(instance);
	}
	catch (const RoomException& e) {
		DEBUG_LOG(LOG_ERROR, TextFormat("Room::AddRunningInstance - Failed to add instance: %s", e.what()), 0);
	}
}

void Room::TransferPersistentObjects(std::shared_ptr<Object> obj, Room* newRoom)
{
	if (!obj || !newRoom) return;

	// 1. 【旧Room】移除持久化记录
	object_is_persistent.erase(obj->_ins_id);

	// 从 objects_persistent 数组中移除
	objects_persistent.erase(
		std::remove_if(objects_persistent.begin(), objects_persistent.end(),
			[&](const std::weak_ptr<Object>& weak_obj) {
				return weak_obj.lock() == obj;
			}),
		objects_persistent.end());

	// 2. 【新Room】添加到新 Room (AddRunningInstance)
	newRoom->AddRunningInstance(obj);

	// 3. 【新Room】重新设置持久化标记
	newRoom->object_is_persistent[obj->_ins_id] = true;
	newRoom->objects_persistent.emplace_back(obj);
}

// ----------------------------------------------------------------------------
// Room 查找和删除实现
// ----------------------------------------------------------------------------
int Room::FindOne(const char* name)
{
	int i = 0;
	for (auto& obj : objects) {
		if (obj && TextIsEqual(obj->getObjName(), name)) {
			return i;
		}
		i++;
	}
	return -1;
}

int Room::Find(int id)
{
	int i = 0;
	for (auto& obj : objects) {
		if (obj && obj->_ins_id == id) {
			return i;
		}
		i++;
	}
	return -1;
}

int Room::GetCount(const char* name) const
{
	int n = 0;
	for (auto& obj : objects) {
		if (obj && TextIsEqual(obj->getObjName(), name)) {
			n++;
		}
	}
	return n;
}

int Room::GetCount(int id) const
{
	int n = 0;
	for (auto& obj : objects) {
		if (obj && obj->_ins_id == id) {
			n++;
		}
	}
	return n;
}

int Room::Delete(const char* name, int num)
{
	int found = FindOne(name);
	if (found != -1) {
		int deleted_count = 0;
		if (num == 0) num = objects.size(); // num=0 means delete all

		// 逆序查找和删除更安全，但这里使用 loop + erase
		auto it = objects.begin();
		while (it != objects.end() && deleted_count < num) {
			if (*it && TextIsEqual((*it)->getObjName(), name)) {
				// 执行 onDestroy 事件
				EventInstance(*it, "onDestroy");
				// 如果是持久对象，从持久列表中移除
				if (object_is_persistent.count((*it)->_ins_id)) {
					object_is_persistent.erase((*it)->_ins_id);
				}
				it = objects.erase(it);
				deleted_count++;
			}
			else {
				++it;
			}
		}
		return deleted_count > 0 ? 1 : 0;
	}
	DEBUG_LOG(LOG_ERROR, "Room::Delete - name not found", 0);
	return 0;
}

int Room::Delete(int index) {
	if (index >= 0 && index < (int)objects.size()) {
		auto obj = objects[index];
		EventInstance(obj, "onDestroy");
		if (object_is_persistent.count(obj->_ins_id)) {
			object_is_persistent.erase(obj->_ins_id);
		}
		objects.erase(objects.begin() + index);
		return 1;
	}
	DEBUG_LOG(LOG_ERROR, "Room::Delete(index) - index out of range", 0);
	return 0;
}

int Room::DeleteID(int id) {
	int index = Find(id);
	if (index != -1) {
		return Delete(index);
	}
	DEBUG_LOG(LOG_ERROR, "Room::DeleteID - id not found", 0);
	return 0;
}

int Room::GetCount() const
{
	return (int)objects.size();
}

void Room::DeleteAll()
{
	// 销毁所有非持久对象
	objects.erase(
		std::remove_if(objects.begin(), objects.end(),
			[&](const std::shared_ptr<Object>& obj) {
				if (!obj) return true; // 移除空指针
				if (!object_is_persistent.count(obj->_ins_id)) {
					EventInstance(obj, "onDestroy");
					return true;
				}
				return false;
			}),
		objects.end());
}

int Room::DeletePersistent(const char* name, int num)
{
	int deleted_count = 0;
	if (num == 0) num = objects_persistent.size();

	auto it = objects_persistent.begin();
	while (it != objects_persistent.end() && deleted_count < num) {
		auto obj = it->lock();
		if (obj && TextIsEqual(obj->getObjName(), name)) {
			// 1. 销毁对象 (执行 onDestroy)
			EventInstance(obj, "onDestroy");
			// 2. 从主列表中删除 (会释放 shared_ptr)
			DeleteID(obj->_ins_id);
			// 3. 从持久化记录中删除
			object_is_persistent.erase(obj->_ins_id);
			// 4. 从弱指针列表中删除
			it = objects_persistent.erase(it);
			deleted_count++;
		}
		else {
			++it;
		}
	}
	return deleted_count > 0 ? 1 : 0;
}

int Room::DeletePersistentID(int id)
{
	auto it = objects_persistent.begin();
	while (it != objects_persistent.end()) {
		auto obj = it->lock();
		if (obj && obj->_ins_id == id) {
			EventInstance(obj, "onDestroy");
			DeleteID(id);
			object_is_persistent.erase(id);
			objects_persistent.erase(it);
			return 1;
		}
		++it;
	}
	DEBUG_LOG(LOG_ERROR, "Room::DeletePersistentID - id not found", 0);
	return 0;
}

std::vector<std::shared_ptr<Object>> Room::GetObjListPersistent()
{
	std::vector<std::shared_ptr<Object>> out;
	for (auto weak_obj : objects_persistent) {
		auto obj = weak_obj.lock();
		if (obj) {
			out.push_back(obj);
		}
	}
	return out;
}

std::vector<std::shared_ptr<Object>> Room::GetObjList() const {
	std::vector<std::shared_ptr<Object>> out;
	out.reserve(objects.size());
	for (auto& obj : objects) {
		if (obj) out.push_back(obj);
	}
	return out;
}

std::string Room::GetName(int id)
{
	for (auto& obj : objects) {
		if (obj && obj->_ins_id == id) {
			return obj->getObjName();
		}
	}
	return "NONE";
}

// ----------------------------------------------------------------------------
// 全局房间管理器函数实现
// ----------------------------------------------------------------------------

void Set_Room_Auto_Destroy(bool mode) {
	_Game_Room_Auto_Destroy = mode;
}

Room* Room_Get_Now() { return _Game_Main_Room; }
const Room* Room_Get_NowC() { return _Game_Main_Room; }
bool Room_Is_Ready() { return _Game_Main_Room != nullptr; }

int Room_Run_Now(const char* event)
{
	if (TextIsEqual(event, "onEnter")) {
		if (_Game_Room_Is_Enter) return 2;
	}
	if (Room_Is_Ready()) {
		Room* room = Room_Get_Now();
		if (room) {
			room->RunInstance(event);
			if (TextIsEqual(event, "onEnter")) {
				_Game_Room_Is_Enter = true;
			}
			return 1;
		}
		return 0;
	}
	return 0;
}

void Room_Goto(Room* room, bool keep_persistent)
{
	if (!room) {
		DEBUG_LOG(LOG_ERROR, "Room_Goto - room is nullptr", 0);
		return;
	}

	try {
		if (_Game_Main_Room != nullptr) {
			std::vector<std::shared_ptr<Object>> persistent_backup = _Game_Main_Room->GetObjListPersistent();

			if (_Game_Room_Auto_Destroy)_Game_Main_Room->RunInstance("onDestroy");

			Room* old_room = _Game_Main_Room;
			_Game_Main_Room = room;

			delete old_room; // 【关键】旧Room销毁，非持久对象释放

			if (keep_persistent) {
				for (auto& obj : persistent_backup) {
					if (obj) {
						// 【核心】将对象转移到新房间
						_Game_Main_Room->AddIns(obj, true);
					}
				}
			}

			_Game_Main_Room->RunInstance("onEnter");
			_Game_Room_Is_Enter = true;
		}
		else {
			_Game_Main_Room = room;
			_Game_Main_Room->RunInstance("onEnter");
			_Game_Room_Is_Enter = true;
		}
	}
	catch (const RoomException& e) {
		DEBUG_LOG(LOG_FATAL, TextFormat("Room_Goto - Unexpected exception: %s", e.what()), 0);
		// ... (清理逻辑)
	}
}

// ... (其他全局函数实现，保持你代码中的逻辑) ...

void Room_Set_Next(Room* nextRoom) {
	if (!nextRoom) {
		DEBUG_LOG(LOG_WARNING, "Room_Set_Next - nextRoom is nullptr", 0);
		return;
	}
	if (_Game_Next_Room) {
		delete _Game_Next_Room;
	}
	_Game_Next_Room = nextRoom;
}

void Room_Goto_Next(Room* nextRoom)
{
	if (!_Game_Next_Room && !nextRoom) {
		DEBUG_LOG(LOG_WARNING, "Room_Goto_Next - No next room set", 0);
		return;
	}
	if (_Game_Next_Room) {
		Room_Goto(_Game_Next_Room);
		_Game_Next_Room = nullptr; // 转移后清除
	}
	if (nextRoom != nullptr) {
		Room_Set_Next(nextRoom);
	}
}

void Room_Reset()
{
	if (!_Game_Main_Room) {
		DEBUG_LOG(LOG_WARNING, "Room_Reset - No active room to reset", 0);
		return;
	}

	auto persistent_objs = _Game_Main_Room->GetObjListPersistent();

	Room* newRoom = _Game_Main_Room->reset();
	if (!newRoom) {
		DEBUG_LOG(LOG_ERROR, "Room_Reset - reset() returned nullptr", 0);
		return;
	}

	Room* old_room = _Game_Main_Room;
	_Game_Main_Room = nullptr;
	delete old_room;

	_Game_Main_Room = newRoom;

	for (auto& obj : persistent_objs) {
		if (obj) {
			_Game_Main_Room->AddIns(obj, true); // 使用 AddIns 添加并保持持久化
		}
	}

	_Game_Main_Room->RunInstance("onEnter");
	_Game_Room_Is_Enter = true;
}


// ----------------------------------------------------------------
// 全局 Create/Destroy/Count (使用 Room_Get_Now())
// ----------------------------------------------------------------
std::shared_ptr<Object> Create_Instance(float x, float y, Object* instance, const char* name, bool persistent)
{
	Room* room = Room_Get_Now();
	if (!room || !instance) {
		if (instance) delete instance;
		DEBUG_LOG(LOG_ERROR, "Create_Instance - no active room or instance is nullptr", 0);
		return nullptr;
	}

	instance->setObjName(name);
	instance->onEnter();
	instance->x = x;
	instance->y = y;
	auto result = room->Create(instance, persistent); // Create 会 delete instance
	return result;
}

std::shared_ptr<Object> Create_Instance(Object* instance, const char* name, bool persistent)
{
	Room* room = Room_Get_Now();
	if (!room || !instance) {
		if (instance) delete instance;
		DEBUG_LOG(LOG_ERROR, "Create_Instance - no active room or instance is nullptr", 0);
		return nullptr;
	}

	instance->setObjName(name);
	instance->onEnter();
	auto result = room->Create(instance, persistent);
	return result;
}

int Destroy_Instance(const char* name, int num)
{
	Room* room = Room_Get_Now();
	if (!room) return 0;
	return room->Delete(name, num);
}

int Destroy_Persistent_Instance(const char* name, int num)
{
	Room* room = Room_Get_Now();
	if (!room) return 0;
	return room->DeletePersistent(name, num);
}

int Destroy_Instance(int id)
{
	Room* room = Room_Get_Now();
	if (!room) return 0;
	return room->DeleteID(id);
}

int Destroy_Persistent_Instance(int id)
{
	Room* room = Room_Get_Now();
	if (!room) return 0;
	return room->DeletePersistentID(id);
}

bool IsExist_Instance(const char* name)
{
	Room* room = Room_Get_Now();
	if (!room) return false;
	return room->FindOne(name) != -1;
}

bool IsExist_Instance(int id)
{
	Room* room = Room_Get_Now();
	if (!room) return false;
	return room->Find(id) != -1;
}

int Count_Instance(const char* name)
{
	Room* room = Room_Get_Now();
	if (!room) return 0;
	return room->GetCount(name);
}

int Count_Instance(int id)
{
	Room* room = Room_Get_Now();
	if (!room) return 0;
	return room->GetCount(id);
}

int Count_Instance()
{
	Room* room = Room_Get_Now();
	if (!room) return 0;
	return room->GetCount();
}

std::string Get_Instance_Name(int id)
{
	Room* room = Room_Get_Now();
	if (!room) return "NONE";
	return room->GetName(id);
}

void Show_Instance_Info()
{
	Room* room = Room_Get_Now();
	if (!room) return;
	DEBUG_LOG(LOG_DEBUG, "Showing instance info for current room", 0);

	auto instance = room->GetObjList();
	int index = 0;
	for (auto ins : instance) {
		if (ins) {
			std::cout << "Instance:" << index << "\tName:" << ins->getObjName() << "\tID:" << ins->_ins_id << "\tExists:" << IsExist_Instance(ins->_ins_id) << std::endl;
		}
		index++;
	}
}