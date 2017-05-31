#ifndef __ASTPARSER_ASTPARSER_H_
#define __ASTPARSER_ASTPARSER_H_
#include <clang-c/Index.h>
#include <rapidjson/document.h>
#include "../Helpers/ASTParserParent.h"
#include <vector>
#include <set>
#include <memory>

namespace PladeParser {

	class ASTParser : public Helpers::ASTParserParent {
	public:
		ASTParser();
		explicit ASTParser(
			std::shared_ptr<std::set<std::string>>,
			std::shared_ptr<std::set<std::string>>,
			std::shared_ptr<std::vector<std::string>>
		);
		~ASTParser();
		void GetSpell();
		void GetType();
		void GetLinkage();
		void GetParent();
		bool GetFileNameAndCheckCanWeContinue();
		void GetUsr();
		void GetCursorKind();
		void AddToIncludeMap();
		void ManualLinker();
		static CXChildVisitResult visitChildrenCallback(CXCursor cursor, CXCursor parent, CXClientData client_data);
		const char* GetClangVersion();
		rapidjson::Document* GetJSONDocument() const;

		std::shared_ptr<std::set<std::string>> includeMap;
		std::shared_ptr<std::set<std::string>> uselessIncludeMap;
		std::shared_ptr<std::vector<std::string>> visitedIncludeMap;
		bool linkData = false;
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
