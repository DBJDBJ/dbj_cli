#pragma once

#include <stdarg.h>
#include <crtdbg.h>
#include <vector>

namespace dbj::dbg {


	/*
	Usage:
	#error dbj::nicer_filename(__FILE__) " has a problem."
	TODO: is wide version necessary?
	*/
	inline auto nicer_filename(const char * filename) {
		return (strrchr(filename, '\\') ? strrchr(filename, '\\') + 1 : filename);
	}

#ifdef BUFSIZ
	constexpr static int TRACEMAXSTRING = BUFSIZ * 2;
#else
	constexpr static int TRACEMAXSTRING = 1024 * 2;
#endif

	/*
	ANSI is the default debugger output
	*/
	inline void trace(const char * filename, const unsigned int line, const char* const format, ...)
	{
		std::vector<char> buffer(TRACEMAXSTRING, '\x0');
		const auto nicer_name = nicer_filename(filename);
		_ASSERTE(nicer_name);
		_snprintf_s(&buffer[0], buffer.size(), TRACEMAXSTRING - 1, "%s(%d): ", nicer_name, line);
		_RPT0(_CRT_WARN, buffer.data());
		buffer.resize(TRACEMAXSTRING);
		va_list args;
		va_start(args, format);
		int nBuf;
		nBuf = _vsnprintf_s(&buffer[0], buffer.size(), TRACEMAXSTRING - 1, format, args);
		va_end(args);
		_RPT0(_CRT_WARN, buffer.data());
	}

#define DBJ_TRACE( format, ...)  dbj::dbg::trace( __FILE__, __LINE__, format, __VA_ARGS__ )
	// keep it in release builds too, or
	// #define DBJ_TRACE((void)0)
} // dbg
