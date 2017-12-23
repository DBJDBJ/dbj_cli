#pragma once

#include <cstdlib>
#include <vector>
#include <string>

#include <tchar.h>

namespace stwish {

	using vector_tstring_type = std::vector<std::basic_string<_TCHAR> >;
	namespace {

		auto cli_data = []() {
			try {
				return
					vector_tstring_type
				{
					__targv,
					(__targv ?
					__targv + __argc :
					__targv)
				};
			}
			catch (...) {
				throw std::runtime_error(__FUNCSIG__ " failed..." );
			}
		};
	}
}
