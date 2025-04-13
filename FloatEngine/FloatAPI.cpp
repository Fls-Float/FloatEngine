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

void F_Render::Draw_Sprite(Sprite spr, float x, float y, float x_scale, float y_scale, float x_origin, float y_origin, float angle, float alpha, Color col)
{
	Texture img = spr.GetCurrentFrame();
	DrawTexturePro(img, { 0,0,(img).width * 1.0f,(img).height * 1.0f }, { x,y,(img).width * x_scale,(img).height * y_scale }, { x_origin * (img).width * x_scale,y_origin * (img).height * y_scale }, angle, ColorAlpha(col, alpha));
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
	using namespace floatapi_math;
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
void F_Render::Draw_Text_Outline(F_Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color outline_color, float alpha, bool fill_all)
{
	Draw_Text_Outline(fnt.to_raylib_font(), text, x, y, o_x, o_y, rot, spacing, font_size, thick, col, outline_color, alpha, fill_all);
}
void F_Render::Draw_Text_Shadow(F_Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color shadow_color, float alpha, bool fill_all)
{
	Draw_Text_Shadow(fnt.to_raylib_font(), text, x, y, o_x, o_y, rot, spacing, font_size, thick, col, shadow_color, alpha, fill_all);
}
void F_Render::Draw_Text_Ex(F_Font font, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float size, Color col, float alpha)
{
	Draw_Text_Ex(font.to_raylib_font(), text, x,y,o_x,o_y,rot,spacing,size,col, alpha);
}
void F_Render::Draw_Text(const char* text, float x, float y, float o_x, float o_y, float rot, float size, Color col, float alpha)
{
	extern Font f_default_font;
	Draw_Text_Ex(f_default_font, text, x, y, o_x, o_y, rot, 0, size, col, alpha);
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
typedef struct tagOFNW {
	DWORD         lStructSize;
	void* hwndOwner;
	void* hInstance;
	const wchar_t* lpstrFilter;
	const wchar_t* lpstrCustomFilter;
	DWORD         nMaxCustFilter;
	DWORD         nFilterIndex;
	wchar_t* lpstrFile;
	DWORD         nMaxFile;
	wchar_t* lpstrFileTitle;
	DWORD         nMaxFileTitle;
	const wchar_t* lpstrInitialDir;
	const wchar_t* lpstrTitle;
	DWORD         Flags;
	WORD          nFileOffset;
	WORD          nFileExtension;
	const wchar_t* lpstrDefExt;
	long unsigned* lCustData;
	const wchar_t* lpTemplateName;
	const wchar_t* lpstrPrompt;
	void* pvReserved;
	DWORD         dwReserved;
	DWORD         FlagsEx;
} OPENFILENAMEW, * LPOPENFILENAMEW;
#define OPENFILENAME OPENFILENAMEA
extern "C" int GetOpenFileNameA(OPENFILENAME* ofn);
extern "C" int GetOpenFileNameW(OPENFILENAMEW* ofn);
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

std::wstring F_File::Get_Open_File_NameW(std::wstring strFilter, unsigned int flag)
{
	using namespace WinFuns;
	OPENFILENAMEW ofn;
	wchar_t szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	memset(&ofn, sizeof(ofn), 0);
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
	if (GetOpenFileNameW(&ofn) == 1) {
		return std::wstring(ofn.lpstrFile);
	}
	else {
		return L"";  // 用户取消选择或发生错误
	}
}

void F_File::Flush_Drop_Files()
{
	if (IsFileDropped()) {
		drop_list = LoadDroppedFiles();
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

#include <fstream>
// 字符串修剪辅助函数
static inline std::string& trim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
	return s;
}

void F_Ini::parse_line(std::string& line) {
	line = line.substr(0, line.find(';'));  // 移除行内注释
	trim(line);

	if (line.empty()) return;

	if (line[0] == '[' && line.back() == ']') {
		currentSection = line.substr(1, line.size() - 2);
		trim(currentSection);
		sections[currentSection];  // 确保section存在
	}
	else if (auto eq_pos = line.find('='); eq_pos != std::string::npos) {
		std::string key = line.substr(0, eq_pos);
		std::string value = line.substr(eq_pos + 1);
		trim(key);
		trim(value);
		sections[currentSection][key] = value;
	}
}
F_Ini::F_Ini(const std::string& filename ) {
	if (!filename.empty()) {
		Load(filename);
	}
}

bool F_Ini::Load(const std::string& filename) {
	sections.clear();
	currentFile = filename;
	currentSection = "default";  // 默认section

	std::ifstream file(filename);
	if (!file.is_open()) return false;

	std::string line;
	while (std::getline(file, line)) {
		parse_line(line);
	}
	return true;
}

bool F_Ini::Save(const std::string& filename) {
	std::string save_path = filename.empty() ? currentFile : filename;
	if (save_path.empty()) return false;

	std::ofstream file(save_path);
	if (!file.is_open()) return false;

	for (const auto& [section, entries] : sections) {
		file << "[" << section << "]\n";
		for (const auto& [key, value] : entries) {
			file << key << "=" << value << "\n";
		}
		file << "\n";
	}
	return true;
}

// 读取方法
int F_Ini::Get_Int_Value_From_Name(const std::string& Class, const std::string& name) {
	try {
		return std::stoi(sections[Class][name]);
	}
	catch (...) {
		return 0;
	}
}

std::string F_Ini::Get_String_Value_From_Name(const std::string& Class, const std::string& name) {
	return sections[Class][name];
}

double F_Ini::Get_Double_Value_From_Name(const std::string& Class, const std::string& name) {
	try {
		return std::stod(sections[Class][name]);
	}
	catch (...) {
		return 0.0;
	}
}

// 写入方法
bool F_Ini::Write_Int_Value(const std::string& Class, const std::string& name, int value) {
	sections[Class][name] = std::to_string(value);
	return true;
}

bool F_Ini::Write_String_Value(const std::string& Class, const std::string& name, const std::string& value) {
	sections[Class][name] = value;
	return true;
}

bool F_Ini::Write_Double_Value(const std::string& Class, const std::string& name, double value) {
	sections[Class][name] = std::to_string(value);
	return true;
}
#include <sstream>
#include <cctype>
#include <algorithm>

namespace F_Json {

	// JsonNull 实现
	JsonType JsonNull::type() const { return JsonType::Null; }
	std::string JsonNull::serialize() const { return "null"; }

	// JsonBoolean 实现
	JsonBoolean::JsonBoolean(bool v) : val(v) {}
	JsonType JsonBoolean::type() const { return JsonType::Boolean; }
	std::string JsonBoolean::serialize() const { return val ? "true" : "false"; }
	bool JsonBoolean::value() const { return val; }

	// JsonNumber 实现
	JsonNumber::JsonNumber(double v) : val(v) {}
	JsonType JsonNumber::type() const { return JsonType::Number; }
	std::string JsonNumber::serialize() const {
		std::ostringstream oss;
		if (val == static_cast<int>(val)) {
			oss << static_cast<int>(val);
		}
		else {
			oss << val;
		}
		return oss.str();
	}
	double JsonNumber::value() const { return val; }

	// JsonString 实现
	JsonString::JsonString(std::string v) : val(std::move(v)) {}
	JsonType JsonString::type() const { return JsonType::String; }
	std::string JsonString::serialize() const {
		std::ostringstream oss;
		oss << '"';
		for (char c : val) {
			switch (c) {
			case '"':  oss << "\\\""; break;
			case '\\': oss << "\\\\"; break;
			case '\b': oss << "\\b";  break;
			case '\f': oss << "\\f";  break;
			case '\n': oss << "\\n";  break;
			case '\r': oss << "\\r";  break;
			case '\t': oss << "\\t";  break;
			default:   oss << c;      break;
			}
		}
		oss << '"';
		return oss.str();
	}
	std::string JsonString::value() const { return val; }

	// JsonArray 实现
	JsonType JsonArray::type() const { return JsonType::Array; }
	std::string JsonArray::serialize() const {
		std::string res = "[";
		for (size_t i = 0; i < vals.size(); ++i) {
			if (i > 0) res += ", ";
			res += vals[i]->serialize();
		}
		return res + "]";
	}
	void JsonArray::add(std::shared_ptr<JsonValue> v) { vals.push_back(v); }
	size_t JsonArray::size() const { return vals.size(); }
	std::shared_ptr<JsonValue> JsonArray::get(size_t i) const { return vals.at(i); }

	// JsonObject 实现
	JsonType JsonObject::type() const { return JsonType::Object; }
	std::string JsonObject::serialize() const {
		std::string res = "{";
		bool first = true;
		for (const auto& [k, v] : vals) {
			if (!first) res += ", ";
			res += JsonString(k).serialize() + ": " + v->serialize();
			first = false;
		}
		return res + "}";
	}
	void JsonObject::set(const std::string& k, std::shared_ptr<JsonValue> v) { vals[k] = v; }
	bool JsonObject::has(const std::string& k) const { return vals.count(k); }
	std::shared_ptr<JsonValue> JsonObject::get(const std::string& k) const { return vals.at(k); }

	size_t JsonObject::size() const {
		return vals.size();
	}

	// Json 包装类实现
	Json::Json(std::shared_ptr<JsonValue> v) : val(v) {}
	Json::Json() : val(std::make_shared<JsonNull>()) {}

	Json Json::Null() { return Json(std::make_shared<JsonNull>()); }
	Json Json::Boolean(bool b) { return Json(std::make_shared<JsonBoolean>(b)); }
	Json Json::Number(double d) { return Json(std::make_shared<JsonNumber>(d)); }
	Json Json::String(const std::string& s) { return Json(std::make_shared<JsonString>(s)); }
	Json Json::Array() { return Json(std::make_shared<JsonArray>()); }
	Json Json::Object() { return Json(std::make_shared<JsonObject>()); }

	JsonType Json::type() const { return val->type(); }

	bool Json::asBoolean() const {
		auto p = dynamic_cast<JsonBoolean*>(val.get());
		if (!p) throw std::runtime_error("Not a boolean");
		return p->value();
	}

	double Json::asNumber() const {
		auto p = dynamic_cast<JsonNumber*>(val.get());
		if (!p) throw std::runtime_error("Not a number");
		return p->value();
	}

	std::string Json::asString() const {
		auto p = dynamic_cast<JsonString*>(val.get());
		if (!p) throw std::runtime_error("Not a string");
		return p->value();
	}

	Json& Json::add(const Json& item) {
		if (val->type() != JsonType::Array) {
			throw std::runtime_error("Not an array");
		}
		auto p = static_cast<JsonArray*>(val.get());
		p->add(item.val);
		return *this;
	}

	Json Json::get(size_t index) const {
		auto p = dynamic_cast<JsonArray*>(val.get());
		if (!p) throw std::runtime_error("Not an array");
		return Json(p->get(index));
	}

	Json Json::operator[](size_t index) const
	{
		return get(index);
	}

	Json& Json::set(const std::string& key, const Json& v) {
		auto p = dynamic_cast<JsonObject*>(val.get());
		if (!p) throw std::runtime_error("Not an object");
		p->set(key, v.val);
		return *this;
	}

	Json Json::get(const std::string& key) const {
		auto p = dynamic_cast<JsonObject*>(val.get());
		if (!p) throw std::runtime_error("Not an object");
		return Json(p->get(key));
	}

	Json Json::operator[](const std::string& key) const
	{
		return get(key);
	}

	std::string Json::serialize() const { return val->serialize(); }

	// 解析相关辅助函数
	namespace {
		// f_json.cpp
		static void skip_ws(const std::string& s, size_t& pos) {
			while (pos < s.size()) {
				// 处理单行注释
				if (s[pos] == '/' && pos + 1 < s.size() && s[pos + 1] == '/') {
					pos += 2;
					while (pos < s.size() && s[pos] != '\n') pos++;
					continue;
				}

				// 处理多行注释
				if (s[pos] == '/' && pos + 1 < s.size() && s[pos + 1] == '*') {
					pos += 2;
					while (pos < s.size()) {
						if (s[pos] == '*' && pos + 1 < s.size() && s[pos + 1] == '/') {
							pos += 2;
							break;
						}
						pos++;
					}
					continue;
				}

				// 空白处理
				if (!std::isspace(s[pos])) break;
				pos++;
			}
		}

		Json parse_value(const std::string& s, size_t& pos);
		Json parse_string(const std::string& s, size_t& pos);
		
		Json parse_object(const std::string& s, size_t& pos) {
			Json obj = Json::Object();
			pos++; // Skip '{'
			bool expect_comma = false; // 标记是否期望逗号

			while (pos < s.size()) {
				skip_ws(s, pos);

				// 检查对象结束
				if (s[pos] == '}') {
					pos++;
					return obj;
				}

				// 检查逗号（如果是期望的）
				if (expect_comma) {
					if (s[pos] == ',') {
						pos++;
						skip_ws(s, pos);
					}
					else {
						throw std::runtime_error("Expected ',' or '}'");
					}
				}

				// 解析键值对
				auto key = parse_string(s, pos).asString();
				skip_ws(s, pos);

				if (s[pos++] != ':') {
					throw std::runtime_error("Expected ':'");
				}

				auto val = parse_value(s, pos);
				obj.set(key, val);
				expect_comma = true; // 下一个应期望逗号或结束符
			}

			throw std::runtime_error("Unterminated object");
		}

		Json parse_array(const std::string& s, size_t& pos) {
			Json arr = Json::Array();
			pos++; // Skip '['
			while (pos < s.size()) {
				skip_ws(s, pos);
				if (s[pos] == ']') { pos++; break; }

				arr.add(parse_value(s, pos));
				skip_ws(s, pos);
				if (s[pos] == ',') pos++;
				else if (s[pos] != ']') throw std::runtime_error("Expected ',' or ']'");
			}
			return arr;
		}

		static Json parse_string(const std::string& s, size_t& pos) {
			std::string res;
			bool escape = false;
			pos++; // Skip opening "
			while (pos < s.size()) {
				char c = s[pos++];
				if (escape) {
					switch (c) {
					case '"':  res += '"';  break;
					case '\\': res += '\\'; break;
					case 'b':  res += '\b'; break;
					case 'f':  res += '\f'; break;
					case 'n':  res += '\n'; break;
					case 'r':  res += '\r'; break;
					case 't':  res += '\t'; break;
					default:   throw std::runtime_error("Invalid escape");
					}
					escape = false;
				}
				else if (c == '"') {
					return Json::String(res);
				}
				else if (c == '\\') {
					escape = true;
				}
				else {
					res += c;
				}
			}
			throw std::runtime_error("Unterminated string");
		}

		Json parse_bool(const std::string& s, size_t& pos) {
			if (s.substr(pos, 4) == "true") {
				pos += 4;
				return Json::Boolean(true);
			}
			else if (s.substr(pos, 5) == "false") {
				pos += 5;
				return Json::Boolean(false);
			}
			throw std::runtime_error("Invalid boolean");
		}

		Json parse_null(const std::string& s, size_t& pos) {
			if (s.substr(pos, 4) == "null") {
				pos += 4;
				return Json::Null();
			}
			throw std::runtime_error("Invalid null");
		}

		Json parse_number(const std::string& s, size_t& pos) {
			size_t end = pos;
			if (s[end] == '-') end++;
			while (end < s.size() && std::isdigit(s[end])) end++;
			if (s[end] == '.') end++;
			while (end < s.size() && std::isdigit(s[end])) end++;
			if (s[end] == 'e' || s[end] == 'E') {
				end++;
				if (s[end] == '+' || s[end] == '-') end++;
				while (end < s.size() && std::isdigit(s[end])) end++;
			}
			double num = std::stod(s.substr(pos, end - pos));
			pos = end;
			return Json::Number(num);
		}

		Json parse_value(const std::string& s, size_t& pos) {
			skip_ws(s, pos);
			if (pos >= s.size()) throw std::runtime_error("Unexpected end");

			switch (s[pos]) {
			case '{': return parse_object(s, pos);
			case '[': return parse_array(s, pos);
			case '"': return parse_string(s, pos);
			case 't': case 'f': return parse_bool(s, pos);
			case 'n': return parse_null(s, pos);
			default:  return parse_number(s, pos);
			}
		}
	}

	Json Json::parse(const std::string& s) {
		size_t pos = 0;
		return parse_value(s, pos);
	}
	bool Json::has(const std::string& key) const {
		if (auto obj = dynamic_cast<JsonObject*>(val.get())) {
			return obj->has(key);
		}
		throw std::runtime_error("Not an object");
	}

	bool Json::isNull() const {
		return val->type() == JsonType::Null;
	}

	size_t Json::size() const {
		if (auto arr = dynamic_cast<JsonArray*>(val.get())) {
			return arr->size();
		}
		if (auto obj = dynamic_cast<JsonObject*>(val.get())) {
			return obj->size();
		}
		throw std::runtime_error("Not a container type");
	}

}


F_Lua::F_Lua() {
	L = luaL_newstate();
	if (!L) {
		throw std::runtime_error("Failed to create Lua state");
	}
	luaL_openlibs(L); // 加载标准库
}

F_Lua::~F_Lua() {
	if (L) {
		lua_close(L);
	}
}

bool F_Lua::ExecuteScript(const std::string& filepath) {
	int status = luaL_dofile(L, filepath.c_str());
	if (status != LUA_OK) {
		HandleError(status);
		return false;
	}
	return true;
}

bool F_Lua::ExecuteString(const std::string& code) {
	int status = luaL_dostring(L, code.c_str());
	if (status != LUA_OK) {
		HandleError(status);
		return false;
	}
	return true;
}

void F_Lua::HandleError(int status) {
	if (status != LUA_OK) {
		const char* errorMsg = lua_tostring(L, -1);
		std::cerr << "[Lua Error] " << errorMsg << std::endl;
		lua_pop(L, 1); // 清除错误信息
	}
}