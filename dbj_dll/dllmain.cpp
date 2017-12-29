// not stricty necessary
#include "stdafx.h"

#include "dbjdllshmem.h"
/*
	A process loads the DLL (DLL_PROCESS_ATTACH).
	The current process creates a new thread (DLL_THREAD_ATTACH).
	A thread exits normally (DLL_THREAD_DETACH).
	A process unloads the DLL (DLL_PROCESS_DETACH).
*/
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		if (!on_process_attach()) return FALSE;
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		if (!on_process_detach()) return FALSE;
        break;
    }
    return TRUE;
}

