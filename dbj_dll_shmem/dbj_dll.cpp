// dbj_dll.cpp : Defines the exported functions for the DLL application.
// Exported functions from this DLL module
#include "stdafx.h"
// additional headers
#include "../dbj_lib/test.h"

static void __stdcall
		PointW(HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine,
			int nCmdShow) {

	/*
	*/
	dbj::print(
			"\n\nIn :\t", __FUNCSIG__,
			hwnd, " - window handle that should be used as the owner window for any windows this DLL creates",
			hinst, " - this DLL's instance handle",
			lpszCmdLine, " - ASCIIZ command line this DLL should parse",
			nCmdShow, " - describes how this DLL's windows should be displayed"
		);
	}
	/*
	*/
	static void __stdcall
		PointA(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine,
			int nCmdShow) {
		dbj::print(
			"\n\nIn :\t", __FUNCSIG__,
			hwnd, " - window handle that should be used as the owner window for any windows this DLL creates",
			hinst, " - this DLL's instance handle",
			lpszCmdLine, " - ASCIIZ command line this DLL should parse",
			nCmdShow, " - describes how this DLL's windows should be displayed"
		);
	}

	static int __stdcall cpp_testA(const char * argv[BUFSIZ], int argc) {
		dbj::print(
			"\n\nIn :\t", __FUNCSIG__ ,
			"\n argv: ", argv,
			"\n argc: ", argc );
		return 1;
	}

	static int __stdcall cpp_testW(const wchar_t * argv[BUFSIZ], int argc) {
		dbj::print(
			"\n\nIn :\t", __FUNCSIG__,
			"\n argv: ", argv,
			"\n argc: ", argc);
		return 1;
	}

/*
DLL Entry Point functions are called from rundll32.exe, see here

https://support.microsoft.com/en-au/help/164787/info-windows-rundll-and-rundll32-interface

This C functions are in the def file
they are mediators to C++ functions
*/
extern "C" {
	
	/*
	UNICODE
	*/
	void __stdcall
		DBJEPointW(HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine,
			int nCmdShow) {
				PointW(hwnd, hinst, lpszCmdLine, nCmdShow);
	}

	int __stdcall testW(const wchar_t * argv[BUFSIZ], int argc) {
		return cpp_testW(argv, argc);
	}
	/*
	ANSI
	*/
	void __stdcall
		DBJEPointA(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine,
			int nCmdShow) {
				PointA(hwnd, hinst, lpszCmdLine, nCmdShow);
	}

	int __stdcall testA( const char * argv [BUFSIZ], int argc ) {
		return cpp_testA(argv, argc);
	}
}

