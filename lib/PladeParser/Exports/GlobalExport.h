#ifndef __EXPORTS_GLOBALEXPORT_H_
#define __EXPORTS_GLOBALEXPORT_H_
namespace PladeParser {
  namespace Exports {
    void OpenConsole();
    const char* fnPladeParser(const char*);
    const char* GetClangVersion();
    bool TerminateParser();
  }
}
#endif