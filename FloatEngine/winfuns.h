#pragma once
#ifdef LINK_WINFUNS

#ifndef CALLBACK
#define CALLBACK __stdcall
#endif
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
namespace WinFuns {
#define MAKEINTRESOURCEA(i) ((const char*)((ULONG_PTR)((WORD)(i))))
#define MAKEINTRESOURCEW(i) ((const wchar_t*)((ULONG_PTR)((WORD)(i))))
#define MAKEINTRESOURCE MAKEINTRESOURCEA
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
	DECLARE_HANDLE(HWND);
#ifdef _WIN64
	typedef __int64 LONG_PTR;
	typedef unsigned __int64 UINT_PTR;
	typedef unsigned __int64 ULONG_PTR;
	typedef UINT_PTR WPARAM;
	typedef LONG_PTR LPARAM;
	typedef LONG_PTR LRESULT;
#else
	typedef long LONG_PTR;
	typedef unsigned int UINT_PTR;
	typedef unsigned long ULONG_PTR;
	typedef unsigned int WPARAM;
	typedef long LPARAM;
	typedef long LRESULT;
#endif

	extern "C" {
		void* GetConsoleWindow();
		void ShowWindow(void* hwnd, int);
		void MessageBoxA(void* hwnd, const char* a, const char* b, int mode);
		void MessageBoxW(void* hwnd, const char* a, const char* b, int mode);
		void SetWindowTextW(void* hwnd, const wchar_t* a);
		void SetWindowTextA(void* hwnd, const char* a);
		void SetConsoleCP(unsigned int cp);
		void SetConsoleOutputCP(unsigned int cp);
		void ZeroMemory(void* memory, int size);
		void* LoadIconW(void* , const wchar_t* );
		void* LoadIconA(void*, const char*);
		void*  GetModuleHandleA(const char* lpModuleName);
		LRESULT SendMessageA(
			HWND   hWnd,
			UINT   Msg,
			WPARAM wParam,
			LPARAM lParam
		);
	}
#define LoadIcon LoadIconA
#define MessageBox MessageBoxA
#define GetModuleHandle GetModuleHandleA
#define SendMessage SendMessageA
#define STD_OUTPUT_HANDLE ((DWORD)-11)
	extern "C" HANDLE GetStdHandle(DWORD nStdHandle);
	extern "C" BOOL SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL  bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);

}
#endif
