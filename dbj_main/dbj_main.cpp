// dbj_main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../dbj_lib/test.h"

#include "dbj_fontlist.h"
#include "../dbj_lib/CL_19_12_25835_0.h"
#include "../dbj_lib/new_clever_nuggets.h"
#include "../dbj_lib/dbj_std_arr_handling.h"

int main()
{
	// auto cl_version = (_MSVC_LANG);

	CL_19_12_25835_0::test();
	dbj::quick_test_of_some_new_clever_nuggets();
	dbj::test_dbj_std_arr_handling();
	

	auto vnames = dbj::font_names(dbj::enumerateFonts());

	dbj::print("\nAvailable Fonts\n");
	auto counter = 1;
	for (auto n : vnames) {
		dbj::print("\n", dbj::string_pad( counter++ ),":\t")(n);
	}

	// dbj::log.flush();
    return 0;
}

