#ifndef __ASTPARSER_ASTPARSER_H_
#define __ASTPARSER_ASTPARSER_H_
#include <clang-c/Index.h>
#include <rapidjson/document.h>
#include "../Helpers/ASTParserParent.h"
#include <vector>

namespace PladeParser {

	class ASTParser: public Helpers::ASTParserParent {
	public:
		ASTParser();
		explicit ASTParser(std::vector<std::string>);
		~ASTParser();
		void GetSpell();
		void GetType();
		void GetLinkage();
		void GetParent();
		bool GetFileNameAndCheckCanWeContinue();
		void GetUsr();
		void GetCursorKind();
		void GetIncludedFile();
		static CXChildVisitResult visitChildrenCallback(CXCursor cursor, CXCursor parent, CXClientData client_data);
		const char* GetClangVersion();
		rapidjson::Document* GetJSONDocument() const;

		std::vector<std::string> visitedIncludeMap;

	protected:
		rapidjson::Document* ret;
		CXCursor* cursor = nullptr;
		CXCursor* parentCursor = nullptr;
		rapidjson::Value* singleItem = nullptr;
		std::vector<ASTParser*> subParsers;
		int currentLevel = 0;
	};
}
#endif
