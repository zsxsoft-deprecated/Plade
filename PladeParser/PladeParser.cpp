// PladeParser.cpp : Defines the exported functions for the DLL application.
//

#include "PladeParser.h"
#include "../third-party/clang/include/clang-c/Index.h"
#include <cstdlib>
#include <ostream>
#include <iostream>

// This is an example of an exported variable
PladeParser_API int nPladeParser = 0;

// This is an example of an exported function.
PladeParser_API int fnPladeParser(void) {
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit = clang_parseTranslationUnit(
		index,
		"header.hpp", nullptr, 0,
		nullptr, 0,
		CXTranslationUnit_None);
	if (unit == nullptr) {
		std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
		exit(-1);
	}

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
	return 42;
}

// This is the constructor of a class that has been exported.
// see PladeParser.h for the class definition
CPladeParser::CPladeParser() {
	return;
}
