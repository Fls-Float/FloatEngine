#include "Room.h"
class InstanceException : public std::exception {
protected:
	std::string context;
public:
	InstanceException(const std::string& ctx) : context(ctx) {}
	const char* what() const noexcept override {
		return context.c_str();
	}
};

void EventInstance(const std::shared_ptr<Object>& inst, const char* event) {
	using namespace WinFuns;
	void* hWnd = GetWindowHandle();
	if (!inst) {
		MessageBox(hWnd, TextFormat("Instance error in event: %s", event), "Error", 0);
		return;
	}
	try {
		if (inst->_ins_id==-1) {
			throw InstanceException("instance id error");
		}
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
			MessageBox(hWnd, TextFormat("Event %s is error", event), "Error", 0);
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
		MessageBox(hWnd, t, "Fatal Error!", 0);
	}
}

Room::Room() : _counts(0) {}

Room::~Room() {
	objects.clear();
	objects_persistent.clear();
}

void Room::RunInstance(const char* event)
{
	// Iterate over a copy to handle deletions during iteration
	auto obj_copy = GetObjListRef();
	for (auto& obj : obj_copy) {
		if (obj && obj->_ins_id != -1) {
			EventInstance(obj, event);
		}
	}	
}

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
		if (num == 0) {
			while ((found = FindOne(name)) != -1) {
				objects[found]->onDestroy();
				objects.erase(objects.begin() + found);
			}
		}
		else {
			int n = num;
			while ((found = FindOne(name)) != -1) {
				if (n == 0) break;
				objects[found]->onDestroy();
				objects.erase(objects.begin() + found);
				n--;
			}
			if (n != 0) return 0;
		}
		return 1;
	}
	DEBUG_LOG(LOG_ERROR, "Room::Delete - name not found or invalid index", 0);
	return 0;
}

int Room::GetCount() const
{
	return (int)objects.size();
}

int Room::DeletePersistent(const char* name, int num)
{
	int index = -1;
	int idx = 0;
	for (auto weak_obj : objects_persistent) {
		auto obj = weak_obj.lock();
		if (obj && TextIsEqual(obj->getObjName(), name)) {
			index = idx;
			break;
		}
		idx++;
	}
	if (index != -1) {
		objects_persistent.erase(objects_persistent.begin() + index);
		return 1;
	}
	DEBUG_LOG(LOG_ERROR, "Room::DeletePersistent - name not found", 0);
	return 0;
}

int Room::DeletePersistentID(int id)
{
	int index = -1;
	int ti = 0;
	for (auto weak_obj : objects_persistent) {
		auto obj = weak_obj.lock();
		if (obj && obj->_ins_id == id) {
			index = ti;
			break;
		}
		ti++;
	}
	if (index != -1) {
		auto obj = objects_persistent[index].lock();
		if (obj) {
			obj->onDestroy();
		}
		objects_persistent.erase(objects_persistent.begin() + index);
		return 1;
	}
	DEBUG_LOG(LOG_ERROR, "Room::DeletePersistentID - id not found", 0);
	return 0;
}

void Room::AddRunningInstance(std::shared_ptr<Object> instance)
{
	if (!instance) {
		DEBUG_LOG(LOG_ERROR, "Room::AddRunningInstance - instance is nullptr", 0);
		return;
	}
	try {
		instance->_ins_id = _counts;
		objects.emplace_back(instance);
		_counts++;
	}
	catch (const std::exception& e) {
		DEBUG_LOG(LOG_ERROR, TextFormat("Room::AddRunningInstance - Failed to add instance: %s", e.what()), 0);
	}
}

