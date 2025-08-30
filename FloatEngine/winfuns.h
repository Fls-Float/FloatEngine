#pragma once
#ifdef LINK_WINFUNS
#include "rlgl.h"
#pragma comment(lib,"iphlpapi.lib")
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
	typedef struct WSAData {
		WORD                    wVersion;
		WORD                    wHighVersion;
#ifdef _WIN64
		unsigned short          iMaxSockets;
		unsigned short          iMaxUdpDg;
		char * lpVendorInfo;
		char                    szDescription[257];
		char                    szSystemStatus[257];
#else
		char                    szDescription[WSADESCRIPTION_LEN + 1];
		char                    szSystemStatus[WSASYS_STATUS_LEN + 1];
		unsigned short          iMaxSockets;
		unsigned short          iMaxUdpDg;
		char * lpVendorInfo;
#endif
	} WSADATA;

	typedef ULONG_PTR DWORD_PTR, * PDWORD_PTR;
	struct sockaddr {
		unsigned short sa_family;              /* address family */
		char    sa_data[14];            /* up to 14 bytes of direct address */
	};

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))

#define _SA_SPECSTRIZE( x ) #x
#define _SA_annotes3(n,pp1,pp2,pp3)    __declspec(#n "(" _SA_SPECSTRIZE(pp1) "," _SA_SPECSTRIZE(pp2) "," _SA_SPECSTRIZE(pp3) ")")
#define _SAL2_Source_(Name, args, annotes) _SA_annotes3(SAL_name, #Name, "", "2") _Group_(annotes _SAL_nop_impl_)
#define _Field_size_bytes_(size)                   _SAL2_Source_(_Field_size_bytes_, (size), _Notnull_   _Writable_bytes_(size))

	typedef struct addrinfo
	{
		int                 ai_flags;       // AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST
		int                 ai_family;      // PF_xxx
		int                 ai_socktype;    // SOCK_xxx
		int                 ai_protocol;    // 0 or IPPROTO_xxx for IPv4 and IPv6
		size_t              ai_addrlen;     // Length of ai_addr
		char* ai_canonname;   // Canonical name for nodename
		_Field_size_bytes_(ai_addrlen) struct sockaddr* ai_addr;        // Binary address
		struct addrinfo* ai_next;        // Next structure in linked list
	}
	ADDRINFOA, * PADDRINFOA;

	extern "C" {
#if !defined(GetConsoleWindow)
		void* GetConsoleWindow();
#endif
#if !defined(ShowWindow)
		void ShowWindow(void* hwnd, int);

#endif
#if !defined(MessageBoxA)
		void MessageBoxA(void* hwnd, const char* a, const char* b, int mode);
#endif
#if !defined(MessageBoxW)
		void MessageBoxW(void* hwnd, const char* a, const char* b, int mode);
#endif
#if (!defined(SetWindowTextW)&&!defined(SetWindowTextA))
		void SetWindowTextW(void* hwnd, const wchar_t* a);
		void SetWindowTextA(void* hwnd, const char* a);
#endif
#if (!defined(SetConsoleCP)&&!defined(SetConsoleOutputCP))
		void SetConsoleCP(unsigned int cp);
		void SetConsoleOutputCP(unsigned int cp);
#endif
#if (!defined(ZeroMemory))
		void ZeroMemory(void* memory, int size);
#endif
#if (!defined(LoadIconA)&&!defined(LoadIconW))
		void* LoadIconW(void* , const wchar_t* );
		void* LoadIconA(void*, const char*);
#endif
#if !defined(GetModuleHandleA)
		void*  GetModuleHandleA(const char* lpModuleName);
#endif
		LRESULT SendMessageA(
			HWND   hWnd,
			UINT   Msg,
			WPARAM wParam,
			LPARAM lParam
		);
		void* GetCapture();
	}
#define LoadIcon LoadIconA
#define MessageBox MessageBoxA
#define GetModuleHandle GetModuleHandleA
#define SendMessage SendMessageA
#define STD_OUTPUT_HANDLE ((DWORD)-11)
#define LPWSADATA WSADATA*
	extern "C" int WSAStartup(
		  WORD      wVersionRequired,
		LPWSADATA lpWSAData
	);
	struct in_addr {
		union {
			struct { unsigned char s_b1, s_b2, s_b3, s_b4; } S_un_b;
			struct { unsigned short s_w1, s_w2; } S_un_w;
			unsigned long S_addr;
		} S_un;
#define s_addr  S_un.S_addr
		/* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2
								/* host on imp */
#define s_net   S_un.S_un_b.s_b1
								/* network */
#define s_imp   S_un.S_un_w.s_w2
								/* imp */
#define s_impno S_un.S_un_b.s_b4
								/* imp # */
#define s_lh    S_un.S_un_b.s_b3
								/* logical host */
	};
	struct sockaddr_in {
		short   sin_family;
		unsigned short sin_port;
		struct  in_addr sin_addr;
		char    sin_zero[8];
	}; typedef struct in6_addr {
		union {
			unsigned char       Byte[16];
			unsigned short      Word[8];
		} u;
	} IN6_ADDR, * PIN6_ADDR, * LPIN6_ADDR;
	typedef struct {
		union {
			struct {
				ULONG Zone : 28;
				ULONG Level : 4;
			} DUMMYSTRUCTNAME;
			ULONG Value;
		} DUMMYUNIONNAME;
	} SCOPE_ID, * PSCOPE_ID;
	typedef struct sockaddr_in6 {
		unsigned short sin6_family; // AF_INET6.
		unsigned short sin6_port;           // Transport level port number.
		ULONG  sin6_flowinfo;       // IPv6 flow information.
		IN6_ADDR sin6_addr;         // IPv6 address.
		union {
			ULONG sin6_scope_id;     // Set of interfaces for a scope.
			SCOPE_ID sin6_scope_struct;
		};
	} SOCKADDR_IN6_LH, * PSOCKADDR_IN6_LH, * LPSOCKADDR_IN6_LH;

	typedef ADDRINFOA ADDRINFO, * LPADDRINFO;
	
	extern "C" int WSAGetLastError();
	extern "C" HANDLE GetStdHandle(DWORD nStdHandle);
	extern "C" BOOL SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL  bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
#define DECLSPEC_IMPORT __declspec (dllimport)
#define WINSOCK_API_LINKAGE	DECLSPEC_IMPORT
	//WINSOCK_API_LINKAGE int  getaddrinfo(const char* nodename, const char* servname, const struct addrinfo* hints, struct addrinfo** res);
	//WINSOCK_API_LINKAGE void freeaddrinfo(LPADDRINFO pAddrInfo);
	extern "C" const char*  inet_ntop(
		  int        Family,
		  const void* pAddr,
		 char*       pStringBuf,
		  size_t     StringBufSize
	);
	extern "C" int WSACleanup();
}
namespace LinuxFuns {

}


#endif
