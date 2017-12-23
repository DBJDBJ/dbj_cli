#pragma once
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <vector>

#include <crtdbg.h>
#include <cstdio>

#pragma region micro util
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

		template< typename T>
		void out_ ( const T & val_) {
			auto debug_stream = std::ostringstream {};
			debug_stream << val_;
			auto string_trans = debug_stream.str();
#ifndef _WINDOWS
			// if one wants to pipe/redirect the console output
			fprintf(stderr, string_trans.data());
#endif
			_RPT0(_CRT_WARN, string_trans.data());
		};

		// unicode
		void out_(const std::wstring & val_) {
			auto debug_stream = std::wostringstream{};
			debug_stream << val_;
			auto string_trans = debug_stream.str();
#ifndef _WINDOWS
			// if one wants to pipe/redirect the console output
			fwprintf(stderr, string_trans.data());
#endif
			_RPTW0(_CRT_WARN, string_trans.data());
		};


		void out_ ( vector_strings_type val_) {
			out_("{");
			for (auto v : val_) { out_(" "); out_(v); out_(" "); }
			out_("}");
		};

		void out_( vector_wstrings_type val_) {
			out_("{");
			for (auto v : val_) { out_(" "); out_(v); out_(" "); }
			out_("}");
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

namespace {
}
#pragma endregion

namespace dbj {

	namespace {
#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

		wchar_t **		wargv_ = std::as_const(__wargv);
		char    **		argv_ = std::as_const(__argv);
		unsigned int	argc_ = std::as_const(__argc);

#undef _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

		// wargv_ !=  nullptr
		auto decide(std::true_type tt) {
			return vector_wstrings_type{ wargv_, wargv_ + argc_ };
		}
		// wargv_ ==  nullptr
		auto decide(bool ft) {
			return vector_strings_type{ argv_, argv_ + argc_ };
		}

		auto cli_data = []() {

			try {
				auto cli_vector = decide(
					wargv_
					? std::true_type{}
					: false
				);
				return cli_vector;
			}
			catch (...) {
				throw  std::runtime_error("dbj::cli_data() has miserably failed...");
			}

		};
	} // anonspace
}

#pragma region research
#if 0

template <typename ... Args>
inline void trace(wchar_t const * const message, Args ... args) noexcept
{
	wchar_t buffer[bufsiz] = {};
	assert(-1 != _snwprintf_s(buffer, bufsiz, bufsiz, message, (args) ...));
	_RPTFW0(_CRT_WARN, buffer);
}

template <typename ... Args>
inline void trace(const char * const message, Args ... args) noexcept
{
	char buffer[bufsiz] = {};
	assert(-1 != _snprintf_s(buffer, bufsiz, bufsiz, message, (args) ...));
	_RPTF0(_CRT_WARN, buffer);
}

/*
https://stackoverflow.com/questions/47452748/how-to-decide-on-auto-return-type-at-run-time/47457592#47457592
*/
auto command_line_data = [&]() {
	if constexpr (wargv_pointer) {
		return std::vector<std::wstring>(wargv_, wargv_ + argc_);
	}
	else {
		return std::vector<std::string>(argv_, argv_ + argc_);
	}
};

// http://en.cppreference.com/w/cpp/language/auto
auto does_not_compile_msvc = [](bool wide)
{
	if (wide)
		return std::vector<std::string>();

	return std::vector<std::wstring>();
};

auto msvc_does_not_compile_too = [](auto _string)
{
	using string_type = decltype(_string);

	if constexpr (std::is_same<string_type, std::string>::value)
		return std::vector<std::string>;
	else {
		return std::vector<std::wstring>;
	}
};
/*
string and wstring have no common type 
so '::type' will be undefined here

using string_type = std::common_type<std::wstring , std::string>::type;
*/
/* unused utility */
template<typename T, typename = std::enable_if_t< std::is_pointer<T>::value> >
constexpr inline bool
is_null(const T tp = 0)
{
	return (tp != nullptr);
}
#endif
#pragma endregion

#pragma region dbj header suffix 
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
*/
#pragma endregion
