#ifdef _WINDOWS
#include "GlobalExport.h"
#include "windows.h"

__declspec(dllexport) void OpenConsole(void) {
	AllocConsole();
	auto out = stdout;
	freopen_s(&out, "CONOUT$", "w", stdout);
}

__declspec(dllexport) LPCWSTR fnPladeParser(LPCWSTR wCharfileName) {
	const char* returnData = nullptr;
	auto fileNameSize = WideCharToMultiByte(CP_UTF8, 0, wCharfileName, wcslen(wCharfileName), nullptr, 0, nullptr, nullptr) + 1;
	auto fileName = new char[fileNameSize];
	WideCharToMultiByte(CP_UTF8, 0, wCharfileName, wcslen(wCharfileName), fileName, fileNameSize, nullptr, nullptr);
	fileName[fileNameSize - 1] = '\0';

	auto returnData = PladeParser::Exports::fnPladeParser(fileName);

	auto wcharReturnSize = MultiByteToWideChar(CP_UTF8, 0, returnData, -1, nullptr, 0);
	auto wstr = new wchar_t[wcharReturnSize + 1];
	MultiByteToWideChar(CP_UTF8, 0, returnData, -1, wstr, wcharReturnSize);
	// delete[] wstr;
//	delete[] fileName;
//	delete[] returnData;
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