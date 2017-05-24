#include "clang-c/Index.h"
#include "rapidjson/document.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "../ASTParser/ASTParser.h"
#include "GlobalExport.h"

namespace PladeParser {
	namespace Exports {
		
		const char* fnPladeParser(const char* fileName) {
			using namespace PladeParser;
			const char* returnData = nullptr;
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

			// delete[] wstr;
		//	delete[] fileName;
		//	delete[] returnData;
			return returnData; // @todo may leak memory
		}

		const char* GetClangVersion() {
			return PladeParser::ASTParser::GetClangVersion();
		}

		bool TerminateParser() {
			PladeParser::ASTParser::Terminate();
			return true;
		}
	}
}