#include "Room.h"
void EventInstance(Object* inst, const char* event) {
	using namespace WinFuns;
	void* hWnd = GetWindowHandle();
	if (!inst) {
		MessageBox(hWnd, TextFormat("Instance %d Is Error In %s", inst,event), "Error", 0);
	}
	if (inst == nullptr) return;
	if (inst == (void*)(0)) return;
	try {
		if (inst->_ins_id==-1) {
			throw std::exception("instance id error");
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
			MessageBox(hWnd, TextFormat("Event %s Is Error", event), "Error", 0);
		}
	}
	catch (std::exception& e) {
		std::string name = "Error";
		int pro = -1;
		if (inst) {
			name = inst->getObjName();
			pro = inst->getObjPro();
		}
		DEBUG_LOG(LOG_FATAL, TextFormat("实例运行错误:%s  ,  object name = %s   , pro = %d", e.what(),name.c_str(), pro));
	}
}

Room::Room()
{
	_counts = 0;
}

Room::~Room() {
	for (auto obj : objects) {
		delete obj;
	}
	objects.clear();
}



void Room::RunInstance(const char* event)
{
	for (auto obj : objects) {
		if (obj && obj->_ins_id != -1) {
			EventInstance(obj, event);
		}
	}	
}

int Room::FindOne(const char* name)
{
	int i = 0;
	for (auto obj : objects) {
		if (TextIsEqual(obj->getObjName(), name)) {
			return i;
		}
		i++;
	}
	return -1;
}

int Room::Find(int id)
{
	int i = 0;
	for (auto obj : objects) {
		if (obj->_ins_id == id) {
			return i;
		}
		i++;
	}
	return -1;
}

int Room::GetCount(const char* name) const
{
	int n = 0;
	for (auto obj : objects) {
		if (TextIsEqual(obj->getObjName(), name)) {
			n++;
		}
	}
	return n;
}

int Room::GetCount(int id) const
{
	int n = 0;
	for (auto obj : objects) {
		if (obj->_ins_id == id) {
			n++;
		}
	}
	return n;
}

int Room::Delete(const char* name,int num)
{
	if (FindOne(name) != -1) {
		if (num == 0) {
			while (FindOne(name) != -1) {
				objects[FindOne(name)]->onDestroy();
				objects.erase(objects.begin() + FindOne(name));
			}
		}
		else {
			int n = num;
			while (FindOne(name) != -1) {
				if (n == 0) break;
				objects[FindOne(name)]->onDestroy();
				objects.erase(objects.begin() + FindOne(name));
				n--;
			}
			if (n != 0) return 0;
		}
		return 1;
	}
	DEBUG_LOG(LOG_ERROR, "Room:在Delete(int index)函数中index不在合法范围内", 0);
	return 0;
}
int Room::GetCount() const
{
	return (int)objects.size();
}
std::string Room::GetName(int id)
{
	for (auto obj : objects) {
		if (obj->_ins_id == id) {
			return obj->getObjName();
		}
	}
	return "NONE";
}
int Room::Delete(int index) {
	if (index >= 0 && index < objects.size()) {
		objects[index]->onDestroy();
		delete objects[index]; // 增加对象释放
		objects.erase(objects.begin() + index);
		return 1;
	}
	DEBUG_LOG(LOG_ERROR, "Room:在Delete(int index)函数中index不在合法范围内", 0);
	return 0;
}
int Room::DeleteID(int id) {
	int index = Find(id);
	if (index != -1) {
		objects[index]->onDestroy();
		delete objects[index]; 
		objects.erase(objects.begin() + index);
		return 1;
	}
	DEBUG_LOG(LOG_ERROR, "Room:在DeleteID(int id)函数中id不在合法范围内", 0);
	return 0;
}
Object* Room::Create(Object* instance) {
	// 创建新实例并存储到对象列表中
	Object* newInstance = new Object(std::move(*instance));  // 深拷贝对象
	newInstance->_ins_id = _counts;
	objects.push_back(newInstance);
	_counts++;
	return objects.back();  // 返回存储对象的指针
}

Object* Room::AddIns(Object* instance)
{
	instance->_ins_id= _counts;
	_counts++;
	objects.push_back(instance);
	return objects.back();
}

static Room* _Game_Main_Room = nullptr;
static bool _Game_Room_Is_Enter = false;
static Room* _Game_Next_Room=nullptr;
void Room_Goto(Room* room)
{
	if (_Game_Main_Room != nullptr)
		_Game_Main_Room->RunInstance("onDestroy");
	_Game_Main_Room = new Room;
	_Game_Main_Room = room;
	_Game_Main_Room->RunInstance("onEnter");
	_Game_Room_Is_Enter = true;
}
void Room_Set_Next(Room* nextRoom) {
	_Game_Next_Room = new Room;
	_Game_Next_Room = nextRoom;
}
void Room_Goto_Next(Room* nextRoom=nullptr) {
	Room_Goto(_Game_Next_Room);
	if (nextRoom != nullptr)
		Room_Set_Next(nextRoom);
}
void Room_Reset()
{
	_Game_Main_Room = _Game_Main_Room->reset();
	_Game_Room_Is_Enter = false;
}

Room* Room_Get_Now()
{
	if (!_Game_Main_Room)
		return nullptr;
	else
		return _Game_Main_Room;
}

const Room* Room_Get_NowC()
{
	if (!_Game_Main_Room)
		return nullptr;
	else
		return _Game_Main_Room;
}

bool Room_Is_Ready()
{
	if (!_Game_Main_Room) {
		return false;
	}
	return true;
}

/*
 * 运行房间
 * 返回值 -1未知 0未初始化 1成功 2已运行该事件
*/
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
	else {
		return 0;
	}
	return -1;
}


Object* Create_Instance(float x, float y, Object* instance, const char* name = "NONE")
{
	Room* room = Room_Get_Now();
	instance->setObjName(name);
	instance->onEnter();
	instance->x = x;
	instance->y = y;
	return room->Create(instance);
}

Object* Create_Instance(Object* instance, const char* name)
{
	Room* room = Room_Get_Now();
	instance->setObjName(name);
	instance->onEnter();
	return room->Create(instance);
}

int Destroy_Instance(const char* name, int num)
{
	Room* room = Room_Get_Now();
	return room->Delete(name, num);
}

int Destroy_Instance(int id)
{
	return Room_Get_Now()->DeleteID(id);
}

bool IsExist_Instance(const char* name)
{
	return Room_Get_Now()->FindOne(name) ;
}

bool IsExist_Instance(int id)
{
	return Room_Get_Now()->Find(id)!=-1;
}

int Count_Instance(const char* name)
{
	return Room_Get_Now()->GetCount(name);
}

int Count_Instance(int id)
{
	int i = 0;
	for (auto obj : Room_Get_Now()->GetObjList()) {
		if (obj->_ins_id == id) {
			i++;
		}
	}
	return i;
}

int Count_Instance()
{
	return Room_Get_Now()->GetCount(); 
}

std::string Get_Instance_Name(int id)
{
	return Room_Get_Now()->GetName(id);
}

void Show_Instance_Info()
{
	Room* room = Room_Get_Now();
	DEBUG_LOG(LOG_DEBUG, "所有实例信息，在当前房间中：",0);
	
	std::vector<Object*> instance = room->GetObjList();
	int index = 0;
	for (auto ins : instance) {
		std::cout << "实例编号:" << index << "\t实例名称:" << ins->getObjName() << "\t实例ID:" << ins->_ins_id << "\t实例状态：" << IsExist_Instance(ins->_ins_id)<<std::endl;
		index++;
	}
}
