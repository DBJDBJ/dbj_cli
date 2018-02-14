#pragma once
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>
#include <functional>

namespace dbj {

	using char_star = decltype("XYZ");
	using wchar_star = decltype(L"XYZ");
	/*
	part of C++20, but also implemented here
	http://en.cppreference.com/w/cpp/types/remove_cvref
	*/
	template< class T >
	struct remove_cvref {
		typedef std::remove_cv_t<std::remove_reference_t<T>> type;
	};

	template< class T >
	using remove_cvref_t = typename remove_cvref<T>::type;

	/*
	are two types equal?
	*/
	auto eqt = [](auto & a, auto & b) constexpr -> bool
	{
		return std::is_same_v< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >;
		/*
				this does not catch pointers
				return std::is_same_v< dbj::remove_cvref_t<decltype(a)>, dbj::remove_cvref_t<decltype(b)> >;
		*/
	};

	/*
	array to vector
	*/
	template<typename Type, size_t N, typename outype = std::vector<Type> >
	inline constexpr outype array_to_vector(const Type(&arr_)[N])
	{
		return outype{ arr_, arr_ + N };
	}

	/*
	find an item in anything that has begin and end iterators
	*/
	auto find = [](auto sequence, auto item) constexpr -> bool {
		return std::find(std::begin(sequence), std::end(sequence), item) != std::end(sequence);
	};


	/*
	bellow works for all the std:: string types and string view types
	but it also works for char pointers and wchar_t pointers
	and it is all inside the single lambda
	which is fast because redundant code is removed at compile time
	*/
	auto starts_with = [](auto val_, auto mat_) {

		static_assert(eqt(val_, mat_),
			"dbj::does_start [error] arguments not of the same type"
			);

		if  constexpr(eqt(val_, char_star{})) {
			// #pragma message ("val type is char *")
			return starts_with(std::string(val_), std::string(mat_));
		}
		else if  constexpr(eqt(val_, wchar_star{})) {
			// #pragma message ("val type is wchar_t *")
			return starts_with(std::wstring(val_), std::wstring(mat_));
		}
		else {
			return 0 == val_.compare(0, mat_.size(), mat_);
		}
	};
#if 0
	/*
	classical overloading solution
	*/
	template<typename T = char>
	inline
		bool starts_with(const std::basic_string<T> & value, const std::basic_string<T> & match)
	{
		return 0 == value.compare(0, match.size(), match);
	}

	template<size_t N, typename C = char >
	inline
		bool starts_with(const C(&value_)[N], const C(&match)[N])
	{
		return starts_with<C>(std::basic_string<C>{value_}, std::basic_string<C>{match});
	}

	template< typename  C = char >
	inline
		bool starts_with(const C * value_, const C * match)
	{
		return starts_with<C>(std::basic_string<C>{value_}, std::basic_string<C>{match});
	}
#endif // 0

	/*
	------------------------------------------------------------------------------------
	*/
	template <typename Type>
	inline std::vector<Type> remove_duplicates(const std::vector<Type> & vec, const bool sort = false) {

		if (sort) {
			/*
			this is apparently also faster for very large data sets
			*/
			const std::set<Type> s(vec.begin(), vec.end());
			return std::vector<Type>{ s.begin(), s.end() };
		}
		else {
			std::vector<Type> unique_chunks{};
			for (auto x : vec) {
				if (!dbj::find(unique_chunks, x)) {
					unique_chunks.push_back(x);
				}
			}
			return unique_chunks;
		}
	}

	template <typename Type, size_t N >
	inline std::vector<Type> remove_duplicates(const Type(&arr_)[N], const bool sort = false) {
		return remove_duplicates(std::vector<Type>{arr_, arr_ + N}, sort);
	}

	/*
	------------------------------------------------------------------------------------
	*/
	inline auto string_pad(std::string s_, char padchar = ' ', size_t maxlen = 12) {
		return s_.insert(0, maxlen - s_.length(), padchar);
	};

	inline auto string_pad(int number_) {
		return string_pad(std::to_string(number_));
	};


	/*
	------------------------------------------------------------------------------------
	*/
	inline void quick_test_of_some_new_clever_nuggets() {

		{

			int intarr[]{ 1,1,2,2,3,4,5,6,6,6,7,8,9,9,0,0 };
			auto ret1 = remove_duplicates(intarr);
			std::string as2 [16] {
				"abra", "ka", "dabra", "babra",
				"abra", "ka", "dabra", "babra",
				"abra", "ka", "dabra", "babra",
				"abra", "ka", "dabra", "babra",
			};
	
			auto ad = dbj::remove_duplicates(as2);
			char carr[] = { 'c','a','b','c','c','c','d', 0x0 };
			dbj::remove_duplicates(carr);
			auto see_mee_here = carr;
		}
		{
			auto doesit1 = dbj::starts_with("abra ka dabra", "abra");
			auto doesit2 = dbj::starts_with(L"abra ka dabra", L"abra");
		}
	}

}