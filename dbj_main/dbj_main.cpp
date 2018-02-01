// dbj_main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../dbj_lib/test.h"

#include "dbj_fontlist.h"

namespace {
	/*
	 bellow works for all the std:: string types and string view types
	 battery of  if constexpr( std::is_same<>::value ) 
	 would be necessary inside to enforce this usage rule, but ...
	 msvc is not there yet with implementing if-constexpr() as required by C++17
	*/
	auto starts_with = []( const auto & value,	const auto & match)
	{
			return 0 == value.compare(0, match.size(), match);
	};

	auto msvc_does_not_compile_01FEB2018 = [](auto _string)
	{
		using string_type = decltype(_string);

		if constexpr (std::is_same<string_type, std::string>::value)
			return std::vector<std::string>{};
		else {
			return std::vector<std::wstring>{};
		}
	};
}

namespace basic_problem {
	auto msvc_does_not_compile = [](auto _string)
		//-> std::vector< decltype(_string) >
	{
		return std::vector<decltype(_string)>{};
	};
}

int main()
{
	// dbj::print(_MSVC_LANG);

	auto string_vec = msvc_does_not_compile_01FEB2018(std::string{});

	auto second_vec = basic_problem::msvc_does_not_compile(std::string{});

	auto doesit = starts_with( std::string("abra ka dabra"), std::string("abra"));

	dbj::print("doesit = ", doesit,"\n\nAbout to exit main()");

	auto vnames = dbj::font_names(dbj::enumerateFonts());

	dbj::print(vnames);

	// dbj::log.flush();
    return 0;
}

