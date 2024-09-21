#include "FloatApi.h"
int F_Debug::m_debug = 1;

void F_Debug::Init(bool debug)
{
	m_debug= debug;
	if (!m_debug) {
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}
}

void F_Debug::Start()
{
	m_debug = 1;
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void F_Debug::Close() {
	m_debug = 0;
   	ShowWindow(GetConsoleWindow(), SW_SHOW);
}

bool F_Debug::IsOpen() {
	return m_debug == 1;
}

void DEBUG_LOG(int lv, const char* text, bool english, bool auto_enter) {
	if (!F_Debug::IsOpen()) {
		return;
	}
	using std::cout;
	switch (lv) {
	case LOG_ALL:
		if (english)
			cout << "LOG:";
		else cout << "日志:";
		break;
	case LOG_DEBUG:
		if (english)
			cout << "DEBUG:";
		else cout << "调试：";
		break;
	case LOG_ERROR:
		if (english)
			cout << "ERROR:";
		else cout << "错误:";
		break;
	case LOG_FATAL:
		if (english)
			cout << "FATAL:";
		else cout << "致命错误:";
		break;
	case LOG_INFO:
		if (english)
			cout << "INFO:";
		else cout << "信息:";
		break;
	case LOG_NONE:
		if (english)
			cout << "NONE:";
		else cout << "无:";
		break;
	case LOG_WARNING:
		if (english)
			cout << "WARNING:";
		else cout << "警告:";
		break;
	default:
		if (english)
			cout << "NONE:";
		else cout << "无:";
		break;
	}
	printf("%s", text);
	if (auto_enter) {
		cout << "\n";
	}
}
Color Make_Color_RGB(int r, int g, int b, float a) {
	return ColorAlpha(Color{ (unsigned char)r,(unsigned char)g,(unsigned char)b }, a);
}
void F_Render::Draw_Sprite_Ex(Texture* img, int ind, float x, float y, float x_scale, float y_scale, float x_origin, float y_origin, float angle, float alpha, Color col) {
	if (ind == 0) DrawTexturePro(*img, { 0,0,(*img).width * 1.0f,(*img).height * 1.0f }, { x,y,(*img).width * x_scale,(*img).height * y_scale }, { x_origin * (*img).width * x_scale,y_origin * (*img).height * y_scale }, angle, ColorAlpha(col, alpha));
	else DrawTexturePro(img[ind], { 0,0,img[ind].width * 1.0f ,img[ind].height * 1.0f }, { x,y,img[ind].width * x_scale,img[ind].height * y_scale }, { x_origin * img[ind].width * x_scale,y_origin * img[ind].height * y_scale }, angle, ColorAlpha(col, alpha));
}

void Audio_Play(F_Audio audio) {
	AudioMode am = audio.GetAudioMode();
	switch (am) {
	case AudioMode::Audio_Unknown:
		DEBUG_LOG(LOG_WARNING, TextFormat("Audio_Play:未初始化的audio!"), false);
		break;
	case AudioMode::Audio_Sound:
		PlaySound(audio.GetSound());
		break;
	case AudioMode::Audio_Music:
		PlayMusicStream(audio.GetMusic());
		break;
	default:
		DEBUG_LOG(LOG_ERROR, "F_Audio:错误的audio_mode,请明确");
		break;
	}
}
void Audio_Stop(F_Audio audio) {
	AudioMode am = audio.GetAudioMode();
	switch (am) {
	case AudioMode::Audio_Unknown:
		DEBUG_LOG(LOG_WARNING, TextFormat("Audio_Play:未初始化的audio!"), false);
		break;
	case AudioMode::Audio_Sound:
		StopSound(audio.GetSound());
		break;
	case AudioMode::Audio_Music:
		StopMusicStream(audio.GetMusic());
		break;
	default:
		DEBUG_LOG(LOG_ERROR, "F_Audio:错误的audio_mode,请明确");
		break;
	}
}
void Audio_Music_Update(F_Audio audio) {
	if (audio.IsTheAudioMode(AudioMode::Audio_Music))
		UpdateMusicStream(audio.GetMusic());
	else {
		DEBUG_LOG(LOG_ERROR, "Audio_Music_Update:只能对music进行update!", false);
	}
}

Font Load_Font(const char* filePath, int size)
{
	int dataSize;
	int count;
	unsigned char* fileData = LoadFileData(filePath, &dataSize);
	int* codepoints = LoadCodepoints(m_CodePoint, &count);
	Font f=LoadFontFromMemory(GetFileExtension(filePath), fileData, dataSize, size, codepoints, count);
	return f;
}

void F_Render::Draw_Text_Ex(Font font, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing,float size, Color col, float alpha) {
	DrawTextPro(font, text, { x,y }, { o_x*MeasureTextEx(font,text,size,spacing).x,o_y*MeasureTextEx(font,text,size,spacing).y }, rot, size, spacing, ColorAlpha(col, alpha));
}
void F_Render::Draw_Shape( F_Shape shape,Color col)
{
	if (shape.Get_Points_Number()<=0) {
		DEBUG_LOG(LOG_WARNING, "Draw_Shape:没有点存在", 0);
	}
	for (int i=0; i < shape.Get_Points_Number()-1; i++) {
		DrawLineV(shape.Get_Points()[i], shape.Get_Points()[i+1],  col);
	}
	DrawLineV(shape.Get_Points()[shape.Get_Points_Number()-1], shape.Get_Points()[0],  col);

}

void F_Render::Draw_Rectangle(F_Rectangle rect, Color col)
{
	DrawRectanglePro({rect.x,rect.y,rect.width,rect.height}, {rect.rot_origin.x * rect.width,rect.rot_origin.y * rect.height}, rect.angle, col);
}

void F_Render::Draw_Circle(F_Circle circle, Color col)
{
	DrawCircle(circle.x, circle.y, circle.radius, col);
}

void F_Render::Draw_Ellipse(F_Ellipse ellipse, Color col)
{
	DrawEllipse(ellipse.x, ellipse.y, ellipse.radiusX, ellipse.radiusY, col);
}

void F_Render::Draw_Shape_Fill(F_Shape shape, Color col)
{
	Draw_Polygon(shape.Get_Points(), col);
}

void F_Render::Draw_Polygon(F_Polygon polygon, Color col)
{
	for (int i = 0; i < polygon.Get_Edge_Number(); i++) {
		Draw_Line(polygon.Get_Edge(i), col);
	}
}

void F_Render::Draw_Polygon(std::vector<FVec2> points, Color color) {
	int pointsCount = static_cast<int>(points.size());
	// 确保点的数量有效
	if (pointsCount < 3) return; // 至少需要三个点

	// 计算并绘制多边形的边
	for (int i = 0; i < pointsCount; i++) {
		int nextIndex = (i + 1) % pointsCount; // 下一个点的索引
		DrawLineV(points[i], points[nextIndex], color); // 绘制边
	}

	// 填充多边形
	// 使用 raylib 提供的 DrawTriangleFan 来填充多边形
	for (int i = 1; i < pointsCount - 1; i++) {
		DrawTriangle(points[0], points[i], points[i + 1], color);
	}
}

void F_Render::Draw_Triangle(F_Triangle triangle, Color col)
{
	DrawTriangle(triangle.a, triangle.b, triangle.c, col);
}

void F_Render::Draw_Line(F_Line line, Color col)
{
	DrawLine(line.p1.x, line.p1.y, line.p2.x, line.p2.y, col);
}

void Load_FCamera( float w, float h, F_Camera* camera)
{
	camera->Load(w, h, true);
}

void Unload_FCamera(F_Camera* camera)
{
	camera->Unload();
}

void Begin_Mode_FCamera(F_Camera* _camera,Color Background_Color)
{
	BeginTextureMode(_camera->Get());
	ClearBackground(Background_Color);
}
void End_Mode_FCamera(F_Camera* _camera,Color Background_Color, int reverse)
{
	EndTextureMode();
	ClearBackground(Background_Color);
	DrawTexturePro(_camera->Get().texture,
		{ 0,0,_camera->Get_Width(),_camera->Get_Height()*(reverse?1:-1)},
		{ _camera->x,_camera->y,_camera->Get_Width() * _camera->x_scale,_camera->Get_Height() * _camera->y_scale },
		{ _camera->Get_Width() * _camera->x_scale * _camera->origin.x,
		_camera->Get_Height() * _camera->y_scale * _camera->origin.y }, _camera->angle, _camera->col);
}
void Begin_Mode_FCamera(F_Camera& _camera,Color Background_Color)
{
	BeginTextureMode(_camera.Get());
	ClearBackground(Background_Color);
}
void End_Mode_FCamera(F_Camera& _camera,Color Background_Color,int reverse)
{
	EndTextureMode();
	ClearBackground(Background_Color);
	DrawTexturePro(_camera.Get().texture,
		{ 0,0,_camera.Get_Width(),_camera.Get_Height() * (reverse ? 1 : -1) },
		{ _camera.x,_camera.y,_camera.Get_Width() * _camera.x_scale,_camera.Get_Height() * _camera.y_scale },
		{ _camera.Get_Width() * _camera.x_scale * _camera.origin.x,
		_camera.Get_Height() * _camera.y_scale * _camera.origin.y }, _camera.angle, _camera.col);
}
void Set_CodePoint(const char* cp)
{
	TextCopy(m_CodePoint, cp);
}

void Add_CodePoint(const char* t)
{
	TextInsert(m_CodePoint, t, TextLength(m_CodePoint));
}

F_Camera::F_Camera()
{
	x_scale = y_scale = 1.0f;
	x = 0; y = 0;
	origin = { 0,0 };
	auto_free = true;
	angle = 0.0f;
	col = WHITE;
	width = height = 0.0f;
	camera_render = RenderTexture();
}

F_Camera::F_Camera(const F_Camera* c)
{
	x_scale = c->x_scale;
	y_scale = c->y_scale;
	x = c->x; y = c->y;
	origin = c->origin;
	camera_render = c->camera_render;
	auto_free = c->auto_free;
	angle = c->angle;
	col = c->col;
	width = c->width;
	height = c->height;
}

void F_Camera::Load(float w, float h,bool _auto_free)
{
	this->camera_render = LoadRenderTexture(w, h);
	auto_free = _auto_free;
	width = w;
	height = h;
}

void F_Camera::Unload()
{
	UnloadRenderTexture(this->camera_render);
}

float F_Camera::Get_Width() const
{
	return width;
}

float F_Camera::Get_Height() const
{
	return height;
}

RenderTexture F_Camera::Get()
{
	return camera_render;
}

F_Camera::~F_Camera()
{
	if (auto_free) {
		Unload();
	}
}
#include <commdlg.h>
std::string F_File::Get_Open_File_Name(std::string strFilter)
{
	OPENFILENAME ofn;
	TCHAR szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter =TEXT(strFilter.c_str());
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = (HWND)GetWindowHandle();
	ofn.Flags = OFN_HIDEREADONLY|OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST| OFN_NODEREFERENCELINKS;

	// 显示文件对话框
	if (GetOpenFileNameA(&ofn) == 1) {
		return std::string(ofn.lpstrFile);
	}
	else {
		return "";  // 用户取消选择或发生错误
	}
}
std::string F_File::Get_Open_File_Name(std::string strFilter,unsigned int flag)
{
	OPENFILENAME ofn;
	char szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT(strFilter.c_str());
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = (HWND)GetWindowHandle();
	ofn.Flags = flag;
	// 显示文件对话框
	if (GetOpenFileNameA(&ofn) == 1) {
		return std::string(ofn.lpstrFile);
	}
	else {
		return "";  // 用户取消选择或发生错误
	}
}

void F_File::Flush_Drop_Files()
{
	if (IsFileDropped()) {
		drop_list = raylib::LoadDroppedFiles();
	}
}

const char* F_File::Get_Drop_File(int index)
{
	if (drop_list.count <= 0) {
		DEBUG_LOG(LOG_WARNING,"F_File_Get_Drop_File没有文件拖入",0);
	}
	else if (index >= drop_list.count) {
     	DEBUG_LOG(LOG_WARNING, "F_File_Get_Drop_File索引超出范围", 0);
	}
	else {
		return drop_list.paths[index];
	}
}
