#include "clang-c/Index.h"
#include "rapidjson/document.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "../ASTParser/ASTParser.h"
#include "GlobalExport.h"

namespace PladeParser {
	namespace Exports {
		
		std::string fnPladeParser(const char* fileName) {
			using namespace PladeParser;
			const char* returnData = nullptr;
			const char* const paramter[] = { "-ferror-limit=0", "-std=c++1z" };
			auto index = clang_createIndex(1, 1);
			auto unit = clang_parseTranslationUnit(
				index,
				fileName, &paramter[0], 2,
				nullptr, 0,
				CXTranslationUnit_CacheCompletionResults);

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

			while (true) {
				if (unit == nullptr) {
					break;
				}
				unsigned level = 0;
				auto cursor = clang_getTranslationUnitCursor(unit);
				ASTParser::Initialize();
				clang_visitChildren(cursor, ASTParser::visitChildrenCallback, &level);
				ASTParser::GetJSONDocument()->Accept(writer);
				returnData = buffer.GetString();
				break;
			}
			clang_disposeTranslationUnit(unit);
			clang_disposeIndex(index);
			
			auto ret =  std::string(returnData); 
			return ret;
		}

		const char* GetClangVersion() {
			return ASTParser::GetClangVersion();
		}

		bool TerminateParser() {
			ASTParser::Terminate();
			return true;
		}
	}
}