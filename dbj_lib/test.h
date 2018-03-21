#pragma once

#include <ctime>
#include <array>
#include "dbj_micro_log.h"
#include "dbj_clidata.h"
#include "steve_wishnousky_cli_data.h"

// DBJ: _CONCAT is apparently defined in msvc <typetraits>
#ifndef _CONCAT
#define _CONCATX(x, y) x ## y
#define _CONCAT(x, y) _CONCATX(x, y)
#endif



namespace {
	namespace detail {
#pragma warning( push )
#pragma warning( disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
		inline auto timestamp = []()
			->std::array<char, 100u>
		{
			// std::locale::global(std::locale(nullptr));
			std::time_t t = std::time(nullptr);
			//char mbstr[100];
			std::array<char, 100u> mbstr;
			if (std::strftime(mbstr.data(), mbstr.size(), "%A %c", localtime(&t))) {
				return mbstr;
			}
			throw std::runtime_error("timestamp() -- std::strftime has failed");
		};
#pragma warning( pop ) 
#undef _CRT_SECURE_NO_WARNINGS
	}

	inline bool test() {

		using namespace dbj;

		constexpr const char * line
		{ "\n____________________________________________________________________________" };
		auto timestamp = detail::timestamp() ;
		constexpr const int cl_version{ _MSC_FULL_VER };

		print("\n\tCL version:\t\t\t", _MSC_FULL_VER);
		print("\n\tBuild timestamp:\t\t", timestamp.data());
		print("\n\tUnicode ", (dbj::unicode ? " IS " : "NOT"), " defined");

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

	static auto run_before_exe_module_start = test();
}
