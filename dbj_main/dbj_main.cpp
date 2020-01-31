// dbj_main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/// <summary>
/// Decision: always use wmain 
/// always build as UNICODE
/// </summary>
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	static auto test_ = dbj::test();

    return EXIT_SUCCESS;
}


