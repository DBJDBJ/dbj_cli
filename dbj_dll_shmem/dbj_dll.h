#pragma once
/*
dbj dll api
*/

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

#ifndef BUFSIZ
#define BUFSIZ 512
#endif

	/*
	UNICODE
	*/
	void __stdcall
		DBJEPointW(HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine,
			int nCmdShow);

	int __stdcall testW(const wchar_t * argv[BUFSIZ], int argc);
	/*
	ANSI
	*/
	void __stdcall
		DBJEPointA(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine,
			int nCmdShow);

	int __stdcall testA(const char * argv[BUFSIZ], int argc);

#ifdef __cplusplus    // If used by C++ code, 
} // extern "C" 
#endif