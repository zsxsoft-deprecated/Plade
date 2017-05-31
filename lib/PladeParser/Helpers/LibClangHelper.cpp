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

		bool detectIsCppFile(std::string fileName) {
			std::experimental::filesystem::path path(fileName);
			return find(cppExtensions.begin(), cppExtensions.end(), path.extension().string()) != cppExtensions.end();
		}

		std::vector<std::string> getExistsExtensions(const char* fileName) {
			namespace fs = std::experimental::filesystem;
			std::vector<std::string> ret;
			fs::path path(fileName);
			for (auto i = 2; i != cppExtensions.size(); i++) { // ignore .h & .hpp
				if (exists(path.replace_extension(cppExtensions[i]))) {
					ret.push_back(path.string());
				}
			}
			return ret;
		}
	}
}