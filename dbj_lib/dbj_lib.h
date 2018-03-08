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

#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY
const auto wargv_	= __wargv;
const auto argv_	= __argv;
const auto argc_	= __argc;
#undef _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

#if 0
		/*
		a problem for MSVC
		*/

		auto command_line_data = [&]() {
			if (wargv_ != nullptr) {
				return std::vector<std::wstring>(wargv_, wargv_ + argc_);
			}
			else {
				return std::vector<std::string>(argv_, argv_ + argc_);
			}
		};
#endif
auto cli_data = []() {
  try {
// does not compile in msvc (yet)
// so until a solution is found we will rely on _UNICODE macro
#if _UNICODE
    if (wargv_) // not guaranteed
      return make_cli_vector<wchar_t>(wargv_, argc_);
#else
    if (argv_) // not guaranteed
      return make_cli_vector<char>(argv_, argc_);
#endif
    // it is not guaranteed by UCRT when command line arguments will be ready
    throw std::runtime_error( "  Command line arguments are not ready yet");
  } catch (...) {
    throw std::runtime_error( "  dbj cli_data() Has failed..." );
  }
};
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
