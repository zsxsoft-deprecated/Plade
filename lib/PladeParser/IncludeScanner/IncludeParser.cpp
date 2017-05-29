#include <vector>
#include <string>
#include <clang-c/Index.h>
#include <experimental/filesystem>
#include "../Helpers/LibClangHelper.h"

namespace PladeParser {
	namespace IncludeScanner {
		using namespace std;
		using namespace experimental;
		vector<string> files;

		void Initialize() {
			files.clear();
		}

		CXChildVisitResult IncludeVisitCallback(CXCursor cursor, CXCursor parent, CXClientData client_data) {
			while (true) {
				if (clang_getCursorKind(cursor) != CXCursor_InclusionDirective) break;
				if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor)) == 0) break;// return CXChildVisit_Break;
				auto file = clang_getIncludedFile(cursor);
				if (file == nullptr) break;
				auto filename = clang_getFileName(file);
				auto cString = clang_getCString(filename);
				if (Helpers::isInSystemInclude(cString)) break;
				auto stdString = string(cString);
				filesystem::path p(stdString);
				files.push_back(canonical(p).string());
				clang_disposeString(filename);
				break;
			}
			return CXChildVisit_Continue;
		}

		vector<string> GetData() {
			return files;
		}

		void Terminate() {
			files.clear();
		}
	}
}
