// dbj_main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../dbj_lib/test.h"

#include "../dbj_lib/lambda_runtime_retval_test.h"

int main()
{
	lambda_runtime_retval_test::test_();

	dbj::log.flush();
    return 0;
}


