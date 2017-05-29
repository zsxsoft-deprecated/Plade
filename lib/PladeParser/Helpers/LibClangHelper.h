#ifndef __HELPERS_LIBCLANGHELPER_H_
#define __HELPERS_LIBCLANGHELPER_H_
#include <functional>
#include <vector>
#include <clang-c/Index.h>

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
			unsigned CXTranslationUnitFlags = CXTranslationUnit_CacheCompletionResults | CXTranslationUnit_DetailedPreprocessingRecord
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


		/**
		 * \brief Detect a file is a C++/C/ObjC Source file by filename
		 * \param fileName
		 * \return
		 */
		bool detectIsCppFile(std::string fileName);

		/**
		 * \brief Find a list of filename which is existed.
		 * \param fileName
		 * \return
		 */
		std::vector<std::string> getExistsExtensions(std::string fileName);
	}
}
#endif