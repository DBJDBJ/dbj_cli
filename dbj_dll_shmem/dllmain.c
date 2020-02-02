#include "stdafx.h"

#include "dbjdllshmem.h"

	static HANDLE hMapObject = NULL;  // handle to file mapping

	LPVOID dbj_global_shmem_pointer() {
		LPVOID dbj_global_shmem_pointer_ = NULL;
		return dbj_global_shmem_pointer_ ;
	}

	static BOOL on_process_attach() {

		BOOL fInit;

		/// Create a named file mapping object
		hMapObject = CreateFileMappingW(
			INVALID_HANDLE_VALUE,   // use paging file
			NULL,                   // default security attributes
			PAGE_READWRITE,         // read/write access
			0,                      // size: high 32-bits
			SHMEMSIZE,              // size: low 32-bits
			TEXT(MAP_OBJECT_NAME)  // name of map object
		);

		if (hMapObject == NULL) {
			
			DBJ_TRACE("Could not create file mapping object (%d).\n",	GetLastError());

			return FALSE;
		}

		/// The first process to attach initializes memory
		fInit = (GetLastError() != ERROR_ALREADY_EXISTS);

		/// Get a pointer to the file-mapped shared memory
		LPVOID dgsp_ = dbj_global_shmem_pointer();
		dgsp_ = MapViewOfFile(
			hMapObject,     // object to map view of
			FILE_MAP_ALL_ACCESS, // read/write access
			0,              // high offset:  map from
			0,              // low offset:   beginning
			SHMEMSIZE);             // default: map entire file
		
		if (dbj_global_shmem_pointer() == NULL)
		{
			printf("Could not map view of file (%d).\n", GetLastError());
			CloseHandle(hMapObject);
				return FALSE;
		}

		/// zero the  memory if this is the first process
		if (fInit)
			memset(dbj_global_shmem_pointer(), '\0', SHMEMSIZE);

		return TRUE;
	}

	///----------------------------------------------------------------------
	static BOOL on_process_detach() {
		// Unmap shared memory from the process's address space
		BOOL fIgnore = UnmapViewOfFile(dbj_global_shmem_pointer() );

		// Close the process's handle to the file-mapping object
		fIgnore = CloseHandle(hMapObject);

		return TRUE;
	}
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
		if (!on_process_attach()) return FALSE;
		break;
	case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		if (!on_process_detach()) return FALSE;
        break;
    }
    return TRUE;
}

