#pragma once

#ifdef PladeParser_EXPORTS
#define PladeParser_API __declspec(dllexport)
#else
#define PladeParser_API __declspec(dllimport)
#endif
#include "windows.h"


extern "C" {
	PladeParser_API LPCWSTR fnPladeParser(LPCWSTR fileName);
	PladeParser_API void OpenConsole();
	PladeParser_API bool TerminateParser();
	PladeParser_API LPCSTR GetClangVersion();
}