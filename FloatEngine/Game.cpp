#include "Game.h"
#include "Room.h"
#include <string>
#include "F_Console.h"

Game::Game()
{
	_fps = _w = _h = 0;
	TextCopy(_title, "");
	background_col = BLACK;
	_load_icon = false;
	_icon_number = 0;
	memset(_icons, 0, sizeof _icons);
	_icon_path = FilePathList();
	_load_icon_pre = 0;
	_icon_path.capacity = 20;
}

void ApplyGuiStyle() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.13f, 0.15f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.17f, 0.19f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.09f, 0.10f, 0.95f);
	colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.27f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.32f, 0.34f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.17f, 0.19f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.20f, 0.22f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	// 样式参数调整
	style.WindowPadding = ImVec2(8, 8);
	style.FramePadding = ImVec2(5, 3);
	style.CellPadding = ImVec2(6, 4);
	style.ItemSpacing = ImVec2(6, 4);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21;
	style.ScrollbarSize = 16;
	style.GrabMinSize = 10;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 0;
	style.TabBorderSize = 0;
	style.WindowRounding = 3;
	style.ChildRounding = 3;
	style.FrameRounding = 2;
	style.PopupRounding = 3;
	style.ScrollbarRounding = 3;
	style.GrabRounding = 2;
	style.TabRounding = 2;
}
void LoadGuiFont(float dpi_scale = 1.0f) {
	ImGuiIO& io = ImGui::GetIO();
	const float base_font_size = 16.0f * dpi_scale;
	ImFontConfig font_cfg;
	font_cfg.OversampleH = 3;
	font_cfg.OversampleV = 2;
	font_cfg.RasterizerMultiply = 1.2f;
	// 加载主字体
	io.FontDefault = io.Fonts->AddFontFromFileTTF("C://windows//fonts//msyh.ttc",
		base_font_size, &font_cfg,
		io.Fonts->GetGlyphRangesChineseFull());
	rlImGuiReloadFonts();
}
void Game::CreateWindow(int w, int h, const char* title, bool debug, int flags)
{
	F_Debug::Init(debug);
	using namespace WinFuns;
	SetConfigFlags(flags);
	Init_FConsole();
	InitWindow(w, h, "Create With FloatEngine");
	BeginDrawing(); EndDrawing();
	F_Gui::Init();
	F_Debug::InitCommand();
	LoadGuiFont();
	BeginDrawing(); EndDrawing();

	ApplyGuiStyle();
	if (_load_icon) {
		SetWindowIcons(_icons, _icon_number);
	}
	if (_load_icon_pre) {
		if (_icon_path.paths == nullptr) {
			DEBUG_LOG(LOG_WARNING, "_icon_path为空!", 0);
		}
		else if (_icon_path.count > 1) {
			Image* icons = new Image[_icon_path.count + 1];
			for (int i = 0; i < _icon_path.count; i++) {
				icons[i] = ::LoadImage(_icon_path.paths[i]);
			}
			::SetWindowIcons(icons, _icon_path.count); 
			for (int i = 0; i < _icon_path.count; i++) {
				UnloadImage(icons[i]);
			}
			delete[]icons;
		}
		else if (_icon_path.count == 1 ) {
			Image icon = ::LoadImage(_icon_path.paths[0]);
			::SetWindowIcon(icon);
			::UnloadImage(icon);
		}
	}
	if (!TextIsEqual(title, "")) {
		if (F_Debug::IsOpen()) {
			SetWindowTitle(TextFormat("%s(DEBUG - floatengine v%s)", title,FLOAT_API_VERSION));
		}
		else {
			SetWindowTitle(title);
		}
	}
	//初始化
	InitAudioDevice();
	floatapi_font::InitDefaultFont();
	_w = w; _h = h;
	TextCopy(_title, title);
	BeginDrawing();EndDrawing();
	DEBUG_LOG(LOG_INFO, "Game_Create_Window:窗口创建成功，当前状态:未激活(使用ShowWindow来手动激活)",0);
}
bool Game::CanStart()
{
	return (IsWindowReady() && IsAudioDeviceReady());
}

void Game::Play(int fps)
{
	if (fps != _fps) {
		_fps = fps;
		SetTargetFPS(_fps);
	}
	start_time = std::chrono::high_resolution_clock::now();
	double last_time = 0;
	while (!WindowShouldClose()) {
		auto end_time = std::chrono::high_resolution_clock::now();
		now_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();
		frame_time = now_time - last_time;
		last_time = now_time;
		GetRealFPS();
		GetFrameTime();
		GetTime();
		//enter
		Room_Run_Now("onEnter");
		//step
		Room_Run_Now("onAlarmEvent");
		Room_Run_Now("onUpdate");
		//draw
		Room_Run_Now("onRenderBefore");
		BeginDrawing(); {
			rlImGuiBegin();
			Room_Run_Now("onBeginCamera");
			ClearBackground(background_col);
			Room_Run_Now("onRender");
			Room_Run_Now("onEndCamera");
			rlImGuiEnd();
		}EndDrawing();
		Room_Run_Now("onRenderNext");
	}
}

