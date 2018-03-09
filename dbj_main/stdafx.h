// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include "../dbj_lib/dbj_rt.h"
#include "../dbj_lib/dbj_clidata.h"
#include "../dbj_lib/steve_wishnousky_cli_data.h"

// DBJ: _CONCAT is apparently defined in msvc <typetraits>
#ifndef _CONCAT
#define _CONCATX(x, y) x ## y
#define _CONCAT(x, y) _CONCATX(x, y)
#endif



