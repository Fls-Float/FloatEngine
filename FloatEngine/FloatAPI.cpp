#include "FloatApi.h"
int F_Debug::m_debug = 1;

void F_Debug::Init(bool debug)
{
	using namespace WinFuns;
	m_debug = debug;
	if(debug) ShowWindow(GetConsoleWindow(), 1);
}

void F_Debug::Start()
{

	using namespace WinFuns;
	m_debug = 1;
	ShowWindow(GetConsoleWindow(), 1);
}

void F_Debug::Close() {

	using namespace WinFuns;
	m_debug = 0;
   	ShowWindow(GetConsoleWindow(),1);
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
int letter_to_kv(char letter)
{
	return F_Input::to_value(letter);
}
int number_to_kv(int number) {
	return F_Input::number_to_value(number);
}
Color Make_Color_RGB(int r, int g, int b, float a) {
	return ColorAlpha(Color{ (unsigned char)r,(unsigned char)g,(unsigned char)b }, a);
}
F_Color Make_FColor_RGB(int r, int g, int b, float a) {
	F_Color t;
	t.R = r;
	t.G = g;
	t.B = b;
	t.A = a;
	return t;
}
void F_Render::Draw_Set_Line_Thick(float thick)
{
	_draw_line_thick = thick;
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

Font Load_Font(const char* filePath, int size,const char*m_CodePoint)
{
	int dataSize;
	int count;
	unsigned char* fileData = LoadFileData(filePath, &dataSize);
	int* codepoints = LoadCodepoints(m_CodePoint, &count);
	Font f=LoadFontFromMemory(GetFileExtension(filePath), fileData, dataSize, size, codepoints, count);
	return f;
}

Font Load_Font_File_Codepoints(const char* filePath, int size, const char* codePoints_File)
{
	const char* codepoint = LoadFileText(codePoints_File);
	return Load_Font(filePath, size, codepoint);
}
// 在类外赋值
const int F_Input::key_null = 0;
const int F_Input::key_apostrophe = 39;
const int F_Input::key_comma = 44;
const int F_Input::key_minus = 45;
const int F_Input::key_period = 46;
const int F_Input::key_slash = 47;
const int F_Input::key_zero = 48;
const int F_Input::key_one = 49;
const int F_Input::key_two = 50;
const int F_Input::key_three = 51;
const int F_Input::key_four = 52;
const int F_Input::key_five = 53;
const int F_Input::key_six = 54;
const int F_Input::key_seven = 55;
const int F_Input::key_eight = 56;
const int F_Input::key_nine = 57;
const int F_Input::key_semicolon = 59;
const int F_Input::key_equal = 61;
const int F_Input::key_a = 65;
const int F_Input::key_b = 66;
const int F_Input::key_c = 67;
const int F_Input::key_d = 68;
const int F_Input::key_e = 69;
const int F_Input::key_f = 70;
const int F_Input::key_g = 71;
const int F_Input::key_h = 72;
const int F_Input::key_i = 73;
const int F_Input::key_j = 74;
const int F_Input::key_k = 75;
const int F_Input::key_l = 76;
const int F_Input::key_m = 77;
const int F_Input::key_n = 78;
const int F_Input::key_o = 79;
const int F_Input::key_p = 80;
const int F_Input::key_q = 81;
const int F_Input::key_r = 82;
const int F_Input::key_s = 83;
const int F_Input::key_t = 84;
const int F_Input::key_u = 85;
const int F_Input::key_v = 86;
const int F_Input::key_w = 87;
const int F_Input::key_x = 88;
const int F_Input::key_y = 89;
const int F_Input::key_z = 90;
const int F_Input::key_left_bracket = 91;
const int F_Input::key_backslash = 92;
const int F_Input::key_right_bracket = 93;
const int F_Input::key_grave = 96;
const int F_Input::key_space = 32;
const int F_Input::key_escape = 256;
const int F_Input::key_enter = 257;
const int F_Input::key_tab = 258;
const int F_Input::key_backspace = 259;
const int F_Input::key_insert = 260;
const int F_Input::key_delete = 261;
const int F_Input::key_right = 262;
const int F_Input::key_left = 263;
const int F_Input::key_down = 264;
const int F_Input::key_up = 265;
const int F_Input::key_page_up = 266;
const int F_Input::key_page_down = 267;
const int F_Input::key_home = 268;
const int F_Input::key_end = 269;
const int F_Input::key_caps_lock = 280;
const int F_Input::key_scroll_lock = 281;
const int F_Input::key_num_lock = 282;
const int F_Input::key_print_screen = 283;
const int F_Input::key_pause = 284;
const int F_Input::key_f1 = 290;
const int F_Input::key_f2 = 291;
const int F_Input::key_f3 = 292;
const int F_Input::key_f4 = 293;
const int F_Input::key_f5 = 294;
const int F_Input::key_f6 = 295;
const int F_Input::key_f7 = 296;
const int F_Input::key_f8 = 297;
const int F_Input::key_f9 = 298;
const int F_Input::key_f10 = 299;
const int F_Input::key_f11 = 300;
const int F_Input::key_f12 = 301;
const int F_Input::key_left_shift = 340;
const int F_Input::key_left_control = 341;
const int F_Input::key_left_alt = 342;
const int F_Input::key_left_super = 343;
const int F_Input::key_right_shift = 344;
const int F_Input::key_right_control = 345;
const int F_Input::key_right_alt = 346;
const int F_Input::key_right_super = 347;
const int F_Input::key_kb_menu = 348;
const int F_Input::key_kp_0 = 320;
const int F_Input::key_kp_1 = 321;
const int F_Input::key_kp_2 = 322;
const int F_Input::key_kp_3 = 323;
const int F_Input::key_kp_4 = 324;
const int F_Input::key_kp_5 = 325;
const int F_Input::key_kp_6 = 326;
const int F_Input::key_kp_7 = 327;
const int F_Input::key_kp_8 = 328;
const int F_Input::key_kp_9 = 329;
const int F_Input::key_kp_decimal = 330;
const int F_Input::key_kp_divide = 331;
const int F_Input::key_kp_multiply = 332;
const int F_Input::key_kp_subtract = 333;
const int F_Input::key_kp_add = 334;
const int F_Input::key_kp_enter = 335;
const int F_Input::key_kp_equal = 336;
const int F_Input::key_back = 4;
const int F_Input::key_menu = 82;
const int F_Input::key_volume_up = 24;
const int F_Input::key_volume_down = 25;
// 鼠标按钮赋值
const int F_Input::mouse_button_left = 0;
const int F_Input::mouse_button_right = 1;
const int F_Input::mouse_button_middle = 2;
const int F_Input::mouse_button_side = 3;
const int F_Input::mouse_button_extra = 4;
const int F_Input::mouse_button_forward = 5;
const int F_Input::mouse_button_back = 6;
bool F_Input::keyboard_down(int key)
{
	return IsKeyDown(key);
}
bool F_Input::mb_down(int mb)
{
	return IsMouseButtonDown(mb);
}
bool F_Input::mb_pressed(int mb)
{
	return IsMouseButtonPressed(mb);
}
bool F_Input::mb_released(int mb)
{
	return IsMouseButtonReleased(mb);
}
bool F_Input::mb_up(int mb)
{
	return IsMouseButtonUp(mb);
}
bool F_Input::keyboard_pressed(int key) {
	return IsKeyPressed(key);
}
bool F_Input::keyboard_released(int key) {
	return IsKeyReleased(key);
}
bool F_Input::keyboard_pressed_repeat(int key){
	return IsKeyPressedRepeat(key);
}
int F_Input::to_value(char letter)
{
	char c = toupper(letter);
	if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 65;
	}
	return key_null;
}

int F_Input::number_to_value(int number)
{
	if (number >= 0 && number <= 9) {
		return number + 48;
	}
	return key_null;
}

Color F_Color::LightGray = LIGHTGRAY;
Color F_Color::Gray = GRAY;
Color F_Color::DarkGray = DARKGRAY;
Color F_Color::DrakGray = YELLOW;
Color F_Color::Gold = GOLD;
Color F_Color::Orange = ORANGE;
Color F_Color::Pink = PINK;
Color F_Color::Red = RED;
Color F_Color::Maroon = MAROON;
Color F_Color::Green = GREEN;
Color F_Color::Lime = LIME;
Color F_Color::DarkGreen = DARKGREEN;
Color F_Color::SkyBlue = SKYBLUE;
Color F_Color::Blue = BLUE;
Color F_Color::DarkBlue = DARKBLUE;
Color F_Color::Purple = PURPLE;
Color F_Color::Violet = VIOLET;
Color F_Color::Darkpurple = DARKPURPLE;
Color F_Color::Beige = BEIGE;
Color F_Color::Borwn = BROWN;
Color F_Color::DarkBrown = DARKBROWN;
Color F_Color::White = WHITE;
Color F_Color::Black = BLACK;
Color F_Color::Blank = BLANK;
Color F_Color::Magenta = MAGENTA;
Color F_Color::RayWhite = RAYWHITE;
Color F_Color::Yellow = YELLOW;

float F_Render::_draw_line_thick = 1.0f;

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

void F_Render::Draw_Line(float x1, float y1, float x2, float y2, Color col)
{
	DrawLine(x1, y1, x2, y1, col);
}

void F_Render::Draw_Rectangle(float x, float y, float w, float h, Color col, bool line_mode)
{
	if(!line_mode)
		DrawRectangle(x, y, w, h, col);
	else {
		DrawRectangleLinesEx({ x,y,w,h }, _draw_line_thick, col);
	}
}

void F_Render::Draw_Rectangle(float x, float y, float w, float h, float rot, FVec2 origin, Color col, bool line_mode)
{
	if(!line_mode)
		DrawRectanglePro({ x,y,w,h }, {origin.x,origin.y}, rot, col);
	else {
		// 计算矩形的半宽和半高
		float hw = w / 2.0f;
		float hh = h / 2.0f;

		// 基于原点计算矩形的四个顶点（未经旋转）
		FVec2 vertices[4] = {
			{ -hw - origin.x, -hh - origin.y },  // 左下角
			{  hw - origin.x, -hh - origin.y },  // 右下角
			{  hw - origin.x,  hh - origin.y },  // 右上角
			{ -hw - origin.x,  hh - origin.y }   // 左上角
		};

		// 旋转矩阵的正余弦值
		float cosRot = cosf(rot);
		float sinRot = sinf(rot);

		// 用来保存旋转后的顶点
		Vector2 transformedVertices[4];

		// 计算旋转和平移后的顶点位置
		for (int i = 0; i < 4; ++i) {
			float rotatedX = vertices[i].x * cosRot - vertices[i].y * sinRot;
			float rotatedY = vertices[i].x * sinRot + vertices[i].y * cosRot;
			transformedVertices[i] = { rotatedX + x, rotatedY + y };
		}
		for (int i = 0; i < 4; i++) {
			for (int i = 0; i < 4; ++i) {
				int next = (i + 1) % 4; // 下一个顶点，形成一个循环
				DrawLineV(transformedVertices[i], transformedVertices[next], col);
			}
		}
	}
}

void F_Render::Draw_Circle(float x, float y, float radius, Color col, bool line_mode)
{
	if(!line_mode)
		DrawCircle(x, y, radius, col);
	else {
		DrawCircleLines(x, y, radius, col);
	}
}

void F_Render::Draw_Circle(float x, float y, float radius, float startAngle, float angle, Color col, bool line_mode)
{
	if(!line_mode)
		DrawCircleSector({ x,y }, radius, startAngle, startAngle + angle, angle, col);
	else {
		DrawCircleSectorLines({ x,y }, radius, startAngle, startAngle + angle, angle, col);
	}
}

void F_Render::Draw_Ellipse(float x, float y, float a, float b, Color col, bool line_mode )
{
	if (!line_mode)
		DrawEllipse(x, y, a, b, col);
	else {
		DrawEllipseLines(x, y, a, b, col);
	}
}

void F_Render::Draw_Triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color col, bool line_mode)
{
	if (!line_mode)
		DrawTriangle({ x1,y1 }, { x2,y2 }, { x3,y3 }, col);
	else {
		DrawTriangleLines({ x1,y1 }, { x2,y2 }, { x3,y3 }, col);
	}
}
void F_Render::Draw_Triangle(float x1, float y1, float x2, float y2, float x3, float y3, float rot, Color col, bool line_mode) {
	if (!line_mode) {
		float centerX = (x1 + x2 + x3) / 3.0f;
		float centerY = (y1 + y2 + y3) / 3.0f;
		FVec2 p1 = Rotate_Point({ x1, y1 }, { centerX, centerY }, rot);
		FVec2 p2 = Rotate_Point({ x2, y2 }, { centerX, centerY }, rot);
		FVec2 p3 = Rotate_Point({ x3, y3 }, { centerX, centerY }, rot);
		DrawTriangle(p1, p2, p3, col);
	}
	else {
		float centerX = (x1 + x2 + x3) / 3.0f;
		float centerY = (y1 + y2 + y3) / 3.0f;
		FVec2 p1 = Rotate_Point({ x1, y1 }, { centerX, centerY }, rot);
		FVec2 p2 = Rotate_Point({ x2, y2 }, { centerX, centerY }, rot);
		FVec2 p3 = Rotate_Point({ x3, y3 }, { centerX, centerY }, rot);
		DrawTriangleLines(p1, p2, p3, col);
	}
}
void F_Render::Draw_Text_Outline(struct Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color outline_color, float alpha ,bool fill_all)
{
	Font _draw_font = fnt;
	if (!fill_all) {
		Draw_Text_Ex(fnt, text, x - thick, y, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
		Draw_Text_Ex(fnt, text, x + thick, y, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
		Draw_Text_Ex(fnt, text, x, y - thick, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
		Draw_Text_Ex(fnt, text, x, y + thick, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
	}
	else {
		for (int i = 1; i <= (int)(thick); i++) {
			Draw_Text_Ex(fnt, text, x - i, y, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
			Draw_Text_Ex(fnt, text, x + i, y, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
			Draw_Text_Ex(fnt, text, x, y - i, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
			Draw_Text_Ex(fnt, text, x, y + i, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
		}
	}
	Draw_Text_Ex(fnt, text, x, y, o_x, o_y, rot, spacing, font_size, col, alpha);
}
void F_Render::Draw_Text_Shadow( Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color shadow_color, float alpha , bool fill_all)
{
	Font _draw_font = fnt;
	if (!fill_all) {
		Draw_Text_Ex(fnt, text, x + thick, y, o_x, o_y, rot, spacing, font_size, shadow_color, alpha);
		Draw_Text_Ex(fnt, text, x, y + thick, o_x, o_y, rot, spacing, font_size, shadow_color, alpha);
	}
	else {
		for (int i = 1; i <= (int)(thick); i++) {
			Draw_Text_Ex(fnt, text, x + i, y, o_x, o_y, rot, spacing, font_size, shadow_color, alpha);
			Draw_Text_Ex(fnt, text, x, y + i, o_x, o_y, rot, spacing, font_size, shadow_color, alpha);
		}
	}
	Draw_Text_Ex(fnt, text, x, y, o_x, o_y, rot, spacing, font_size, col, alpha);
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

#include <string.h>
using namespace WinFuns;
typedef struct tagOFNA {
	DWORD         lStructSize;
	void*         hwndOwner;
	void*     hInstance;
	const char*       lpstrFilter;
	const char*        lpstrCustomFilter;
	DWORD         nMaxCustFilter;
	DWORD         nFilterIndex;
	char*        lpstrFile;
	DWORD         nMaxFile;
	char*        lpstrFileTitle;
	DWORD         nMaxFileTitle;
	const char*       lpstrInitialDir;
	const char*    lpstrTitle;
	DWORD         Flags;
	WORD          nFileOffset;
	WORD          nFileExtension;
	const char*       lpstrDefExt;
	long unsigned*        lCustData;
	const char*       lpTemplateName;
	const char*        lpstrPrompt;
	void* pvReserved;
	DWORD         dwReserved;
	DWORD         FlagsEx;
} OPENFILENAMEA, * LPOPENFILENAMEA;
#define OPENFILENAME OPENFILENAMEA
extern "C" int GetOpenFileNameA(OPENFILENAME* ofn);
#define TCHAR wchar_t
std::string F_File::Get_Open_File_Name(std::string strFilter)
{
	using namespace WinFuns;
	OPENFILENAME ofn;
	char szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	memset(&ofn, sizeof(ofn),0);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter =strFilter.c_str();
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = GetWindowHandle();
	ofn.Flags = 0x00000004 | 0x00000800 | 0x00001000 | 0x00100000;

	// 显示文件对话框
	if (GetOpenFileNameA(&ofn) == 1) {
		return std::string(ofn.lpstrFile);
	}
	else {
		return "";  // 用户取消选择或发生错误
	}
}
bool F_File::loaded = false;
FilePathList F_File::drop_list = FilePathList();
std::string F_File::Get_Open_File_Name(std::string strFilter,unsigned int flag)
{
	using namespace WinFuns;
	OPENFILENAME ofn;
	char szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	memset(&ofn, sizeof(ofn),0);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = (strFilter.c_str());
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = GetWindowHandle();
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
		loaded = true;
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

F_File::F_File()
{
	drop_list = FilePathList();
	loaded = false;
}

F_File::~F_File()
{
	if (loaded) {
		UnloadDroppedFiles(drop_list);
	}
}

Color F_Color::To_RlCol()
{
	return Make_Color_RGB(R, G, B, A);
}

F_Color::operator struct Color()
{
	return Make_Color_RGB(R, G, B, A);
}
