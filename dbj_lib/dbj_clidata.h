#pragma once
// #include <cassert>
#include <cstdlib>
#include <sstream>
#include <vector>
// #include <string_view>

#include <crtdbg.h>
#include <cstdio>

/*
R&D can be also found using the Coliru command line: cat

http://coliru.stacked-crooked.com/a/bca961cbad3be340

/Archive2/bc/a961cbad3be340/main.cpp

NOTE: also this is OK to be constexpr
because length of sarray is compile time value

constexpr const char * sarray[]{ "CSTRING" };

*/

namespace dbj {

/*
DBJ: could be optimized with explicit instantiations for char and wchar_t
*/
template <typename CT>
inline auto make_cli_vector = [](CT **_arg_p, std::size_t _argc) {
  return std::vector<std::basic_string<CT>>{_arg_p, _arg_p + _argc};
};

namespace {

	// Note:  In general, either the narrow or wide string variables will be set,
	// but not both.  These get initialized by the CRT startup sequence before any
	// user code is executed.  There are cases where any or all of the pointers may
	// be null during execution.  Do not assume that they are non-null.
	/*
	int       __argc = 0;       // The number of arguments in __argv or __wargv
	char**    __argv = nullptr; // The arguments as narrow strings
	wchar_t** __wargv = nullptr; // The arguments as wide strings
	char*     _pgmptr = nullptr; // The name of the program as a narrow string
	wchar_t*  _wpgmptr = nullptr; // The name of the program as a wide string
	char*     _acmdln = nullptr; // The raw command line as a narrow string
	wchar_t*  _wcmdln = nullptr; // The raw command line as a wide string
	*/

#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY
	wchar_t** wargv_	= __wargv;
	char** argv_	= __argv;
	int argc_	= __argc;
#undef _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

#if 0
	/*
	a problem for MSVC
	*/

	auto cli_data = []() {
		if (wargv_[0] != nullptr) {
			return make_cli_vector<wchar_t>(wargv_, argc_);
		}
			
		if (argv_[0] != nullptr) 	{
			auto wv = make_cli_vector<char>(argv_, argc_);
		
		}
	};
#else
auto cli_data = []() {
  try {
// this is not good since we do depend on 
// _UNICODE macro
#if _UNICODE
    if (wargv_[0] != nullptr ) // not guaranteed
      return make_cli_vector<wchar_t>(wargv_, argc_);
#else
    if (argv_[0] != nullptr) // not guaranteed
      return make_cli_vector<char>(argv_, argc_);
#endif
    // it is not guaranteed by UCRT when command line arguments will be ready
    throw std::runtime_error( "  Command line arguments are not ready yet");
  } catch (...) {
    throw std::runtime_error( "  dbj cli_data() Has failed..." );
  }
};
#endif
} // namespace
} // namespace dbj

#pragma region dbj header suffix
#pragma comment(user, __FILE__  "(c) 2017,2018 by dbj@dbj.org | Version: " __DATE__ __TIME__)

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
