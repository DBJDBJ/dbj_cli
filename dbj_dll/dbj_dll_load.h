#pragma once

#ifndef _INC_WINDOWS
		#define WIN32_LEAN_AND_MEAN
		#define STRICT
		#define NOMINMAX
		#include <windows.h>
#endif

#include <exception>    // for std::set_terminate
#include <string>
/*
#include <cstdlib>      // for std::exit, EXIT_FAILURE, EXIT_SUCCESS
*/

namespace dbj {

	using CHAR_T = char;
	using STRING = std::string;
	using long_char_pointer = CHAR_T * ; // LPWSTR;

	namespace sysinfo {

		namespace {
			DWORD	INFO_BUFFER_SIZE = 1024;
					static STRING  infoBuf(INFO_BUFFER_SIZE, (CHAR_T)0);
			/*
			usage: 
			std::string compname = call(GetComputerName, infoBuf.data(), &INFO_BUFFER_SIZE);
			*/
			template<class F, class... Pack>
			constexpr __forceinline auto
				call (F&& fun, Pack&&... args) {
					infoBuf.clear();
				if (0 == std::invoke(fun, (args)...))
					throw std::runtime_error (typeid(F).name());
				return (infoBuf);
			}
		}
		// 
		__forceinline STRING computer_name() {
			static STRING computer_name_ =
				call(GetComputerName, infoBuf.data(), &INFO_BUFFER_SIZE);
			return computer_name_;
		}

		__forceinline STRING user_name() {
			static STRING user_name_ =
				call(GetUserName, infoBuf.data(), &INFO_BUFFER_SIZE);
			return user_name_;
		}

		__forceinline STRING system_directory() {
			static STRING system_directory_ =
				call(GetSystemDirectory, infoBuf.data(), INFO_BUFFER_SIZE);
			return system_directory_;
		}

		__forceinline STRING windows_directory() {
			static STRING windows_directory_ =
				call(GetWindowsDirectory, infoBuf.data(), INFO_BUFFER_SIZE);
			return windows_directory_;
		}
	} // sysinfo

		class dll_load final
		{
			// TODO: map of functions as a cache 
			HINSTANCE				dllHandle_;
			mutable	std::string		dllName_;
			mutable	bool			is_system_module = false;

			dll_load() {} ;
		public:
			dll_load(const std::string & dllFname, bool system_mod = false)
				: dllHandle_(NULL), dllName_(dllFname), is_system_module(system_mod)
			{
				if (dllName_.empty())
					throw std::exception("dbj::dll_load ctor needs dll or exe name as the first argument");

				if (is_system_module) {
					dllName_ = sysinfo::system_directory() + "\\" + dllName_;
				}
				auto libpath = dllName_.data();
				dllHandle_ = ::LoadLibraryA(libpath);
				// address of filename of executable module 
				if (NULL == dllHandle_)
					throw std::runtime_error("Could not find the DLL named: " + dllName_);
			}
			/*
			FreeLibrary() failure is very rare and might signal some deep error with the machines or OS
			thus we will not ignore it.
			NOTE: C++11/14/17/20 standard behavior is not to throw exceptions from destructor
			*/
			~dll_load()
			{
				if (NULL != dllHandle_)
					if (!FreeLibrary(dllHandle_))
					{
						::OutputDebugStringA(
							(dllName_ +  " failed to be freed by FreeLibrary()?!").data()
						);
					}
			}
			/*
			TODO: map< func name, func handle > as a cache?
			*/
			FARPROC function (const CHAR_T * funName)
			{
			_ASSERTE(NULL != this->dllHandle_);
			FARPROC 	result =
					::GetProcAddress(
					(HMODULE)dllHandle_,	// handle to DLL module 
						funName				// name of a function 
					);
					_ASSERTE(result);
				return result;
			}

		};
} // dbj

  /* Copyright 2017 dbj@dbj.org

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http ://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
  */
#pragma comment( user, __FILE__ "(c) 2017 by dbj@dbj.org | Version: " __DATE__ __TIME__ ) 