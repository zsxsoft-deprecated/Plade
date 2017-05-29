#ifndef __EXPORTS_GLOBALEXPORT_H_
#define __EXPORTS_GLOBALEXPORT_H_
#include <vector>
#include <functional>

namespace PladeParser {
  namespace Exports {
	std::vector<std::string> FindMainFile(const char* fileName);
    std::string ParseCode(const char*);
    const char* GetClangVersion();
    bool TerminateParser();
  }
}
#endif