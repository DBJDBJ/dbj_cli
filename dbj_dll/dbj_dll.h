#pragma once
/*
dbj dll api
*/

extern "C" {

#ifndef BUFSIZ
#define BUFSIZ 512
#endif

	/*
	UNICODE
	*/
	void __stdcall
		DBJEPointW(HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine,
			int nCmdShow);
	/*
	ANSI
	*/
	void __stdcall
		DBJEPointA(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine,
			int nCmdShow);

	int __stdcall testA(const char * argv[BUFSIZ], int argc);

	int __stdcall testW(const wchar_t * argv[BUFSIZ], int argc);

}

