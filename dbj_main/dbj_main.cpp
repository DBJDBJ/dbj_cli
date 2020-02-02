// dbj_main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static void test_dbj_shlmem()
{
	wchar_t const * to_shmem = L"Hello!" ;

	DWORD mesg_len = DWORD(wcslen(to_shmem));

	dbj_set_shared_mem(LPWSTR(to_shmem), mesg_len );

	wchar_t from_shmem[0xF]{};

	dbj_get_shared_mem(from_shmem, mesg_len);

	assert(!lstrcmp(to_shmem, from_shmem));

}

static void test_almost_certain_cli()
{
	auto [ w_cli, n_cli   ] = dbj::cli_arguments();

	if (w_cli.size())
		dbj::print(w_cli);

	if (n_cli.size())
		dbj::print(n_cli);

}
/// <summary>
/// Decision: always use wmain 
/// always build as UNICODE
/// </summary>
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	test_dbj_shlmem();
	test_almost_certain_cli();
	static auto test_ = dbj::test();

    return EXIT_SUCCESS;
}


