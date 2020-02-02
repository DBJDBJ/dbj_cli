#pragma once

#include <stdarg.h>
#include <memory.h>
#include <assert.h>
#include <crtdbg.h>
#include <stdio.h>
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface

	inline int dbj_mbox_w ( const wchar_t * msg_ ) {
			return MessageBoxW(
				NULL,
				(LPCWSTR)msg_,
				(LPCWSTR)TEXT(__FILE__),
				MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
			);
	};

	inline int dbj_mbox_a ( const char * msg_ ) {
			return MessageBoxA(
				NULL,
				(LPCSTR)msg_,
				(LPCSTR)(__FILE__),
				MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
			);
	};

#endif
	/*
	Usage:
	#error dbj::dbj_nicer_filename(__FILE__) " has a problem."
	TODO: is wide version necessary?
	*/
	inline const char * dbj_nicer_filename(const char * filename) {
		return (strrchr(filename, '\\') ? strrchr(filename, '\\') + 1 : filename);
	}

#ifdef BUFSIZ
	#define TRACEMAXSTRING  (BUFSIZ * 2)
#else
	#define TRACEMAXSTRING  (1024 * 2)
#endif

	/*
	ANSI is the default debugger output
	*/
	inline void dbj_trace(const char * filename, const unsigned int line, const char* const format, ...)
	{
		const size_t buffer_size = TRACEMAXSTRING;
		char buffer[TRACEMAXSTRING] = {0};
		const char * nicer_name = dbj_nicer_filename(filename);
		_ASSERTE(nicer_name);
		// first print file(line)
		_snprintf_s(&buffer[0], buffer_size, buffer_size - 1, "\n\n%s(%d): ", nicer_name, line);
		_RPT0(_CRT_WARN, buffer );
		memset(buffer, 0, TRACEMAXSTRING);
		va_list args;
		va_start(args, format);
		int nBuf;
		nBuf = _vsnprintf_s(&buffer[0], buffer_size, buffer_size - 1, format, args);
		va_end(args);
		_RPT0(_CRT_WARN, buffer);
	}

#define DBJ_TRACE( format, ...)  dbj_trace( __FILE__, __LINE__, format, __VA_ARGS__ )

#ifdef __cplusplus    // If used by C++ code, 
} // extern "C"
#endif
