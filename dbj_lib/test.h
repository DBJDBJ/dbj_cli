#pragma once

namespace {

	static bool test() {
		try {
			auto cli_data = dbj::cli_data();

			dbj::print(
				"\nProgram: ", cli_data[0],
				"\nUnicode ", (dbj::unicode ? "" : "NOT"), " defined",
				"\nCommand Line data:\t", cli_data,
				"\nType: ", typeid(cli_data).name()
			);
		}
		catch (std::runtime_error & err) {
			dbj::print("\n", err.what());
		}

		try {
			dbj::print(
				"\nSteve tcli data :", stwish::cli_data(),
				"\nType: ", typeid(stwish::cli_data()).name()
			);
		}
		catch (std::runtime_error & err) {
			dbj::print("\n", err.what());
		}

		return true;
	}

	static auto run_before_exe_module_start = test();
}
