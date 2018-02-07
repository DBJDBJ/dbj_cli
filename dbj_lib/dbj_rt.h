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

	namespace {
		/*
		Usage:
		#error dbj::nicer_filename(__FILE__) " has a problem."
		TODO: is wide version necessary?
		*/
		static __forceinline
			constexpr auto nicer_filename(const char * filename) {
			return (strrchr(filename, '\\') ? strrchr(filename, '\\') + 1 : filename);
		}
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

#pragma region micro logging fwork 

namespace {
	
	template<typename T> 
	std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
		os << '[';
		for (auto& el : vec) { os << el << ' '; }
		os << ']';
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const std::wstring & s_ ) {
		os <<  std::string( std::begin(s_), std::end( s_) ) ; 
		return os;
	}

};

namespace dbj {

	namespace {

		class DBJLog final {

			static constexpr bool PIPE_OUT{ true };

			static void write( const std::string & buf_ ) {
				auto rez = ::puts(buf_.data()) ;
				_ASSERTE(EOF != rez);
			}

			/*
			usage: MyBuf buff; 
			std::ostream stream(&buf); 
			stream << 1 << true << L"X" << std::flush ;
			*/
			mutable class DBJBuf final : public std::stringbuf
			{
			public:
				// called on stream flush
				virtual int sync() {
					// use this->str() here
					auto string_trans = this->str();

					if (DBJLog::PIPE_OUT) {
						// if one wants to pipe/redirect the console output
						auto rez = ::puts(string_trans.data());
						_ASSERTE(EOF != rez);
					}

					_RPT0(_CRT_WARN, string_trans.data());
					// clear the buffer afterwards
					this->str("");
					return 1 /*true*/ ;
				}
			} buffer_{} ;
			// mutable  DBJBuf buffer_{};

		public:
			
			typedef std::ostream  stream_type;

			stream_type & stream () noexcept {
				static std::ostream log_stream_( &buffer_ );
				return log_stream_;
			}

			 void flush () {
					 stream_type & stream_ = this->stream();
					 _ASSERTE(stream_.good());
					 stream_.flush();
			 }

			 ~DBJLog() {
				 static bool flushed = false;
				 try {
					if (!flushed) {
						 this->flush();
						 flushed = true;
					}
				 }
				 catch (... ) {
					 DBJ_TRACE("\n%s %s\n", __FUNCSIG__, " Unknown exception");
				 }
			 }

		}  log{} ; // DBJLog
#if 0
		// msvc if-constepxr() is not "there yet"
		// dbj 25DEC17
		auto out_ = [] (auto & val_) 
		{
			auto range_out = [] (auto bgn_, auto end_ ) {
				auto & walker = bgn_;
				log.stream("{");
				while (walker != end_) { log.stream(" "); log.stream(*walker); log.stream(" "); walker++; }
				log.stream("}");
			};
			
			using val_type = decltype(val_);

			if constexpr( std::is_same_v<val_type, std::wstring>) {
				log.stream() << ( std::string(val_.begin(), val_.end()) );
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
					log.stream() << val_;
					return;
		};
#endif
		auto print = [](auto p1, auto ... param)
		{
			constexpr auto no_of_args = sizeof...(param);
			auto & os = log.stream();

			os << p1;

			if constexpr (no_of_args > 0) {
				char dummy[no_of_args] = {
					(( 
						os << param 
					  ), 0)...
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
