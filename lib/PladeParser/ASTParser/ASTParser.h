#ifndef __ASTPARSER_ASTPARSER_H_
#define __ASTPARSER_ASTPARSER_H_
#include <clang-c/Index.h>
#include <rapidjson/document.h>

namespace PladeParser {

	namespace ASTParser {
		void Initialize();
		void Terminate();
		void GetSpell(rapidjson::Value& single, const CXCursor& cursor);
		void GetType(rapidjson::Value& single, const CXCursor& cursor);
		void GetLinkage(rapidjson::Value& single, const CXCursor& cursor);
		void GetParent(rapidjson::Value& single, const CXCursor& cursor, const CXCursor& parent);
		bool GetFileNameAndCheckCanWeContinue(rapidjson::Value& single, const CXCursor& cursor);
		void GetUsr(rapidjson::Value& single, const CXCursor& cursor);
		void GetCursorKind(rapidjson::Value& single, const CXCursor& cursor);
		void GetIncludedFile(rapidjson::Value& single, const CXCursor& cursor);
		CXChildVisitResult visitChildrenCallback(CXCursor cursor, CXCursor parent, CXClientData client_data);
		const char* GetClangVersion();
		rapidjson::Document* GetJSONDocument();
	}
}
#endif
