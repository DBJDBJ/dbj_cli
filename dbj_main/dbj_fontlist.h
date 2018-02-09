#pragma once

#include <Windows.h>

#include <vector>
#include <algorithm>
#include <set>

namespace dbj {

	using char_star  = decltype("XYZ");
	using wchar_star = decltype(L"XYZ");

	auto eq_ = [] (auto & a, auto & b) constexpr -> bool
	{
		return std::is_same_v< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> > ;
	};
	/*
	bellow works for all the std:: string types and string view types
	battery of  if constexpr( std::is_same<>::value )
	would be necessary inside to enforce this usage rule, but ...
	msvc is not there yet with implementing if-constexpr() as required by C++17
	*/
	auto does_start = []( auto val_, auto mat_) {

		static_assert( eq_(val_,mat_) , 
			"dbj::does_start [error] arguments not of the same type"
			);

		if  constexpr(eq_(val_, char_star{})) {
#pragma message ("val type is char *")
			return does_start( std::string(val_), std::string(mat_));
		}
		else if  constexpr(eq_(val_, wchar_star{})) {
#pragma message ("val type is wchar_t *")
			return does_start(std::wstring(val_), std::wstring(mat_));
		}
		else {
			return 0 == val_.compare(0, mat_.size(), mat_);
		}
	};

	/*
	classical overloadin solution
	*/
	template<typename T>
	inline 
	bool starts_with ( const std::basic_string<T> & value, const std::basic_string<T> & match)
	{
			return 0 == value.compare(0, match.size(), match);
	}

	template<typename C, size_t N>
	inline
		bool starts_with(const C (&value_)[N] , const C(&match)[N])
	{
		return starts_with<C>(std::basic_string<C>{value_}, std::basic_string<C>{match});
	}

	template< typename  C = char >
	inline
		bool starts_with(const C * value_, const C * match)
	{
		return starts_with<C>(std::basic_string<C>{value_}, std::basic_string<C>{match});
	}


	template <typename Type>
	inline std::vector<Type> & remove_duplicates(std::vector<Type> & vec) {
		/*
		this is apparently faster when very large data set is used
		std::set<Type> s(vec.begin(), vec.end());
		vec.assign(s.begin(), s.end());
		return vec;
		*/
		// we do not want to sort the input: std::sort(std::begin(arr_), std::end(arr_));
		auto iterator_following_the_last_removed_element = 
			vec.erase(std::unique(std::begin(vec), std::end(vec)), std::end(vec));

		return vec;
	}
	// dbj crazy fast specialized version
	template<typename Type, size_t N>
	static inline void remove_duplicates (const Type(&arr_)[N]) {
		// std::sort(std::begin(arr_), std::end(arr_));
		std::erase( std::unique(std::begin(arr_), std::end(arr_)), std::end(arr_));
	}

	inline auto string_pad (std::string s_, char padchar = ' ', size_t maxlen = 12) {
		// std::string s = "123";
		return s_.insert(0, maxlen - s_.length(), padchar );
	};

	inline auto string_pad (int number_) {
		return string_pad(std::to_string(number_));
	};

	   
	namespace {

		typedef std::pair<LOGFONTW, DWORD>	FontPair;
		typedef std::vector<FontPair>		FontVec;

		// why extern "C" ?
		inline bool comp(FontPair& left, FontPair& right)
		 {
		     if (( L'@' != left.first.lfFaceName[0]) && ( L'@' == right.first.lfFaceName[0]))
		          return true;
		     if (( L'@' == left.first.lfFaceName[0]) && ( L'@' != right.first.lfFaceName[0]))
		          return false;

		     return ( wcscmp(left.first.lfFaceName, right.first.lfFaceName) < 0);
		 }
		
		 /* extern "C" */ 
		inline int CALLBACK enumerateFontsCallBack(
			 ENUMLOGFONTEXW *lpelf,
		     NEWTEXTMETRICEX *lpntm,
		     DWORD fontType, LPARAM lParam)
		 {
		     lpntm;
		
		     FontVec* pFontVec = (FontVec*)lParam;
		     pFontVec->push_back(FontPair(lpelf->elfLogFont, fontType));
		     return TRUE;
		 }

		/**
		  return *all* fonts available
		  by default sort the result by puting names starting with "@" at the end
		  if argument is false do not sort
		*/
		inline FontVec enumerateFonts( bool sort = true )
		{
			FontVec     fonts_vector;
			HDC the_hdc = ::GetWindowDC(::GetConsoleWindow());

			fonts_vector.clear();

			LOGFONTW     lf;
			lf.lfFaceName[0] = L'\0' ;
			lf.lfCharSet = DEFAULT_CHARSET;
			lf.lfPitchAndFamily = 0;
			::EnumFontFamiliesExW(the_hdc, &lf,
				(FONTENUMPROCW)enumerateFontsCallBack,
				(LPARAM)&fonts_vector, 0);

			  if (sort) std::sort(fonts_vector.begin(), fonts_vector.end(), comp);

			  return fonts_vector;

		}

		/**
		 by default remove duplicates before returning
		 if second optional argument is false do not remove duplicates
		*/
		inline 
		std::vector<std::wstring>  
			font_names(
				const FontVec & font_vec_, 
				bool remove_duplicates = true 
			) {
			std::vector<std::wstring> str_vec;
			for (auto el : font_vec_ ) {
				str_vec.push_back(el.first.lfFaceName);
			}

			if (remove_duplicates)
				str_vec = dbj::remove_duplicates(str_vec);

			return str_vec;
		}

	} // anon
} // dbj