#pragma once
#ifdef LINK_WINFUNS

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"Kernel32.lib")
extern "C" void* GetConsoleWindow();
extern "C" void ShowWindow(void* hwnd, int);
extern "C" void MessageBoxA(void* hwnd, const char* a, const char* b, int mode);
extern "C" void MessageBoxW(void* hwnd, const char* a, const char* b, int mode);
extern "C" void SetWindowTextW(void* hwnd, const wchar_t* a);
extern "C" void SetWindowTextA(void* hwnd, const char* a);
extern "C" void SetConsoleCP(unsigned int cp);
extern "C" void SetConsoleOutputCP(unsigned int cp);
extern "C" void ZeroMemory(void* memory, int size);
#define MessageBox MessageBoxA
typedef unsigned long ULONG;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef int                 INT;
typedef unsigned int        UINT;
typedef void* HANDLE;

typedef short SHORT;
typedef wchar_t WCHAR;
typedef struct _COORD {
	SHORT X;
	SHORT Y;
} COORD, * PCOORD;

typedef struct _CONSOLE_FONT_INFOEX {
	ULONG cbSize;
	DWORD nFont;
	COORD dwFontSize;
	UINT  FontFamily;
	UINT  FontWeight;
	WCHAR FaceName[32];
} CONSOLE_FONT_INFOEX, * PCONSOLE_FONT_INFOEX;

#define STD_OUTPUT_HANDLE ((DWORD)-11)
extern "C" HANDLE GetStdHandle(DWORD nStdHandle);
extern "C" BOOL SetCurrentConsoleFontEx(HANDLE hConsoleOutput,BOOL  bMaximumWindow,PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
#endif