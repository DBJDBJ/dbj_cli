#pragma once

namespace {

	inline bool test() {

		static constexpr const char *  line 
			= "\n____________________________________________________________________________";
		static constexpr const char *  timestamp
			= __TIMESTAMP__;
		static constexpr auto cl_version = (_MSC_FULL_VER);


		dbj::print("\n\tCL version:\t\t\t", cl_version);
		dbj::print("\n\tBuild timestamp:\t\t", timestamp);
		dbj::print("\n\tUnicode ", (dbj::unicode ? " IS " : "NOT"), " defined");

		auto run = [&]( auto && fun, auto && title ) {
			try {
				auto result = fun();
				using type_ = typename decltype(result)::value_type;
				std::string type_name_{ typeid(type_).name() };
				dbj::print(
					line,
					"\n\t", title,
					"\n\tProgram:\t\t\t", (result.size() > 0 ? result[0] : L"Bad Command Line ?"),
					"\n\tCommand Line data:\t\t", result,
					"\n\tType is vector of:\n", type_name_
				);
			}
			catch (std::runtime_error & err) {
				dbj::print("\n\tException:\n", err.what());
			}
			catch (...) {
				dbj::print("\n\tUnknown Xception.");
			}

		};


		run(dbj::cli_data, "DBJ Solution");
		run(stwish::cli_data, "STWISH Solution");


		dbj::print(
			line,
			"\nR&D and testing (c) 2017-2018 by dbj.org",
			"\n__targv based solution variant (c) by Steve Wishnousky\n"
		);

		return true;
	}

	static auto run_before_exe_module_start = test();
}
