#pragma once
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms686958(v=vs.85).aspx
#include <windows.h> 
#include <memory.h> 

namespace dbj::shmem {

	constexpr auto SHMEMSIZE = 4096;
	constexpr TCHAR MAP_OBJECT_NAME[]{ TEXT("dllmemfilemap") } ;

	/* 
	Shared memory can be mapped to a different address in each process.For this reason, 
	each process has its own instance of lpvMem, which is declared as a global variable 
	so that it is available to all DLL functions.The example assumes that the DLL global
	data is not shared, so each process that loads the DLL has its own instance of lpvMem.
	*/
	static LPVOID lpvMem = NULL;      // pointer to shared memory

} // ns

  // The export mechanism used here is the __declspec(export)
  // method supported by Microsoft Visual Studio, but any
  // other export method supported by your development
  // environment may be substituted.

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

	// SetSharedMem sets the contents of the shared memory 
	__declspec(dllexport)
	 inline VOID __stdcall SetSharedMem(LPWSTR lpszBuf)
	{
		LPWSTR lpszTmp;
		DWORD dwCount = 1;

		// Get the address of the shared memory block

		lpszTmp = (LPWSTR)dbj::shmem::lpvMem;

		// Copy the null-terminated string into shared memory

		while (*lpszBuf && dwCount< dbj::shmem::SHMEMSIZE)
		{
			*lpszTmp++ = *lpszBuf++;
			dwCount++;
		}
		*lpszTmp = '\0';
	}

	// GetSharedMem gets the contents of the shared memory

	__declspec(dllexport)
		inline VOID __stdcall GetSharedMem(LPWSTR lpszBuf, DWORD cchSize)
	{
		LPWSTR lpszTmp;

		// Get the address of the shared memory block

		lpszTmp = (LPWSTR)dbj::shmem::lpvMem;

		// Copy from shared memory into the caller's buffer

		while (*lpszTmp && --cchSize)
			*lpszBuf++ = *lpszTmp++;
		*lpszBuf = '\0';
	}
#ifdef __cplusplus
}
#endif