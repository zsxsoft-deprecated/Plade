#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PladeParser_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PladeParser_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PladeParser_EXPORTS
#define PladeParser_API __declspec(dllexport)
#else
#define PladeParser_API __declspec(dllimport)
#endif

// This is an example of a class exported from the PladeParser.dll
class PladeParser_API CPladeParser
{
public:
    CPladeParser();
    // TODO: add your methods here.
};

// This is an example of an exported variable
extern PladeParser_API int nPladeParser;

// This is an example of an exported function.
PladeParser_API int fnPladeParser(void);
