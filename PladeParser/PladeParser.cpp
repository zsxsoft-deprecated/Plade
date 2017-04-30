// PladeParser.cpp : Defines the exported functions for the DLL application.
//

#include "PladeParser.h"

// This is an example of an exported variable
PladeParser_API int nPladeParser = 0;

// This is an example of an exported function.
PladeParser_API int fnPladeParser(void) {
	return 42;
}

// This is the constructor of a class that has been exported.
// see PladeParser.h for the class definition
CPladeParser::CPladeParser() {
	return;
}
