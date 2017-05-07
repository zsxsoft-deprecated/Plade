// PladeParser.cpp : Defines the exported functions for the DLL application.
//
#ifdef _WINDOWS
#include "DllExport.h"
#include "../third-party/clang/include/clang-c/Index.h"
#include "windows.h"
#include <cstdlib>
#include <ostream>
#include <iostream>

std::ostream& operator<<(std::ostream& stream, const CXString& str) {
	stream << clang_getCString(str);
	clang_disposeString(str);
	return stream;
}

PladeParser_API void OpenConsole(void) {
	AllocConsole();
	auto out = stdout;
	freopen_s(&out, "CONOUT$", "w", stdout);
}

PladeParser_API bool fnPladeParser(LPCWSTR wCharfileName) {
	auto returnFlag = true;
	auto fileNameSize = WideCharToMultiByte(CP_UTF8, 0, wCharfileName, wcslen(wCharfileName), nullptr, 0, nullptr, nullptr) + 1;
	auto fileName = new char[fileNameSize];
	WideCharToMultiByte(CP_UTF8, 0, wCharfileName, wcslen(wCharfileName), fileName, fileNameSize, nullptr, nullptr);
	fileName[fileNameSize - 1] = '\0';

	auto index = clang_createIndex(0, 0);
	auto unit = clang_parseTranslationUnit(
		index,
		fileName, nullptr, 0,
		nullptr, 0,
		CXTranslationUnit_None);

	while (true) {
		if (unit == nullptr) {
			returnFlag = false;
			break;
		}

		auto cursor = clang_getTranslationUnitCursor(unit);
		clang_visitChildren(
			cursor,
			[](CXCursor c, CXCursor parent, CXClientData client_data) {
			std::cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
				<< clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
			return CXChildVisit_Recurse;
		},
			nullptr);
		break;

	}

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
	return returnFlag;
}


#endif