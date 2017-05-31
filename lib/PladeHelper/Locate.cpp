#include "Locate.h"
#include <experimental/filesystem>
#include <functional>
#include <vector>

namespace PladeHelper {
	namespace Locate {

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

		const char* LocateToUTF8(const char* fileNameChar) {
#ifdef _WIN32
			auto wcharReturnSize = MultiByteToWideChar(CP_ACP, 0, fileNameChar, -1, nullptr, 0);
			auto wcharFileName = new wchar_t[wcharReturnSize + 1];
			MultiByteToWideChar(CP_ACP, 0, fileNameChar, -1, wcharFileName, wcharReturnSize);

			auto fileNameSize = WideCharToMultiByte(CP_UTF8, 0, wcharFileName, wcharReturnSize, nullptr, 0, nullptr, nullptr) + 1;
			auto fileName = new char[fileNameSize];
			WideCharToMultiByte(CP_UTF8, 0, wcharFileName, wcharReturnSize, fileName, fileNameSize, nullptr, nullptr);
			fileName[fileNameSize - 1] = '\0';
			return fileName;
#else
			return fileNameChar;
#endif
		}

	}
}