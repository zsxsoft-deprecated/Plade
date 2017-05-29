#ifndef __EXPORTS_GLOBALEXPORT_H_
#define __EXPORTS_GLOBALEXPORT_H_
#include <string>
#include <functional>

#include "clang-c/Index.h"

namespace PladeParser {
  namespace Exports {
	void GetMainFile(const char* fileName);
    std::string ParseCode(const char*);
    const char* GetClangVersion();
    bool TerminateParser();
  }
}
#endif