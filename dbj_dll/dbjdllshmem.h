#pragma once
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms686958(v=vs.85).aspx
#include <windows.h> 
#include <memory.h> 

namespace {

	constexpr auto SHMEMSIZE = 4096;

	/* 
	Shared memory can be mapped to a different address in each process.For this reason, 
	each process has its own instance of lpvMem, which is declared as a global variable 
	so that it is available to all DLL functions.The example assumes that the DLL global
	data is not shared, so each process that loads the DLL has its own instance of lpvMem.
	*/
	static LPVOID lpvMem = NULL;      // pointer to shared memory
	static HANDLE hMapObject = NULL;  // handle to file mapping

	BOOL on_process_attach() {

		BOOL fInit ;

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

	BOOL on_process_detach() {
		// Unmap shared memory from the process's address space

		BOOL fIgnore = UnmapViewOfFile(lpvMem);

		// Close the process's handle to the file-mapping object

		fIgnore = CloseHandle(hMapObject);

		return TRUE;
	}
} // ns

  // The export mechanism used here is the __declspec(export)
  // method supported by Microsoft Visual Studio, but any
  // other export method supported by your development
  // environment may be substituted.

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

	// SetSharedMem sets the contents of the shared memory 

	/*__declspec(dllexport) we use def file in this dll */ 
	VOID __stdcall SetSharedMem(LPWSTR lpszBuf)
	{
		LPWSTR lpszTmp;
		DWORD dwCount = 1;

		// Get the address of the shared memory block

		lpszTmp = (LPWSTR)lpvMem;

		// Copy the null-terminated string into shared memory

		while (*lpszBuf && dwCount<SHMEMSIZE)
		{
			*lpszTmp++ = *lpszBuf++;
			dwCount++;
		}
		*lpszTmp = '\0';
	}

	// GetSharedMem gets the contents of the shared memory

	/* __declspec(dllexport) */
		VOID __stdcall GetSharedMem(LPWSTR lpszBuf, DWORD cchSize)
	{
		LPWSTR lpszTmp;

		// Get the address of the shared memory block

		lpszTmp = (LPWSTR)lpvMem;

		// Copy from shared memory into the caller's buffer

		while (*lpszTmp && --cchSize)
			*lpszBuf++ = *lpszTmp++;
		*lpszBuf = '\0';
	}
#ifdef __cplusplus
}
#endif