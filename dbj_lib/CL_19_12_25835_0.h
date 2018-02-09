#pragma once

#include <map>
#include <string>

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
			return std::exception("Exception in [" __FUNCSIG__ "] argument has to be std::string or std::wstring");
		}
	};

	template <typename T>
	auto make_vector(T * = 0) {
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
	auto && Plus = [](auto && t1, auto && t2)
		// ->	decltype(forward<T1>(t1) + forward<T2>(t2))
	{
		// return forward<T1>(t1) + forward<T2>(t2);
		return (t1)+(t2);
	};

	inline void what_r_pointers() {

		int ten{ 10 };
		bool outcome{ true };
		char letter{ 'A' };
		int * pointer_to_int;
		bool * pointer_to_bool;
		char * pointer_to_char;

		pointer_to_int = &ten;
		pointer_to_bool = &outcome;
		pointer_to_char = &letter;
	}

	namespace {

		using class_instance_register_map = std::map< std::string, int >;

		auto register_new_arrival = [](const std::string & class_name) {
			static class_instance_register_map cirm_{};

			auto found = cirm_.find(class_name);
			if (found != cirm_.end()) {
				// here we have search->first und search->second to play with
				cirm_[class_name] = (1 + found->second);
			}
			else { // not registered yet
				cirm_[class_name] = 0;
			}
			return cirm_[class_name];
		};
		// return the id of the new instance for the class	 
		inline int friend_to_many(const std::type_info & type_of_newborn_) {
			auto id_ = register_new_arrival(type_of_newborn_.name());
			printf("%s has just born a new instance, with the id: %d", type_of_newborn_.name(), id_);
			return id_;
		}
	}

	inline void what_are_friends() {
		struct A {
			int id_;
			friend decltype(friend_to_many);
			A() {
				id_ = friend_to_many(typeid(*this));
			}
		};
		struct B {
			int id_;
			friend decltype(friend_to_many);
			B() {
				id_ = friend_to_many(typeid(*this));
			}
		};

		A a_{};
		B b_{};
	}
	inline void test() {
		using namespace std;
		static auto the_string = string{};
		// auto rv1 = does_not_compile(the_string);
		auto rv2 = does_compile(&the_string);
		auto rv3 = if_constexpr(true);
		auto rv4 = make_vector(&the_string);
		auto && rv5 = Plus(1, 2);

		what_r_pointers();
		what_are_friends();
	}
}

