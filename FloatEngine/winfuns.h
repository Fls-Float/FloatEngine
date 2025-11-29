#pragma once
#ifndef WINFUNS_H_INCLUDED
#define WINFUNS_H_INCLUDED

#ifdef LINK_WINFUNS
#include "rlgl.h"
#pragma comment(lib,"iphlpapi.lib")
#ifndef CALLBACK
#define CALLBACK __stdcall
#endif
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#ifndef DECLARE_HANDLE
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
#endif
namespace WinFuns {
#ifndef MAKEINTRESOURCEA
#define MAKEINTRESOURCEA(i) ((const char*)((ULONG_PTR)((WORD)(i))))
#endif
#ifndef MAKEINTRESOURCEW
#define MAKEINTRESOURCEW(i) ((const wchar_t*)((ULONG_PTR)((WORD)(i))))
#endif
#ifndef MAKEINTRESOURCE
#define MAKEINTRESOURCE MAKEINTRESOURCEA
#endif

#ifndef WINFUNS_HAVE_ULONG
typedef unsigned long ULONG;
#define WINFUNS_HAVE_ULONG
#endif

#ifndef WINFUNS_HAVE_DWORD
typedef unsigned long       DWORD;
#define WINFUNS_HAVE_DWORD
#endif

#ifndef WINFUNS_HAVE_BOOL
typedef int                 BOOL;
#define WINFUNS_HAVE_BOOL
#endif

#ifndef WINFUNS_HAVE_BYTE
typedef unsigned char       BYTE;
#define WINFUNS_HAVE_BYTE
#endif

#ifndef WINFUNS_HAVE_WORD
typedef unsigned short      WORD;
#define WINFUNS_HAVE_WORD
#endif

#ifndef WINFUNS_HAVE_INT
typedef int                 INT;
#define WINFUNS_HAVE_INT
#endif

#ifndef WINFUNS_HAVE_UINT
typedef unsigned int        UINT;
#define WINFUNS_HAVE_UINT
#endif

#ifndef WINFUNS_HAVE_HANDLE
typedef void* HANDLE;
#define WINFUNS_HAVE_HANDLE
#endif

#ifndef WINFUNS_HAVE_SHORT
typedef short SHORT;
#define WINFUNS_HAVE_SHORT
#endif

#ifndef WINFUNS_HAVE_WCHAR
typedef wchar_t WCHAR;
#define WINFUNS_HAVE_WCHAR
#endif

#ifndef WINFUNS_HAVE_COORD
typedef struct _COORD {
	SHORT X;
	SHORT Y;
} COORD, * PCOORD;
#define WINFUNS_HAVE_COORD
#endif

#ifndef WINFUNS_HAVE_CONSOLE_FONT_INFOEX
typedef struct _CONSOLE_FONT_INFOEX {
	ULONG cbSize;
	DWORD nFont;
	COORD dwFontSize;
	UINT  FontFamily;
	UINT  FontWeight;
	WCHAR FaceName[32];
} CONSOLE_FONT_INFOEX, * PCONSOLE_FONT_INFOEX;
#define WINFUNS_HAVE_CONSOLE_FONT_INFOEX
#endif

#ifndef WINFUNS_HAVE_HWND
DECLARE_HANDLE(HWND);
#define WINFUNS_HAVE_HWND
#endif

#ifdef _WIN64
#ifndef WINFUNS_HAVE_LONG_PTR
typedef __int64 LONG_PTR;
#define WINFUNS_HAVE_LONG_PTR
#endif
#ifndef WINFUNS_HAVE_UINT_PTR
typedef unsigned __int64 UINT_PTR;
#define WINFUNS_HAVE_UINT_PTR
#endif
#ifndef WINFUNS_HAVE_ULONG_PTR
typedef unsigned __int64 ULONG_PTR;
#define WINFUNS_HAVE_ULONG_PTR
#endif
#ifndef WINFUNS_HAVE_WPARAM
typedef UINT_PTR WPARAM;
#define WINFUNS_HAVE_WPARAM
#endif
#ifndef WINFUNS_HAVE_LPARAM
typedef LONG_PTR LPARAM;
#define WINFUNS_HAVE_LPARAM
#endif
#ifndef WINFUNS_HAVE_LRESULT
typedef LONG_PTR LRESULT;
#define WINFUNS_HAVE_LRESULT
#endif
#else
#ifndef WINFUNS_HAVE_LONG_PTR
typedef long LONG_PTR;
#define WINFUNS_HAVE_LONG_PTR
#endif
#ifndef WINFUNS_HAVE_UINT_PTR
typedef unsigned int UINT_PTR;
#define WINFUNS_HAVE_UINT_PTR
#endif
#ifndef WINFUNS_HAVE_ULONG_PTR
typedef unsigned long ULONG_PTR;
#define WINFUNS_HAVE_ULONG_PTR
#endif
#ifndef WINFUNS_HAVE_WPARAM
typedef unsigned int WPARAM;
#define WINFUNS_HAVE_WPARAM
#endif
#ifndef WINFUNS_HAVE_LPARAM
typedef long LPARAM;
#define WINFUNS_HAVE_LPARAM
#endif
#ifndef WINFUNS_HAVE_LRESULT
typedef long LRESULT;
#define WINFUNS_HAVE_LRESULT
#endif
#endif

#ifndef WINFUNS_HAVE_WSADATA
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
#define WINFUNS_HAVE_WSADATA
#endif

#ifndef WINFUNS_HAVE_DWORD_PTR
typedef ULONG_PTR DWORD_PTR, * PDWORD_PTR;
#define WINFUNS_HAVE_DWORD_PTR
#endif

#ifndef WINFUNS_HAVE_SOCKADDR
struct sockaddr {
	unsigned short sa_family;              /* address family */
	char    sa_data[14];            /* up to 14 bytes of direct address */
};
#define WINFUNS_HAVE_SOCKADDR
#endif

#ifndef MAKEWORD
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#endif

#ifndef _SA_SPECSTRIZE
#define _SA_SPECSTRIZE( x ) #x
#endif
#ifndef _SAL2_Source_
#define _SAL2_Source_(Name, args, annotes) _SA_annotes3(SAL_name, #Name, "", "2") _Group_(annotes _SAL_nop_impl_)
#endif
#ifndef _Field_size_bytes_
#define _Field_size_bytes_(size)                   _SAL2_Source_(_Field_size_bytes_, (size), _Notnull_   _Writable_bytes_(size))
#endif

#ifndef WINFUNS_HAVE_ADDRINFOA
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
#define WINFUNS_HAVE_ADDRINFOA
#endif

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
#ifndef LoadIcon
#define LoadIcon LoadIconA
#endif
#ifndef MessageBox
#define MessageBox MessageBoxA
#endif
#ifndef GetModuleHandle
#define GetModuleHandle GetModuleHandleA
#endif
#ifndef SendMessage
#define SendMessage SendMessageA
#endif
#ifndef STD_OUTPUT_HANDLE
#define STD_OUTPUT_HANDLE ((DWORD)-11)
#endif
#ifndef LPWSADATA
#define LPWSADATA WSADATA*
#endif
extern "C" int WSAStartup(
	  WORD      wVersionRequired,
	LPWSADATA lpWSAData
);
#ifndef WINFUNS_HAVE_IN_ADDR
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
#define WINFUNS_HAVE_IN_ADDR
#endif

#ifndef WINFUNS_HAVE_SOCKADDR_IN
struct sockaddr_in {
	short   sin_family;
	unsigned short sin_port;
	struct  in_addr sin_addr;
	char    sin_zero[8];
};
#define WINFUNS_HAVE_SOCKADDR_IN
#endif

#ifndef WINFUNS_HAVE_IN6_ADDR
typedef struct in6_addr {
	union {
		unsigned char       Byte[16];
		unsigned short      Word[8];
	} u;
} IN6_ADDR, * PIN6_ADDR, * LPIN6_ADDR;
#define WINFUNS_HAVE_IN6_ADDR
#endif

#ifndef WINFUNS_HAVE_SCOPE_ID
typedef struct {
	union {
		struct {
			ULONG Zone : 28;
			ULONG Level : 4;
		} DUMMYSTRUCTNAME;
		ULONG Value;
	} DUMMYUNIONNAME;
} SCOPE_ID, * PSCOPE_ID;
#define WINFUNS_HAVE_SCOPE_ID
#endif

#ifndef WINFUNS_HAVE_SOCKADDR_IN6
typedef struct sockaddr_in6 {
	unsigned short sin6_family; // AF_INET6.
	unsigned short sin6_port;           // Transport level port number.
	ULONG  sin6_flowinfo;       // IPv6 flow information.
	IN6_ADDR sin6_addr;         // IPv6 address.
	union {
		ULONG sin6_scope_id;     // Set of interfaces for a scope.
		SCOPE_ID sin6_scope_struct;
	} ;
} SOCKADDR_IN6_LH, * PSOCKADDR_IN6_LH, * LPSOCKADDR_IN6_LH;
#define WINFUNS_HAVE_SOCKADDR_IN6
#endif

#ifndef WINFUNS_HAVE_ADDRINFO
typedef ADDRINFOA ADDRINFO, * LPADDRINFO;
#define WINFUNS_HAVE_ADDRINFO
#endif

extern "C" int WSAGetLastError();
extern "C" HANDLE GetStdHandle(DWORD nStdHandle);
extern "C" BOOL SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL  bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
#ifndef DECLSPEC_IMPORT
#define DECLSPEC_IMPORT __declspec (dllimport)
#endif
#ifndef WINSOCK_API_LINKAGE
#define WINSOCK_API_LINKAGE	DECLSPEC_IMPORT
#endif
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

#endif // LINK_WINFUNS

#endif // WINFUNS_H_INCLUDED
