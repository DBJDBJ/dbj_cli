#pragma once
/*
dbj Run Time lib
*/

#include <stdarg.h>
#include <crtdbg.h>
#include <vector>
#include <sstream>

namespace dbj {

#ifdef _UNICODE
	constexpr auto unicode = true;
#else
	constexpr auto unicode = false;
#endif

	constexpr auto bufsiz = BUFSIZ * 2;

	using vector_wstrings_type = std::vector<std::wstring>;
	using vector_strings_type = std::vector<std::string >;

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

#pragma region micro print fwork 
namespace dbj {

	namespace {

		class DBJLog final {

			typedef std::ostream  stream_type;
			/*
			usage: MyBuf buff; std::ostream stream(&buf); stream << 1 << true << L"X" << std::flush ;
			*/
			mutable class DBJBuf final : public std::stringbuf
			{
			public:
				// called on stream flush
				virtual int sync() {
					// use this->str() here
					auto string_trans = this->str();
#ifndef _WINDOWS
					// if one wants to pipe/redirect the console output
					fprintf(stdout, string_trans.data());
#endif
					_RPT0(_CRT_WARN, string_trans.data());
					// clear the buffer afterwards
					this->str("");
								return 1 /*true*/ ;
				}
			} buffer_{} ;
			// mutable  DBJBuf buffer_{};

		public:
			
			stream_type & operator () () noexcept {
				static std::ostream log_stream_( &buffer_ );
				return log_stream_;
			}

			 mutable bool flushed = false;

			 void flush () {
					 stream_type & stream_ = (*this)();
					 _ASSERTE(stream_.good());
					 stream_.flush();
					 this->flushed = !this->flushed;
			 }

			 ~DBJLog() {
				 try {
					 if (!this->flushed) {
						 this->flush();
					 }
				 }
				 catch (... ) {
					 DBJ_TRACE("\n%s %s\n", __FUNCSIG__, " Unknown exception");
				 }
			 }

		}  log{} ; // DBJLog

		auto out_ = [] (auto & val_) 
		{
			auto range_out = [] (auto bgn_, auto end_ ) {
				auto & walker = bgn_;
				out_("{");
				while (walker != end_) { out_(" "); out_(*walker); out_(" "); walker++; }
				out_("}");
			};
			
			using val_type = decltype(val_);

			if constexpr( std::is_same_v<val_type, std::wstring>) {
				log() << ( std::string(val_.begin(), val_.end()) );
				return;
			} 
			if constexpr( std::is_same_v<val_type, vector_wstrings_type > ) {
				range_out(val_.begin(), val_.end());
				return;
			}
			if constexpr(std::is_same_v<val_type, vector_strings_type > ) {
				range_out(val_.begin(), val_.end());
				return;
			}
					log() << val_;
					return;
		};

		auto print = [](auto... param)
		{
			if constexpr (sizeof...(param) > 0) {
				char dummy[sizeof...(param)] = {
					(out_(/* std::forward<decltype(param)> */ (param)), 0)...
				};
			}
			return print;
		};
	} // nspace
} // dbj

#pragma endregion

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