void Game::Destroy()
{
	_fps = _w = _h = 0;
	Room* t = Room_Get_Now();
	delete t;
	delete _title;
	F_Gui::Shutdown();
	CloseAudioDevice();
	CloseWindow();
}


void Game::Set_Background_Color(Color bk_col)
{
	background_col = bk_col;
}

void Game::Set_Console_Icon(int id)
{
	using namespace WinFuns;
	// 获取当前控制台窗口的句柄
	HWND hwnd =(HWND)GetConsoleWindow();

	// 从资源文件中加载图标（使用资源标识符 IDI_MY_ICON）
	void* hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(id));

	if (hIcon) {
		// 发送消息设置图标
		SendMessage(hwnd, 0x0080,1, (LPARAM)hIcon); // 设置小图标
		SendMessage(hwnd, 0x0080, 0, (LPARAM)hIcon);   // 设置大图标
	}
}

void Game::Set_Title(const char* title)
{
	TextCopy(_title, title);
	SetWindowTitle(title);
}

void Game::Set_Icons(Image* icons, int n)
{
	SetWindowIcons(icons, n);
	memset(_icons, 0, sizeof _icons);
	for (int i = 0; i < n; i++) {
		_icons[i] = icons[i];
	}
}

void Game::Set_Icon(Image icon)
{
	SetWindowIcon(icon);
	memset(_icons, 0, sizeof _icons);
	_icons[0] = icon;
}
char** ConvertStringVectorToCharArray(const std::vector<std::string>& strings) {
	// 创建 char** 数组
	char** result = new char* [strings.size() + 1]; // 额外分配一个空指针用于标记结束

	for (size_t i = 0; i < strings.size(); ++i) {
		// 分配空间并拷贝字符串
		result[i] = new char[strings[i].size() + 1];
		std::copy(strings[i].begin(), strings[i].end(), result[i]);
		result[i][strings[i].size()] = '\0'; // 添加 null 终止符
	}

	// 设置最后一个指针为空指针
	result[strings.size()] = nullptr;

	return result;
}

void FreeCharArray(char** array) {
	if (!array) return;

	for (size_t i = 0; array[i] != nullptr; ++i) {
		delete[] array[i];
	}

	delete[] array;
}
void Game::Set_Icon_Pre(const char* filePath) {
	if (_icon_path.count >= _icon_path.capacity) {
		if (_icon_path.capacity == 0) {
			_icon_path.capacity = 1;
		}
		else {
			_icon_path.capacity *= 2;
		}
		std::vector<std::string> newPaths;
		for (unsigned int i = 0; i < _icon_path.count; i++) {
			newPaths.push_back(_icon_path.paths[i]);
		}
		delete[] _icon_path.paths;
		_icon_path.paths = ConvertStringVectorToCharArray(newPaths);
	}
	size_t pathLen = strlen(filePath) + 1; 
	_icon_path.paths[_icon_path.count] = (char*)MemAlloc(pathLen);
	//_icon_path.paths[_icon_path.count] = new char[pathLen];
	strcpy(_icon_path.paths[_icon_path.count], filePath);  
	_icon_path.count++;
	_load_icon_pre = 1;
}

void Game::Set_Icons_Pre(FilePathList filePathList) {
	// 确保传入的文件路径列表有效
	if (filePathList.paths == nullptr || filePathList.count == 0) {
		DEBUG_LOG(LOG_ERROR,"Set_Icons_Pre: 文件路径列表为空。",0);
		return;
	}
	for (unsigned int i = 0; i < filePathList.count; i++) {
		const char* filePath = filePathList.paths[i];
		if (filePath == nullptr) continue; 
		if (_icon_path.count >= _icon_path.capacity) {
			if (_icon_path.capacity == 0) {
				_icon_path.capacity = 1;  
			}
			else {
				_icon_path.capacity *= 2; 
			}
			std::vector<std::string> newPaths;
			for (unsigned int i = 0; i < _icon_path.count; i++) {
				newPaths.push_back(_icon_path.paths[i]);
			}
			delete[] _icon_path.paths;
			_icon_path.paths = ConvertStringVectorToCharArray(newPaths);
		}
		size_t pathLen = strlen(filePath) + 1; 
		_icon_path.paths[_icon_path.count] = (char*)MemAlloc(pathLen);
		strcpy(_icon_path.paths[_icon_path.count], filePath);
		_icon_path.count++;
	}
	_load_icon_pre = 1;
}

double Game::Get_Now_Time()
{
	return now_time;
}

double Game::Get_Frame_Time()
{
	return frame_time;
}

void Game::Reset_Clock()
{
	start_time = std::chrono::high_resolution_clock::now();
}


void Game::SetFPS(int fps) {
	::SetTargetFPS(fps);
	_fps = fps;
}
int Game::GetFPS()const {
	return _fps;
}
int Game::GetRealFPS() const
{
	return ::GetFPS();
}
void Game::Set_Window_Pos(int x,int y) {
	if (x == -1 && y == -1)
	{
		return;
	}
	if (x == -1) {
		x = ::GetWindowPosition().x;
	}
	if (y == -1) {
		y = ::GetWindowPosition().y;
	}
	::SetWindowPosition(x, y);
}

void Game::Set_Window_Opacity(float op) {
	::SetWindowOpacity(op);
}