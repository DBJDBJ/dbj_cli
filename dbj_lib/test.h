#pragma once

#include <ctime>
#include <array>
#include <chrono>
#include "dbj_micro_log.h"
#include "dbj_clidata.h"
#include "steve_wishnousky_cli_data.h"

// DBJ: _CONCAT is apparently defined in msvc <typetraits>
#ifndef _CONCAT
#define _CONCATX(x, y) x ## y
#define _CONCAT(x, y) _CONCATX(x, y)
#endif



namespace {
	namespace chronos {

		using namespace std;

		inline constexpr auto BUFSIZE = 1024u;
		typedef array<char, BUFSIZE>  char_buffer ;
		typedef std::chrono::time_point<std::chrono::system_clock>  system_time_point;


		std::time_t systemtime_now()
		{
			system_time_point system_now = std::chrono::system_clock::now();
			return std::chrono::system_clock::to_time_t(system_now);
		}

		/// <summary>
		/// Apparently this is thread safe
		/// </summary>
		/// <param name="time">our own std::time_t buffer</param>
		/// <returns>std::tm snapshot</returns>
		inline tm localtime( std::time_t time = 0 )
		{
			std::tm tm_snapshot;
			if (time == 0) time = systemtime_now();
			localtime_s(&tm_snapshot, (&time));
			return tm_snapshot;
		}

		inline char_buffer timestamp ()
		{
			// locale::global(locale(nullptr));
			// time_t t = time(nullptr);
			// std::time_t tt_{};
			const auto lt_ = & localtime();
			char_buffer mbstr;
			if (strftime(mbstr.data(), mbstr.size(), "%c", lt_ )) {
				return mbstr;
			}
			throw runtime_error("timestamp() -- strftime has failed");
		}
	}

	inline bool test() {

		using namespace dbj;

		constexpr const char * line
		{ "\n____________________________________________________________________________" };
		auto timestamp = chronos::timestamp() ;
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
