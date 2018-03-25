// dbj_main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C" int promise_tests(int argc, wchar_t * argv[]);

/// <summary>
/// Decision: always use wmain 
/// always build as UNICODE
/// </summary>
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
//	extern "C" int promise_tests(int argc, wchar_t * argv[])
	promise_tests(argc, argv);

    return EXIT_SUCCESS;
}


