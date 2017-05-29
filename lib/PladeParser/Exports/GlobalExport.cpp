#include "GlobalExport.h"

#include "clang-c/Index.h"
#include "rapidjson/document.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <functional>
#include <vector>

#include "../ASTParser/ASTParser.h"
#include "../Helpers/LibClangHelper.h"
#include "../IncludeScanner/IncludeScanner.h"

namespace PladeParser {
	namespace Exports {

		void GetMainFile(const char* fileName) {
			IncludeScanner::FindMainFile(fileName);
		}

		std::string ParseCode(const char* fileName) {
			return Helpers::OpenClangUnit<std::string>(fileName, [](CXTranslationUnit unit) {
				unsigned level = 0;
				auto cursor = clang_getTranslationUnitCursor(unit);
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				ASTParser::Initialize();
				clang_visitChildren(cursor, ASTParser::visitChildrenCallback, &level);
				ASTParser::GetJSONDocument()->Accept(writer);
				return std::string(buffer.GetString());
			});
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
