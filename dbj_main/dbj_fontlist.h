#pragma once

#include <Windows.h>

#include <vector>
#include <algorithm>

namespace dbj {
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

		inline FontVec enumerateFonts()
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

			  std::sort(fonts_vector.begin(), fonts_vector.end(), comp);

			  return fonts_vector;

		}

		std::vector<std::wstring>  font_names(const FontVec & font_vec_ ) {
			std::vector<std::wstring> str_vec;
			for (auto el : font_vec_ ) {
				str_vec.push_back(el.first.lfFaceName);
			}
			return str_vec;
		}

	} // anon
} // dbj