#ifndef __PLADEHELPER_LOCATE_H_
#define __PLADEHELPER_LOCATE_H_
#include <functional>
#include <vector>
#ifdef _WIN32
#include <codecvt>
#include <cvt/wstring>
#include <windows.h>
#endif

namespace PladeHelper {
	namespace Locate {

		/**
		 * \brief UTF-8 char* to ANSI char* (Important in Windows)
		 * \param fileNameChar 
		 * \return 
		 */
		const char* UTF8ToLocate(const char* fileNameChar);
		/**
		* \brief ANSI char* to UTF-8 char* (Important in Windows)
		* \brief libclang accepts UTF-8 Only
		* \param fileNameChar
		* \return
		*/
		const char* LocateToUTF8(const char* fileNameChar);
	}
}
#endif