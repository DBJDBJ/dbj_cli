#pragma once
// #include <cassert>
#include <cstdlib>
#include <sstream>
#include <vector>

#include <crtdbg.h>
#include <cstdio>

/*
R&D can be also found using the Coliru command line: cat 

http://coliru.stacked-crooked.com/a/bca961cbad3be340

/Archive2/bc/a961cbad3be340/main.cpp
*/



namespace dbj {

	/*
	OK since VS 2017 15.5 update
	*/
	template<typename CT>
	auto make_cli_vector = [](CT ** _arg_p, int _argc)
	{
		return std::vector< std::basic_string<CT> >{ _arg_p, _arg_p + _argc};
	};

	namespace {
#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

		wchar_t **		wargv_ = std::as_const(__wargv);
		char    **		argv_ = std::as_const(__argv);
		unsigned int	argc_ = std::as_const(__argc);

#undef _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

		auto cli_data = [] ( ) {

			try {
// following does not compile (of course) 
// so until a solution is found we will rely on _UNICODE macro
#if _UNICODE
				if (wargv_) // not guaranteed 
				return make_cli_vector<wchar_t>( wargv_ , argc_ );
#else
				if (argv_) // not guaranteed
				return make_cli_vector<char>( argv_, argc_ );
#endif
				// it is not guaranteed by UCRT when command line arguments will be ready
				throw  std::runtime_error(  __FUNCSIG__ " Error: command line arguments are not ready yet");
			}
			catch (...) {
				throw  std::runtime_error( __FUNCSIG__ " Error: has miserably failed...");
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
