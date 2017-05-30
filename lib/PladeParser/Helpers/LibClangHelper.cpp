#include "LibClangHelper.h"
#include <clang-c/Index.h>
#include <experimental/filesystem>
#include <functional>
#include <vector>

namespace PladeParser {
	namespace Helpers {

		std::vector<std::string> cppExtensions = {
			".h",
			".hpp",
			".c",
			".cpp",
			".c++",
			".cc",
			".cxx",
			".c11",
			".m" // Objective-C
		};

		const char* UTF8ToLocate(const char* fileNameChar) {
#ifdef _WIN32
			std::wstring_convert<std::codecvt_utf8<wchar_t>> cv1(new std::codecvt_utf8<wchar_t>());
			auto wStrFileName = cv1.from_bytes(fileNameChar);
			auto wcharFileName = wStrFileName.c_str();
			auto fileNameSize = WideCharToMultiByte(CP_ACP, 0, wcharFileName, wcslen(wcharFileName), nullptr, 0, nullptr, nullptr) + 1;
			auto fileName = new char[fileNameSize];
			WideCharToMultiByte(CP_ACP, 0, wcharFileName, wcslen(wcharFileName), fileName, fileNameSize, nullptr, nullptr);
			fileName[fileNameSize - 1] = '\0';
			return fileName;
#else
			return fileNameChar;
#endif
		}

		bool detectIsCppFile(std::string fileName) {
			std::experimental::filesystem::path path(fileName);
			return find(cppExtensions.begin(), cppExtensions.end(), path.extension().string()) != cppExtensions.end();
		}

		std::vector<std::string> getExistsExtensions(std::string fileName) {
			namespace fs = std::experimental::filesystem;
			std::vector<std::string> ret;
			fs::path path(fileName);
			for (auto i = 0; i != cppExtensions.size(); i++) {
				if (exists(path.replace_extension(cppExtensions[i]))) {
					ret.push_back(path.string());
				}
			}
			return ret;
		}
	}
}