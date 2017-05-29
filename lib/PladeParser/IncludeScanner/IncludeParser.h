#ifndef __INCLUDESCANNER_ASTPARSER_H_
#define __INCLUDESCANNER_ASTPARSER_H_
#include <clang-c/Index.h>
#include <vector>
#include <string>
namespace PladeParser {
	namespace IncludeScanner {
		void Initialize();
		std::vector<std::string> GetData();
		void Terminate();
		CXChildVisitResult IncludeVisitCallback(CXCursor cursor, CXCursor parent, CXClientData client_data);
	}
}
#endif