#ifndef __EXPORTS_DLLEXPORT_H_
#define __EXPORTS_DLLEXPORT_H_
#ifdef _WINDOWS
#include "windows.h"
extern "C" {
	__declspec(dllexport) LPCWSTR fnPladeParser(LPCWSTR fileName);
	__declspec(dllexport) void OpenConsole();
	__declspec(dllexport) bool TerminateParser();
	__declspec(dllexport) LPCSTR GetClangVersion();
}
#endif
#endif