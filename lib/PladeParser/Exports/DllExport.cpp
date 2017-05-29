#include "GlobalExport.h"
#include "windows.h"
#include <cstdio>
#include "../ASTParser/ASTParser.h"

#ifdef _WIN32
__declspec(dllexport) void OpenConsole(void) {
	AllocConsole();
	auto out = stdout;
	freopen_s(&out, "CONOUT$", "w", stdout);
	freopen_s(&out, "CONOUT$", "w", stderr);
}


__declspec(dllexport) LPCWSTR fnPladeParser(LPCWSTR wCharfileName) {
	auto fileNameSize = WideCharToMultiByte(CP_UTF8, 0, wCharfileName, wcslen(wCharfileName), nullptr, 0, nullptr, nullptr) + 1;
	auto fileName = new char[fileNameSize];
	WideCharToMultiByte(CP_UTF8, 0, wCharfileName, wcslen(wCharfileName), fileName, fileNameSize, nullptr, nullptr);
	fileName[fileNameSize - 1] = '\0';

	auto returnString = PladeParser::Exports::ParseCode(fileName);
	auto returnData = returnString.c_str();

	auto wcharReturnSize = MultiByteToWideChar(CP_UTF8, 0, returnData, -1, nullptr, 0);
	auto wstr = new wchar_t[wcharReturnSize + 1];
	MultiByteToWideChar(CP_UTF8, 0, returnData, -1, wstr, wcharReturnSize);
	// delete[] wstr;
//	delete[] fileName;
//	delete[] returnData;
	PladeParser::ASTParser::Terminate();
	return wstr; // @todo may leak memory
}

__declspec(dllexport) LPCSTR GetClangVersion() {
	return PladeParser::ASTParser::GetClangVersion();
}

__declspec(dllexport) bool TerminateParser() {
	PladeParser::ASTParser::Terminate();
	return true;
}
#endif