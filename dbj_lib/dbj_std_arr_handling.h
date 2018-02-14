#pragma once

#include <vector>
#include <array>

namespace dbj {
	/*
	------------------------------------------------------------------------------------
	declare inbuilt array reference to the inbuilt array inside std::array

	genericaly this is
	array< T, N > as ;
	// T is as::value_type
	T(&ref)[N] = *(T(*)[N](as.data())
	*/
	template<typename T, size_t N,
		typename ARR = std::array<T, N>,
		typename ART = T[N],
		typename ARF = ART & ,
		typename ARP = ART * >
		constexpr ARF
		array_to_arf(const std::array<T, N> & arr)
		// ->ARF
	{
		return *(ARP) const_cast<typename ARR::pointer>(arr.data());
	}

	/*
	Array Handler

	(c) 2018 by dbj.org
	*/
	template< typename T, size_t N >
	struct ARH
	{
		// std::array type
		typedef std::array<T, N> ARR;
		// inbuilt ARray type
		typedef T ART[N];
		// reference to ART
		typedef ART& ARF;
		// pointer to ART
		typedef ART* ARP;

		/*
		return pointer to the underlying array
		of an instance of std::array<T,N>
		*/
		static constexpr ARP
			to_arp(const ARR & arr)
		{
			return (ARP)const_cast<typename ARR::pointer>(arr.data());
		}

		/*
		return reference to the underlying array
		of an instance of std::array<T,N>
		*/
		static constexpr ARF
			to_arf(const ARR & arr)
		{
			return *(ARP) const_cast<typename ARR::pointer>(arr.data());
		}
		/*
		A arr[N];
		A (&ref)[N] = *static_cast<A(*)[N]>(&arr);
		std::string arr[16];
		std::string(&ref)[16] = * (std::string(*)[16])(as.data());
		std::string(&ref)[16] = array_to_array(as);
		*/
	};

	namespace {
		/*
		array to vector
		*/
		template<typename Type, size_t N, typename VType = std::vector<Type> >
		inline constexpr VType 
			intrinsic_array_to_vector(const Type(&arr_)[N])
		{
			return VType{ arr_, arr_ + N };
		}
	}

	inline void test_dbj_std_arr_handling ()
	{
		using A16 = ARH<int, 16>;
		A16::ARR arr;
		std::generate(
			arr.begin(), arr.end(), [count = 0]() mutable -> int {
			return count++;
		});
		A16::ARP arp = A16::to_arp(arr);
		A16::ARF arf = A16::to_arf(arr);

		auto rdr0 = intrinsic_array_to_vector(arf);

		decltype(auto) arf2 = array_to_arf(arr);

		decltype(auto) rdr1 = intrinsic_array_to_vector(arf2);

		decltype(auto) arf3 = arf2;
		auto rdr2 = intrinsic_array_to_vector(arf3);
	}
}