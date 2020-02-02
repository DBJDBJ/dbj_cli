#pragma once
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms686958(v=vs.85).aspx
#include <windows.h> 
#include <memory.h> 

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

	#define SHMEMSIZE 4096

	/// Prefixing the file mapping object names with "Global\" allows processes 
	/// to communicate with each other even if they are in different terminal 
	/// server sessions. This requires that the creating process must have 
	/// the SeCreateGlobalPrivilege privilege.
	/// #define MAP_OBJECT_NAME "Global\\dbj_shmem_dll_mem_filemap"
	#define MAP_OBJECT_NAME "dbj_shmem_dll_mem_filemap.txt"

	/* 
	Shared memory can be mapped to a different address in each process.For this reason, 
	each process has its own instance of dbj_global_shmem_pointer  , which is declared as a global variable 
	so that it is available to all DLL functions.The example assumes that the DLL global
	data is not shared, so each process that loads the DLL has its own instance of dbj_global_shmem_pointer  .
	*/
	/*static*/ __declspec(dllexport) LPVOID dbj_global_shmem_pointer()  ;      // pointer to shared memory

  // The export mechanism used here is the __declspec(export)
  // method supported by Microsoft Visual Studio, but any
  // other export method supported by your development
  // environment may be substituted.

	/// dbj_set_shared_mem sets the contents of the shared memory 
	__declspec(dllexport)
	 inline VOID __stdcall dbj_set_shared_mem(LPWSTR lpszBuf, DWORD cchSize )
	{
		assert(cchSize < SHMEMSIZE);

		LPWSTR lpszTmp = 0 ;
		DWORD dwCount = 0;

		// Get the address of the shared memory block
		lpszTmp = (LPWSTR)dbj_global_shmem_pointer()  ;

		assert(lpszTmp);

		// Copy the NOT-null-terminated string into shared memory
		while ( dwCount< cchSize )
		{
			*lpszTmp++ = *lpszBuf++;
			dwCount++;
		}
		// at the terminator for a good measure
		*lpszTmp = '\0';
	}

	/// dbj_get_shared_mem gets the contents of the shared memory
	__declspec(dllexport)
		inline VOID __stdcall dbj_get_shared_mem(LPWSTR lpszBuf, DWORD cchSize)
	{
		LPWSTR lpszTmp;

		// Get the address of the shared memory block
		lpszTmp = (LPWSTR)dbj_global_shmem_pointer () ;

		// Copy from shared memory into the caller's buffer
		while (*lpszTmp && --cchSize)
			*lpszBuf++ = *lpszTmp++;
		*lpszBuf = '\0';
	}
#ifdef __cplusplus
}
#endif