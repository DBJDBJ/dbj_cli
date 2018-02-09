// dbj_main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../dbj_lib/test.h"

#include "dbj_fontlist.h"
#include "../dbj_lib/CL_19_12_25835_0.h"

int main()
{
	// auto cl_version = (_MSVC_LANG);

	CL_19_12_25835_0::test();
	{
		auto doesit1 = dbj::starts_with("abra ka dabra", "abra");
		auto doesit2 = dbj::starts_with(L"abra ka dabra", L"abra");
	}
	{
		auto doesit1 = dbj::does_start("abra ka dabra", "abra");
		auto doesit2 = dbj::does_start(L"abra ka dabra", L"abra");
	}

	auto vnames = dbj::font_names(dbj::enumerateFonts());

	dbj::print("\nAvailable Fonts\n");
	auto counter = 1;
	for (auto n : vnames) {
		dbj::print("\n", dbj::string_pad( counter++ ),":\t")(n);
	}

	// dbj::log.flush();
    return 0;
}

