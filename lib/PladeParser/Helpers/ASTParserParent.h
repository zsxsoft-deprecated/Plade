#ifndef __HELPERS_ASTPARSERPARENT_H_
#define __HELPERS_ASTPARSERPARENT_H_
#include <clang-c/Index.h>
#include <cstdlib>
#include <cstring>
#include <stack>

namespace PladeParser {
	namespace Helpers {
		class ASTParserParent {
		protected:
			std::stack<char*> tempStringObjects;
			~ASTParserParent() {
				while (!tempStringObjects.empty()) {
					auto ptr = tempStringObjects.top();
					tempStringObjects.pop();
					delete[] ptr;
				}
			}

			const char* GetTextWrapper(CXString originalObject) {
				char* newString = nullptr;
				auto originalString = clang_getCString(originalObject);
				auto len = strlen(originalString) + 1;
				newString = new char[len];
				strncpy_s(newString, len, originalString, len);
				clang_disposeString(originalObject);
				tempStringObjects.push(newString);
				return newString;
			}
		};
	}
}
#endif