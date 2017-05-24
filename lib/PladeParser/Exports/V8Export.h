#pragma once

#ifdef PladeParser_EXPORTS
#define PladeParser_API __declspec(dllexport)
#else
#define PladeParser_API __declspec(dllimport)
#endif
#include "windows.h"


extern "C" {
	__declspec(dllexport) LPCWSTR fnPladeParser(LPCWSTR fileName);
	__declspec(dllexport) void OpenConsole();
	__declspec(dllexport) bool TerminateParser();
	__declspec(dllexport) LPCSTR GetClangVersion();
}