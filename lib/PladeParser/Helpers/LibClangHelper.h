#ifndef __HELPERS_LIBCLANGHELPER_H_
#define __HELPERS_LIBCLANGHELPER_H_
#include <functional>

#define LIBCLANGHELPER_USINGCHARDESTRUCTOR \
	std::stack<char*> tempStringObjects;\
	void TempStringDestructor() {\
		while (!tempStringObjects.empty()) {\
			auto ptr = tempStringObjects.top();\
			tempStringObjects.pop();\
			delete[] ptr;\
	    }\
	}

#define LIBCLANGHELPER_WRAPTEXTTOCHAR(originalObject, newString)\
char* newString;\
	{\
auto originalString = clang_getCString(originalObject);\
auto len = strlen(originalString) + 1;\
newString = new char[len];\
strncpy_s(newString, len, originalString, len);\
clang_disposeString(originalObject);\
tempStringObjects.push(newString);\
	}

namespace PladeParser {
	namespace Helpers {
		/**
		 * \brief Parse a Clang Unit to callback function
		 * \tparam T
		 * \param fileName
		 * \param callback
		 * \param CXTranslationUnitFlags
		 * \return
		 */
		template<typename T> T OpenClangUnit(
			const char* fileName,
			std::function<T(CXTranslationUnit)> callback,
			unsigned CXTranslationUnitFlags = CXTranslationUnit_CacheCompletionResults
		) {
			using namespace PladeParser;
			T returnData;
			const char* const paramter[] = { "-ferror-limit=0", "-std=c++1z" };
			auto index = clang_createIndex(1, 1);
			auto unit = clang_parseTranslationUnit(index, fileName, &paramter[0], 2, nullptr, 0, CXTranslationUnitFlags);
			while (true) {
				if (unit == nullptr) break;
				returnData = callback(unit);
				break;
			}
			clang_disposeTranslationUnit(unit);
			clang_disposeIndex(index);
			return returnData;
		}

		/**
		 * \brief Detect whether the included file is from system.
		 * \param fileName
		 * \return
		 */
		inline bool isInSystemInclude(const char* fileName) {
			if (strstr(fileName, "Windows Kits") != nullptr || strstr(fileName, "/usr/include/") != nullptr) {
				return true;
			}
			return false;
		}
	}
}
#endif