#pragma once

#ifdef PladeParser_EXPORTS
#define PladeParser_API __declspec(dllexport)
#else
#define PladeParser_API __declspec(dllimport)
#endif
#include "windows.h"


extern "C" PladeParser_API bool fnPladeParser(LPCWSTR fileName);
extern "C" PladeParser_API void OpenConsole(void);