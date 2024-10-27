#include "F_Console.h"
void Init_FConsole() {
    using namespace WinFuns;
    // 控制台显示乱码纠正
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    CONSOLE_FONT_INFOEX info = { 0 }; // 以下设置字体
    info.cbSize = sizeof(info);
    info.dwFontSize.Y = 16; // leave X as zero
    info.FontWeight = 500;
    wcscpy(info.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}

