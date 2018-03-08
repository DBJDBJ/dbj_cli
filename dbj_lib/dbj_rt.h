#pragma once
/*
dbj Run Time lib
*/

#include <vector>
#include <sstream>
#include <iostream>
#include "dbj_dbg.h"

namespace dbj {

	namespace {
#ifdef _UNICODE
	constexpr auto unicode = true;
	using outstream_type = std::wostream ;
	using stringbuf_type = std::wstringbuf;
	auto && COUT = std::wcout;
	auto && LEFT_ANGLED = L'[';
	auto && RGHT_ANGLED = L']';
	auto && SPACE = L' ';
	auto && COMMA = L',';
#else
	constexpr auto unicode = false;
	using outstream_type = std::ostream;
	using stringbuf_type = std::stringbuf;
	auto && COUT = std::cout;
	auto && LEFT_ANGLED = '[';
	auto && RGHT_ANGLED = ']';
	auto && SPACE = ' ';
	auto && COMMA = ',';
#endif

	constexpr auto bufsiz = BUFSIZ * 2;

	using vector_wstrings_type = std::vector<std::wstring>;
	using vector_strings_type = std::vector<std::string >;

	} // ns


}




#pragma region micro logging fwork 

namespace {

	template<typename IT, typename UF>
	inline void range_out(dbj::outstream_type & os, IT && first_, IT && last_, UF && unary_fun_ ) {
		for (; first_ != last_; ++first_) {
			unary_fun_( os , (*first)) ;
		}
	}
	
	 template<typename T> 
	 inline 
		 dbj::outstream_type & operator<<(dbj::outstream_type & os, const std::vector<T>& vec) {
		os <<  dbj::LEFT_ANGLED << dbj::SPACE;
		for (auto& el : vec) { os << el << dbj::SPACE; }
		os << dbj::RGHT_ANGLED;
		return os;
	}

	 inline
		std::ostream & operator<<(std::ostream & os, const std::wstring & s_ ) {
		os <<  std::string( std::begin(s_), std::end( s_) ) ; 
		return os;
	}

	 inline
		 std::wostream & operator<<(std::wostream & os, const std::string & s_) {
		 os << std::wstring(std::begin(s_), std::end(s_));
		 return os;
	 }
};

namespace dbj {

	namespace {

		class DBJLog final {

			static constexpr bool PIPE_OUT{ true };

			template<typename T>
			static void write( std::basic_string<T> && buf_ ) {
#ifdef UNICODE
				auto rez = ::_putws(buf_.data());
#else
				auto rez = ::puts(buf_.data());
#endif
				_ASSERTE(EOF != rez);
			}

			/*
			usage: MyBuf buff; 
			std::ostream stream(&buf); 
			stream << 1 << true << L"X" << std::flush ;
			*/
			mutable struct DBJBuf final : public dbj::stringbuf_type
			{
				// called on stream flush
				virtual int sync() {
					// use this->str() here
					auto string_trans = this->str();

					if (DBJLog::PIPE_OUT) {
						// if one wants to pipe/redirect the console output
#ifdef UNICODE
						auto rez = ::_putws(string_trans.data());
#else
						auto rez = ::puts(string_trans.data());
#endif
						_ASSERTE(EOF != rez);
					}

					_RPT0(_CRT_WARN, string_trans.data());
					// clear the buffer afterwards
#ifdef UNICODE
					this->str(L"");
#else
					this->str("");
#endif
					return 1 /*true*/ ;
				}
			} buffer_{} ;
			// mutable  DBJBuf buffer_{};

		public:
			
			typedef dbj::outstream_type   stream_type;

			stream_type & stream () noexcept {
				static stream_type log_stream_( &buffer_ );
				return log_stream_;
			}

			 void flush () {
					 stream_type & stream_ = this->stream();
					 _ASSERTE(stream_.good());
					 stream_.flush();
			 }

			 DBJLog() { this->flush(); }

			 ~DBJLog() {
				 static bool flushed = false;
				 try {
					if (!flushed) {
						 this->flush();
						 flushed = true;
					}
				 }
				 catch (... ) {
#if _DEBUG
					 DBJ_TRACE ( __func__, " Unknown exception");
#endif
				 }
			 }

		}  
			log{} ; // DBJLog made here

		auto print = [](auto ... param)
		{
			constexpr auto no_of_args = sizeof...(param);
			auto & os = log.stream();

			/* 
			'constexpr' should not compile here since no_of_args is runtime value
			perhaps?
			*/
			if (no_of_args > 0) {
				char dummy[no_of_args] = {
					(( 
						os << param 
					  ), 0)...
				};
				// remove the annoying warning : unused variable 'dummy' [-Wunused-variable]
				(void)dummy;
			}
#ifndef __clang__
			return print;
#endif
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
