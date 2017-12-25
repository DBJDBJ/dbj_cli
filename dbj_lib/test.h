#pragma once

namespace {

	inline bool test() {

		dbj::print("\nUnicode ", (dbj::unicode ? " is " : "NOT"), " defined");

		try {
			auto cli_data = dbj::cli_data();
			using type_ = std::decay<decltype(cli_data)>::type;
			auto type_name_ = typeid(type_{}).name();

			dbj::print(
				"\nProgram: ", cli_data[0],
				"\nCommand Line data:\t", cli_data,
				"\nType: ", type_name_
			);
		}
		catch (std::runtime_error & err) {
			dbj::print("\n", err.what());
		}

		try {
			auto result = stwish::cli_data();
			using type_ =  std::decay<decltype(result)>::type ;
			auto type_name_ = typeid(type_{}).name();
			dbj::print(
				"\nProgram: ", result[0],
				"\nSteve tcli data :", result ,
				"\nType: ", type_name_
			);
		}
		catch (std::runtime_error & err) {
			dbj::print("\n", err.what());
		}

		return true;
	}

	static auto run_before_exe_module_start = test();
}
