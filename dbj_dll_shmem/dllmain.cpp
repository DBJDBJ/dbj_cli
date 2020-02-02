// not stricty necessary
#include "stdafx.h"

#include "dbjdllshmem.h"

namespace dbj::shmem {

	static HANDLE hMapObject = NULL;  // handle to file mapping

	extern "C" inline BOOL on_process_attach() {

		BOOL fInit;

		// Create a named file mapping object

		hMapObject = CreateFileMapping(
			INVALID_HANDLE_VALUE,   // use paging file
			NULL,                   // default security attributes
			PAGE_READWRITE,         // read/write access
			0,                      // size: high 32-bits
			SHMEMSIZE,              // size: low 32-bits
			TEXT("dllmemfilemap")); // name of map object
		if (hMapObject == NULL)
			return FALSE;

		// The first process to attach initializes memory

		fInit = (GetLastError() != ERROR_ALREADY_EXISTS);

		// Get a pointer to the file-mapped shared memory

		lpvMem = MapViewOfFile(
			hMapObject,     // object to map view of
			FILE_MAP_WRITE, // read/write access
			0,              // high offset:  map from
			0,              // low offset:   beginning
			0);             // default: map entire file
		if (lpvMem == NULL)
			return FALSE;

		// Initialize memory if this is the first process

		if (fInit)
			memset(lpvMem, '\0', SHMEMSIZE);

		return TRUE;
	}

	extern "C" inline BOOL on_process_detach() {
		// Unmap shared memory from the process's address space

		BOOL fIgnore = UnmapViewOfFile(lpvMem);

		// Close the process's handle to the file-mapping object

		fIgnore = CloseHandle(hMapObject);

		return TRUE;
	}
} // ns
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
		if (!dbj::shmem::on_process_attach()) return FALSE;
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		if (!dbj::shmem::on_process_detach()) return FALSE;
        break;
    }
    return TRUE;
}

