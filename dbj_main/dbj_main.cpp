// dbj_main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../dbj_lib/test.h"

#include "dbj_fontlist.h"

namespace {
	/*
	 bellow works for all the std:: string types and string view types
	 battery of  if constexpr( std::is_same<>::value ) 
	 would be necessary inside to enforce this usage rule, but ...
	 msvc is not there yet with implementing if-constexpr() as required by C++17
	*/
	auto starts_with = []( const auto & value,	const auto & match)
	{
			return 0 == value.compare(0, match.size(), match);
	};


}

namespace CL_19_12_25835_0 {

	auto does_not_compile = [](auto _string )
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
			return std::exception("Exception in [" __FUNCSIG__ "] argument has to be std::string or std::wstring");
		}
	};

	template <typename T> 
	auto make_vector( T * = 0 ) {
		return std::vector<T>{};
	}
	/*

	template<typename T1, typename T2>
	auto Plus(T1&& t1, T2&& t2) ->
		decltype(forward<T1>(t1) + forward<T2>(t2))
	{
		return forward<T1>(t1) + forward<T2>(t2);
	}
	https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev15.query%3FappId%3DDev15IDEF1%26l%3DEN-US%26k%3Dk(decltype_CPP)%3Bk(decltype)%3Bk(DevLang-C%2B%2B)%3Bk(TargetOS-Windows)%26rd%3Dtrue
	
	instead one could use lambda/auto combination for much simpler solution
	*/
	auto && Plus = [] (auto && t1, auto && t2) 
		// ->	decltype(forward<T1>(t1) + forward<T2>(t2))
	{
		// return forward<T1>(t1) + forward<T2>(t2);
		return (t1) + (t2);
	};

	inline void what_r_pointers() {

		int ten{ 10 };
		bool outcome{ true };
		char letter{ 'A' };
		int * pointer_to_int;
		bool * pointer_to_bool;
		char * pointer_to_char;

		pointer_to_int  = & ten;
		pointer_to_bool = & outcome;
		pointer_to_char = & letter;
	}

	inline void friend_to_many (const char * newborn_) {
		printf("%s has just born a new isntance", newborn_);
	}

	inline void what_are_friends() {
		struct A {
			friend decltype(friend_to_many);
			A() {
				friend_to_many("A");
			}
		};
		struct B {
			friend decltype(friend_to_many);
			B() {
				friend_to_many("B");
			}
		};
	}
	inline void test() {
		using namespace std;
		static auto the_string = string{};
			// auto rv1 = does_not_compile(the_string);
			auto rv2 = does_compile(&the_string);
			auto rv3 = if_constexpr(true);
			auto rv4 = make_vector(&the_string);
			auto && rv5 = Plus(1,2);

			what_r_pointers();
	}
}

int main()
{
	// auto cl_version = (_MSVC_LANG);

	CL_19_12_25835_0::test();

	auto doesit = starts_with( std::string("abra ka dabra"), std::string("abra"));

	dbj::print("doesit = ", doesit,"\n\nAbout to exit main()");

	auto vnames = dbj::font_names(dbj::enumerateFonts());

	dbj::print(vnames);

	// dbj::log.flush();
    return 0;
}

