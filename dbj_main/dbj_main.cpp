// dbj_main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static void test_almost_certain_cli()
{
	auto [ w_cli, n_cli   ] = dbj::cli_almost_certain();

	if (w_cli.size())
		dbj::detail::print(w_cli);

	if (n_cli.size())
		dbj::detail::print(n_cli);

}
/// <summary>
/// Decision: always use wmain 
/// always build as UNICODE
/// </summary>
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	test_almost_certain_cli();
	static auto test_ = dbj::test();

    return EXIT_SUCCESS;
}


