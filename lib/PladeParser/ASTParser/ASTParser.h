#ifndef __ASTPARSER_ASTPARSER_H_
#define __ASTPARSER_ASTPARSER_H_
#include <clang-c/Index.h>
#include <rapidjson/document.h>
#include "../Helpers/ASTParserParent.h"

namespace PladeParser {

	class ASTParser: public Helpers::ASTParserParent {
	public:
		ASTParser();
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

	protected:
		rapidjson::Document* ret;
		CXCursor* cursor = nullptr;
		CXCursor* parentCursor = nullptr;
		rapidjson::Value* singleItem = nullptr;
		int currentLevel = 0;
	};
}
#endif
