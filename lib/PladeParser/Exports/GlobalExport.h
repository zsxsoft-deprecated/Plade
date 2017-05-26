#ifndef __EXPORTS_GLOBALEXPORT_H_
#define __EXPORTS_GLOBALEXPORT_H_
#include <string>
namespace PladeParser {
  namespace Exports {
    void OpenConsole();
    std::string fnPladeParser(const char*);
    const char* GetClangVersion();
    bool TerminateParser();
  }
}
#endif