#pragma once

namespace {

	inline bool test() {

		using namespace dbj;

		static constexpr const char *  line 
			= "\n____________________________________________________________________________";
		static constexpr const char *  timestamp
			= __TIMESTAMP__;
		static constexpr auto cl_version = (_MSC_FULL_VER);


		print("\n\tCL version:\t\t\t", cl_version);
		print("\n\tBuild timestamp:\t\t", timestamp);
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
