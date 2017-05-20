#include "ASTParser.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#ifdef _WINDOWS
#include "DllExport.h"
#include "clang-c/Index.h"
#include "rapidjson/document.h"
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

PladeParser_API LPCWSTR fnPladeParser(LPCWSTR wCharfileName) {
	using namespace PladeParser;
	const char* returnData = nullptr;
	auto fileNameSize = WideCharToMultiByte(CP_UTF8, 0, wCharfileName, wcslen(wCharfileName), nullptr, 0, nullptr, nullptr) + 1;
	auto fileName = new char[fileNameSize];
	WideCharToMultiByte(CP_UTF8, 0, wCharfileName, wcslen(wCharfileName), fileName, fileNameSize, nullptr, nullptr);
	fileName[fileNameSize - 1] = '\0';
	const auto paramter = "";

	auto index = clang_createIndex(1, 1);
	auto unit = clang_parseTranslationUnit(
		index,
		fileName, &paramter, 0,
		nullptr, 0,
		CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_ForSerialization);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	while (true) {
		if (unit == nullptr) {
			break;
		}
		unsigned level = 0;
		clang_saveTranslationUnit(unit, "Z:\\1233.txt", 0);
		auto cursor = clang_getTranslationUnitCursor(unit);
		ASTParser::Initialize();
		clang_visitChildren(cursor, ASTParser::visitChildrenCallback, &level);
		ASTParser::GetJSONDocument()->Accept(writer);
		returnData = buffer.GetString();
		break;
	}
	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
	auto wcharReturnSize = MultiByteToWideChar(CP_UTF8, 0, returnData, -1, nullptr, 0);
	auto wstr = new wchar_t[wcharReturnSize + 1];
	MultiByteToWideChar(CP_UTF8, 0, returnData, -1, wstr, wcharReturnSize);
	// delete[] wstr;
//	delete[] fileName;
//	delete[] returnData;
	return wstr; // @todo may leak memory
}

PladeParser_API LPCSTR GetClangVersion() {
	return PladeParser::ASTParser::GetClangVersion();
}

PladeParser_API bool TerminateParser() {
	PladeParser::ASTParser::Terminate();
	return true;
}
#endif