void Room::TransferPersistentObjects(Room* newRoom)
{
	if (!newRoom) {
		DEBUG_LOG(LOG_ERROR, "Room::TransferPersistentObjects - newRoom is nullptr", 0);
		return;
	}
	// Create copies of persistent objects
	std::vector<std::shared_ptr<Object>> persistent_copy;
	for (auto weak_obj : objects_persistent) {
		auto obj = weak_obj.lock();
		if (obj) {
			try {
				auto new_obj = std::make_shared<Object>(*obj);
				persistent_copy.push_back(new_obj);
			}
			catch (const std::exception& e) {
				DEBUG_LOG(LOG_ERROR, TextFormat("Room::TransferPersistentObjects - Failed to copy object: %s", e.what()), 0);
			}
		}
	}
	// Add to new room
	for (auto& obj : persistent_copy) {
		newRoom->AddRunningInstance(obj);
	}
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

std::string Room::GetName(int id)
{
	for (auto& obj : objects) {
		if (obj && obj->_ins_id == id) {
			return obj->getObjName();
		}
	}
	return "NONE";
}

int Room::Delete(int index) {
	if (index >= 0 && index < (int)objects.size()) {
		objects[index]->onDestroy();
		objects.erase(objects.begin() + index);
		return 1;
	}
	DEBUG_LOG(LOG_ERROR, "Room::Delete(index) - index out of range", 0);
	return 0;
}

int Room::DeleteID(int id) {
	int index = Find(id);
	if (index != -1) {
		objects[index]->onDestroy();
		objects.erase(objects.begin() + index);
		return 1;
	}
	DEBUG_LOG(LOG_ERROR, "Room::DeleteID - id not found", 0);
	return 0;
}

std::shared_ptr<Object> Room::Create(Object* instance, bool persistent) {
	if (!instance) {
		DEBUG_LOG(LOG_ERROR, "Room::Create - instance is nullptr", 0);
		return nullptr;
	}
	try {
		// Create shared_ptr from raw object
		auto newInstance = std::make_shared<Object>(std::move(*instance));
		newInstance->_ins_id = _counts;
		objects.emplace_back(newInstance);
		if (persistent) {
			objects_persistent.emplace_back(newInstance);
		}
		_counts++;
		delete instance;  // Clean up the source object
		return objects.back();
	}
	catch (const std::exception& e) {
		DEBUG_LOG(LOG_ERROR, TextFormat("Room::Create - Exception: %s", e.what()), 0);
		delete instance;
		return nullptr;
	}
}

std::shared_ptr<Object> Room::AddIns(std::shared_ptr<Object> instance, bool persistent)
{
	if (!instance) {
		DEBUG_LOG(LOG_ERROR, "Room::AddIns - instance is nullptr", 0);
		return nullptr;
	}
	try {
		instance->_ins_id = _counts;
		_counts++;
		objects.emplace_back(instance);
		if (persistent) {
			// Avoid duplicate entries
			bool already_persistent = false;
			for (auto weak_obj : objects_persistent) {
				auto obj = weak_obj.lock();
				if (obj == instance) {
					already_persistent = true;
					break;
				}
			}
			if (!already_persistent) {
				objects_persistent.emplace_back(instance);
			}
		}
		return objects.back();
	}
	catch (const std::exception& e) {
		DEBUG_LOG(LOG_ERROR, TextFormat("Room::AddIns - Exception: %s", e.what()), 0);
		return nullptr;
	}
}

std::vector<std::shared_ptr<Object>> Room::GetObjList() const {
	std::vector<std::shared_ptr<Object>> out;
	out.reserve(objects.size());
	for (auto& obj : objects) {
		out.push_back(obj);
	}
	return out;
}

static Room* _Game_Main_Room = nullptr;
static bool _Game_Room_Is_Enter = false;
static Room* _Game_Next_Room = nullptr;

void Room_Goto(Room* room, bool keep_persistent)
{
	if (!room) {
		DEBUG_LOG(LOG_ERROR, "Room_Goto - room is nullptr", 0);
		return;
	}

	try {
		if (_Game_Main_Room != nullptr) {
			// Backup persistent objects
			auto persistent_backup = _Game_Main_Room->GetObjListPersistent();

			// Destroy old room's objects
			_Game_Main_Room->RunInstance("onDestroy");

			// Delete old room
			Room* old_room = _Game_Main_Room;
			_Game_Main_Room = nullptr;
			delete old_room;

			// Switch to new room
			_Game_Main_Room = room;

			// Initialize new room
			try {
				_Game_Main_Room->RunInstance("onEnter");
			}
			catch (const std::exception& e) {
				DEBUG_LOG(LOG_ERROR, TextFormat("Room_Goto - Failed to initialize new room: %s", e.what()), 0);
			}

			// Transfer persistent objects
			if (keep_persistent) {
				for (size_t i = 0; i < persistent_backup.size(); ++i) {
					auto obj = persistent_backup[i];
					if (!obj) {
						DEBUG_LOG(LOG_WARNING, TextFormat("Room_Goto - Persistent object at index %zu is nullptr", i), 0);
						continue;
					}
					try {
						_Game_Main_Room->AddRunningInstance(obj);
					}
					catch (const std::exception& e) {
						DEBUG_LOG(LOG_ERROR, TextFormat("Room_Goto - Failed to add persistent object: %s", e.what()), 0);
					}
				}
			}

			_Game_Room_Is_Enter = true;
		}
		else {
			_Game_Main_Room = room;
			try {
				_Game_Main_Room->RunInstance("onEnter");
			}
			catch (const std::exception& e) {
				DEBUG_LOG(LOG_ERROR, TextFormat("Room_Goto - Failed to initialize first room: %s", e.what()), 0);
				_Game_Main_Room = nullptr;
				delete room;
				return;
			}
			_Game_Room_Is_Enter = true;
		}
	}
	catch (const std::exception& e) {
		DEBUG_LOG(LOG_FATAL, TextFormat("Room_Goto - Unexpected exception: %s", e.what()), 0);
	}
}

void Room_Set_Next(Room* nextRoom) {
	try {
		if (!nextRoom) {
			DEBUG_LOG(LOG_WARNING, "Room_Set_Next - nextRoom is nullptr", 0);
			return;
		}
		if (_Game_Next_Room) {
			delete _Game_Next_Room;
		}
		_Game_Next_Room = nextRoom;
	}
	catch (const std::exception& e) {
		DEBUG_LOG(LOG_ERROR, TextFormat("Room_Set_Next - Exception: %s", e.what()), 0);
	}
}

void Room_Goto_Next(Room* nextRoom)
{
	try {
		if (!_Game_Next_Room && !nextRoom) {
			DEBUG_LOG(LOG_WARNING, "Room_Goto_Next - No next room set", 0);
			return;
		}
		if (_Game_Next_Room) {
			Room_Goto(_Game_Next_Room);
		}
		if (nextRoom != nullptr) {
			Room_Set_Next(nextRoom);
		}
	}
	catch (const std::exception& e) {
		DEBUG_LOG(LOG_ERROR, TextFormat("Room_Goto_Next - Exception: %s", e.what()), 0);
	}
}

void Room_Reset()
{
	if (!_Game_Main_Room) {
		DEBUG_LOG(LOG_WARNING, "Room_Reset - No active room to reset", 0);
		return;
	}

	try {
		auto persistent_objs = _Game_Main_Room->GetObjListPersistent();

		Room* newRoom = nullptr;
		try {
			newRoom = _Game_Main_Room->reset();
		}
		catch (const std::exception& e) {
			DEBUG_LOG(LOG_ERROR, TextFormat("Room_Reset - Failed to create new room: %s", e.what()), 0);
			return;
		}

		if (!newRoom) {
			DEBUG_LOG(LOG_ERROR, "Room_Reset - reset() returned nullptr", 0);
			return;
		}

		Room* old_room = _Game_Main_Room;
		_Game_Main_Room = nullptr;
		delete old_room;

		_Game_Main_Room = newRoom;

		for (auto& obj : persistent_objs) {
			if (!obj) {
				DEBUG_LOG(LOG_WARNING, "Room_Reset - Persistent object is nullptr", 0);
				continue;
			}
			try {
				_Game_Main_Room->AddRunningInstance(obj);
			}
			catch (const std::exception& e) {
				DEBUG_LOG(LOG_ERROR, TextFormat("Room_Reset - Failed to add persistent object: %s", e.what()), 0);
			}
		}

		_Game_Room_Is_Enter = false;
	}
	catch (const std::exception& e) {
		DEBUG_LOG(LOG_FATAL, TextFormat("Room_Reset - Unexpected exception: %s", e.what()), 0);
	}
}

Room* Room_Get_Now()
{
	return _Game_Main_Room;
}

const Room* Room_Get_NowC()
{
	return _Game_Main_Room;
}

bool Room_Is_Ready()
{
	return _Game_Main_Room != nullptr;
}

int Room_Run_Now(const char* event)
{
	if (TextIsEqual(event, "onEnter")) {
		if (_Game_Room_Is_Enter) {
			return 2;
		}
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
		else {
			return 0;
		}
	}
	return 0;
}

std::shared_ptr<Object> Create_Instance(float x, float y, Object* instance, const char* name, bool persistent)
{
	Room* room = Room_Get_Now();
	if (!room) {
		DEBUG_LOG(LOG_ERROR, "Create_Instance - no active room", 0);
		delete instance;
		return nullptr;
	}
	if (!instance) {
		DEBUG_LOG(LOG_ERROR, "Create_Instance - instance is nullptr", 0);
		return nullptr;
	}
	instance->setObjName(name);
	instance->onEnter();
	instance->x = x;
	instance->y = y;
	auto result = room->Create(instance, persistent);
	if (!result) {
		DEBUG_LOG(LOG_ERROR, "Create_Instance - room->Create failed", 0);
	}
	return result;
}

std::shared_ptr<Object> Create_Instance(Object* instance, const char* name, bool persistent)
{
	Room* room = Room_Get_Now();
	if (!room) {
		DEBUG_LOG(LOG_ERROR, "Create_Instance - no active room", 0);
		delete instance;
		return nullptr;
	}
	if (!instance) {
		DEBUG_LOG(LOG_ERROR, "Create_Instance - instance is nullptr", 0);
		return nullptr;
	}
	instance->setObjName(name);
	instance->onEnter();
	auto result = room->Create(instance, persistent);
	if (!result) {
		DEBUG_LOG(LOG_ERROR, "Create_Instance - room->Create failed", 0);
	}
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
	int i = 0;
	for (auto obj : room->GetObjList()) {
		if (obj && obj->_ins_id == id) {
			i++;
		}
	}
	return i;
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
