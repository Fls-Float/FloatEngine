#include "Room.h"
void EventInstance(Object* inst, const char* event) {
	using namespace WinFuns;
	void* hWnd = GetWindowHandle();
	if (TextIsEqual(event, "onEnter")) {
		if (inst) inst->onEnter();
		else MessageBox(hWnd, TextFormat("Instance %d Is Error In onEnter", inst), "Error", 0);
	}
	else if (TextIsEqual(event, "onUpdate")) {
		if (inst) inst->onUpdate();
		else MessageBox(hWnd, TextFormat("Instance %d Is Error In onUpdate", inst), "Error", 0);
	}
	else if (TextIsEqual(event, "onRender")) {
		if (inst)inst->onRender();
		else MessageBox(hWnd, TextFormat("Instance %d Is Error In onRender", inst), "Error", 0);
	}
	else if (TextIsEqual(event, "onDestroy")) {
		if (inst) inst->onDestroy();
		else MessageBox(hWnd, TextFormat("Instance %d Is Error In onDestroy", inst), "Error", 0);
	}
	else if (TextIsEqual(event, "onRenderBefore")) {
		if (inst)inst->onRenderBefore();
		else MessageBox(hWnd, TextFormat("Instance %d Is Error In onRenderBefore", inst), "Error", 0);
	}
	else if (TextIsEqual(event, "onRenderNext")) {
		if (inst)inst->onRenderNext();
		else MessageBox(hWnd, TextFormat("Instance %d Is Error In onRenderNext", inst), "Error", 0);
	}
}

void Room::sort_objects()
{
	int len = objects.size();
	int i = 0;
	int j = 0;
	int n = 0;//同时找最大值的最小需要两个下标遍历
	int flag = 0;
	int pos = 0;//用来记录最后一次交换的位置
	int k = len - 1;
	for (i = 0; i < len - 1; i++)//确定排序趟数
	{
		pos = 0;
		flag = 0;
		//正向寻找最大值
		for (j = n; j < k; j++)//确定比较次数
		{
			if (objects[j]->depth > objects[j + 1]->depth)
			{
				//交换
				Object* tmp = objects[j];
				objects[j] = objects[j + 1];
				objects[j + 1] = tmp;
				flag = 1;//加入标记
				pos = j;//交换元素，记录最后一次交换的位置
			}
		}
		if (flag == 0)//如果没有交换过元素，则已经有序,直接结束
		{
			return;
		}
		k = pos;//下一次比较到记录位置即可
		//反向寻找最小值
		for (j = k; j > n; j--)
		{
			Object* tmp = objects[j];
			objects[j] = objects[j - 1];
			objects[j - 1] = tmp;
			flag = 1;
		}
		n++;
		if (flag == 0)//如果没有交换过元素，则已经有序,直接结束
		{
			return;
		}
	}
}

Room::Room()
{
	auto_sort = false;
}

Room::~Room()
{
	RunInstance("onDestroy");
	objects.clear();
	objects.shrink_to_fit();
	std::vector<Object*>().swap(objects);
}

void Room::RunInstance(const char* event)
{
	for (auto obj : objects) {
		if (!(!obj)) {
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

int Room::GetNum(const char* name) const
{
	int n = 0;
	for (auto obj : objects) {
		if (TextIsEqual(obj->getObjName(), name)) {
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
				objects.erase(objects.begin() + FindOne(name));
			}
		}
		else {
			int n = num;
			while (FindOne(name) != -1) {
				if (n == 0) break;
				objects.erase(objects.begin() + FindOne(name));
				n--;
			}
			if (n != 0) return -1;
		}
		return 1;
	}
	return -1;
}
int Room::GetAllNum() const
{
	return (int)objects.size();
}
void Room::Delete(int index)
{
	objects.erase(objects.begin() + index);
	if(auto_sort) sort_objects();
}
void Room::Create(Object* instance)
{
	Object* inst = instance;
	objects.push_back(inst);
	if(auto_sort) sort_objects();
}
void Room_Goto(Room* room)
{
	_Game_Main_Room = new Room;
	_Game_Main_Room = room;
	_Game_Main_Room->RunInstance("onEnter");
	_Game_Room_Is_Enter = true;
}
void Room_Set_Next(Room* nextRoom) {
	_Game_Next_Room = new Room;
	_Game_Next_Room = nextRoom;
}
void Room_Goto_Next(Room* nextRoom) {
	Room_Goto(_Game_Next_Room);
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
		room->RunInstance(event);
		if (TextIsEqual(event, "onEnter")) {
			_Game_Room_Is_Enter = true;
		}
		return 1;
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
	room->Create(instance);
	return instance;
}

void Destroy_Instance(const char* name, int num, ...)
{
	Room* room = Room_Get_Now();
	auto objects = room->GetObjList();
	int index = 0;
	int n = num;
	int delete_num=0;
	if (n == 0) {
		for (auto obj : objects) {
			if (TextIsEqual(obj->getObjName(), name)) {
				room->Delete(index);
			}
		}
	}
	else for (auto obj:objects) {
		if (!obj) {
			return;
		}
		if (n == delete_num) {
			break;
		}
		index++;
		if (TextIsEqual(obj->getObjName(), name)) {
			room->Delete(index);
			delete_num++;
		}
	}

}

bool IsExist_Instance(const char* name)
{
	return (Room_Get_Now()->FindOne(name) != -1);
}

int Count_Instance(const char* name)
{
	return Room_Get_Now()->GetNum(name);
}

int GetNum_Instance()
{
	return Room_Get_Now()->GetAllNum(); 
}						   
