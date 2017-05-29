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

		std::vector<std::string> FindMainFile(const char* fileName) {
			return IncludeScanner::FindMainFile(fileName);
		}

		std::string ParseCode(const char* fileName) {
			return Helpers::OpenClangUnit<std::string>(fileName, [](CXTranslationUnit unit) {
				auto cursor = clang_getTranslationUnitCursor(unit);
				auto parser = new ASTParser();
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				clang_visitChildren(cursor, ASTParser::visitChildrenCallback, parser);
				parser->GetJSONDocument()->Accept(writer);
				auto ret = std::string(buffer.GetString());
				delete parser;
				return ret;
			});
		}

		const char* GetClangVersion() {
			return ASTParser().GetClangVersion();
		}

		bool TerminateParser() {
			return true;
		}
	}
}
