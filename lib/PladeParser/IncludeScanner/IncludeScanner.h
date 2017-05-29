#ifndef __INCLUDESCANNER_INCLUDESCANNER_H_
#define __INCLUDESCANNER_INCLUDESCANNER_H_
#include <clang-c/Index.h>
namespace PladeParser {
	namespace IncludeScanner {
		void FindMainFile(const char* directionaryPath);
	}
}
#endif