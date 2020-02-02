#pragma once

#include "dbj_micro_log.h"
#include "dbj_clidata.h"
#include "steve_wishnousky_cli_data.h"

// DBJ: _CONCAT is apparently defined in msvc <typetraits>
#ifndef _CONCAT
#define _CONCATX(x, y) x ## y
#define _CONCAT(x, y) _CONCATX(x, y)
#endif



namespace dbj {
	

	inline bool test() {

		// using namespace dbj;

		constexpr const char * line
		{ "\n____________________________________________________________________________" };
		auto timestamp =  dbj::god_of_time::timestamp() ;
		constexpr const int cl_version{ _MSC_FULL_VER };

		print("\n\tCL version:\t\t\t", _MSC_FULL_VER);
		print("\n\tBuild timestamp:\t\t", timestamp.data());
		print("\n\tUnicode ", (dbj::detail::unicode ? " IS " : "NOT"), " defined");

		auto run = [&]( auto && fun, auto && title ) {
			print(line, "\n\t", title);
			try {
				auto result = fun();
				using type_ = typename decltype(result)::value_type;
				std::string type_name_{ typeid(type_).name() };
				print(
					"\n\tProgram name:\t\t\t", (result.size() > 0 ? result[0] : L"Bad Command Line ?"),
					"\n\tCommand Line:\t\t", result,
					"\n\tType is vector of:\n", type_name_
				);
			}
			catch (std::runtime_error & err) {
				print("\n\tException:\n", err.what());
			}
			catch (...) {
				print("\n\tUnknown Xception.");
			}

		};


		run(dbj::cli_data, "DBJ Solution");
		run(stwish::cli_data, "STWISH Solution");


		print(
			line,
			"\n\tR&D and testing (c) 2017-2018 by dbj.org",
			"\n\t__targv based solution variant (c) by Steve Wishnousky\n"
		);

		return true;
	}
	/*
	Activate it here and risk static order fiasco

	static auto run_before_exe_module_start = test();

	or do this from main();
	*/
} // dbj
