// 
//

#include "stdafx.h"
#include "../dbj_lib/test.h"

/// <summary>
/// without main/wmain conditionaly used as bellow 
/// cli data provided by UCRT will not be properly set
/// only __argv will be set even in unicode builds
/// </summary>
#ifdef UNICODE
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
#else
int main(int argc, char* argv[], char *envp[])
#endif
{
	dbj::print("About to exit wmain()");
	dbj::log.flush();
	return EXIT_SUCCESS;
}