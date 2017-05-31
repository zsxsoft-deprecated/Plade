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

			// @todo Performance issue here
			// It still finds the header file in SDK.
			// And I can't find a way to control it.
			// Anyhow, I have to add a visited map here.
			while (true) {
				auto loc = clang_getCursorLocation(cursor);
				if (clang_getCursorKind(cursor) != CXCursor_InclusionDirective) break;
				{
					CXFile file;
					clang_getFileLocation(loc, &file, nullptr, nullptr, nullptr);
					if (file == nullptr) break;
					auto filename = clang_getFileName(file);
					auto cString = clang_getCString(filename);
					if (Helpers::isInSystemInclude(cString)) break;
				}
				if (clang_Location_isFromMainFile(loc) == 0) break;// return CXChildVisit_Break;
				auto file = clang_getIncludedFile(cursor);
				if (file == nullptr) break;
				auto filename = clang_getFileName(file);
				auto cString = clang_getCString(filename);
				printf("%s\n==============================", cString);
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
