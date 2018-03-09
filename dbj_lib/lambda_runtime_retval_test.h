#pragma once

#include <map>
#include <string>

namespace CL_19_13_26128_0 {
#ifdef __clang__
	auto does_not_compile = [](auto _string)
		-> std::vector< decltype(_string) >
	{
		return std::vector<decltype(_string)>{_string};
	};
#endif
	auto does_compile = [](auto _string)
		//-> std::vector< decltype(_string) >
	{
		using string_type = decltype(_string);
		return std::vector<string_type>{_string};
	};

	auto if_constexpr = [](auto _string)
	{
		using string_type = decltype(_string);

		if constexpr (std::is_same<string_type, std::string>::value) {
			return std::vector<std::string>{_string};
		}
		else if constexpr (std::is_same<string_type, std::wstring>::value)
		{
			return std::vector<std::wstring>{_string};
		}
		else {
			return std::exception(
				"Exception in [ if_constexpr() ] argument has to be std::string or std::wstring"
			);
		}
	};
}


namespace CL_19_12_25835_0 {

	auto does_not_compile = [](auto _string)
		-> std::vector< decltype(_string) >
	{
		return std::vector<decltype(_string)>{};
	};
	auto does_compile = [](auto _string)
		//-> std::vector< decltype(_string) >
	{
		return std::vector<decltype(_string)>{};
	};

	auto if_constexpr = [](auto _string)
	{
		using string_type = decltype(_string);

		if constexpr (std::is_same<string_type, std::string>::value) {
			return std::vector<std::string>{};
		}
		else if constexpr (std::is_same<string_type, std::wstring>::value)
		{
			return std::vector<std::wstring>{};
		}
		else {
			return std::exception(
		"Exception in [ if_constexpr() ] argument has to be std::string or std::wstring"
			);
		}
	};
}

namespace lambda_runtime_retval_test {
	using namespace std;
	void test_ () {
		
		 string &&		narrow_{ "NARROW" };
		 wstring &&		wide_{ L"WIDE" };

		if ( __argv )
			auto v1 = CL_19_13_26128_0::does_compile( *__argv  );

		if (__wargv)
			auto v2 = CL_19_13_26128_0::does_compile( *__wargv );

#ifdef __clang__
		auto v3 = CL_19_13_26128_0::does_not_compile(narrow_);
		auto v4 = CL_19_13_26128_0::does_not_compile(wide_);
#endif
		auto v5 = CL_19_13_26128_0::if_constexpr( narrow_ );
		auto v6 = CL_19_13_26128_0::if_constexpr( wide_   );
	}
}