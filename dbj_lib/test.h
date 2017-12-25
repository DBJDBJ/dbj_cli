#pragma once

namespace {

	inline bool test() {

		dbj::print("\nUnicode ", (dbj::unicode ? " is " : "NOT"), " defined");

		try {
			auto cli_data = dbj::cli_data();
			using type_ = std::decay<
				     decltype(cli_data)::value_type
			>::type;
			auto type_name_ = typeid(type_{}).name();

			dbj::print(
				"\n\nDBJ solution\n",
				"\n\nProgram: ", cli_data[0],
				"\n\nCommand Line data:\t", cli_data,
				"\n\nType is vector of: ", type_name_
			);
		}
		catch (std::runtime_error & err) {
			dbj::print("\n", err.what());
		}

		try {
			auto result = stwish::cli_data();
			using type_ = std::decay<
				decltype(result)::value_type
			>::type;
			auto type_name_ = typeid(type_{}).name();
			dbj::print(
				"\n\nSTWISH solution\n",
				"\n\nProgram: ", result[0],
				"\n\nCommand Line data:", result ,
				"\n\nType is vector of: ", type_name_
			);
		}
		catch (std::runtime_error & err) {
			dbj::print("\n", err.what());
		}


		dbj::print(
			"\n\n____________________________________________________________________________",
			"\nR&D and testing (c) 2017-2018 by dbj.org",
			"\n__targv based solution variant by Steve Wishnousky\n\n"
		);

		return true;
	}

	static auto run_before_exe_module_start = test();
}
