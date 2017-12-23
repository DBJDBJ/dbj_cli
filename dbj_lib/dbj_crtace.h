#pragma once

#include <stdarg.h>
#include <crtdbg.h>
#include <vector>

namespace dbj {
	/*
	Usage:
	#error dbj::nicer_filename(__FILE__) " has a problem."
	TODO: is wide version necessary?
	*/
	static __forceinline
		constexpr auto nicer_filename(const char * filename) {
		return (strrchr(filename, '\\') ? strrchr(filename, '\\') + 1 : filename);
	}

	namespace dbg {

#ifdef BUFSIZ
		constexpr static int TRACEMAXSTRING = BUFSIZ * 2;
#else
		constexpr static int TRACEMAXSTRING = 1024 * 2;
#endif

		/* 
		    https://stackoverflow.com/questions/451749/is-there-a-trace-statement-for-basic-win32-c
			no windows.h required 

			ANSI is the default debugger output 
		*/
		inline void trace(const char * filename, const unsigned int line, const char* const format, ...)
		{
			std::vector<char> buffer(TRACEMAXSTRING, '\x0');
			const auto nicer_name = dbj::nicer_filename(filename);
						_ASSERTE(nicer_name);
			_snprintf_s(&buffer[0], buffer.size(), TRACEMAXSTRING - 1, "%s(%d): ", nicer_name, line);
						_RPT0(_CRT_WARN, buffer.data());
			buffer.resize(TRACEMAXSTRING);
			va_list args;
			va_start(args, format);
			int nBuf;
			nBuf = _vsnprintf_s( &buffer[0],	buffer.size(),	TRACEMAXSTRING - 1,	format, args);
			va_end(args);
						_RPT0(_CRT_WARN, buffer.data());
		}
	}
}

#define DBJ_TRACE( format, ...)  dbj::dbg::trace( __FILE__, __LINE__, format, __VA_ARGS__ )

// keep it in release builds too
// #define DBJ_TRACE((void)0)

#pragma region standard header suffix 
#pragma comment( user, __FILE__ "(c) 2017 by dbj@dbj.org | Version: " __DATE__ __TIME__ ) 

/*
Copyright 2017 by dbj@dbj.org

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

inspirators are mentioned in the comments bellow
*/
#pragma endregion

#if 0

https://stackoverflow.com/questions/13703823/a-custom-ostream
A custom destination for ostream means implementing your own ostreambuf.
If you want your streambuf to actually buffer(i.e.don't connect to the database after each character), 
the easiest way to do that is by creating a class inheriting from std::stringbuf. 
The **only** function that you'll need to override is the sync() method, 
which is being called whenever the stream is flushed.

	class MyBuf : public std::stringbuf
{
public:
	virtual int sync() {
		// add this->str() to database here
		// (optionally clear buffer afterwards)
	}
};

You can then create a std::ostream using your buffer :

MyBuf buff;
std::ostream stream(&buf)

buff.flush() // provokes the call to the MyBuf::sync()

#endif
