// dbj_main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../dbj_lib/test.h"

int main()
{
	dbj::print("About to exit main()");
	dbj::log.flush();
    return 0;
}

