// This file is part of the iff library
//
// Copyright (c) 2018 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// Standard program main with error handling.

#pragma once
#include "../utils.h"
#include <stdio.h>
using namespace iff;
using namespace ustl;

extern "C" void InstallCleanupHandlers (void) noexcept;

/// Exception handling harness for demos
template <typename T>
inline int RunTest (void) noexcept
{
    int rv = EXIT_FAILURE;
    try {
	T::Instance().Run();
	rv = EXIT_SUCCESS;
    } catch (ustl::exception& e) {
	cout.flush();
	cerr << "Error: " << e << endl;
    } catch (...) {
	cout.flush();
	cerr << "Unexpected error." << endl;
    }
    return rv;
}

/// Standard main with error handling
#define StdTestMain(TestClass)	\
int main (void)			\
{				\
    InstallCleanupHandlers();	\
    return RunTest<TestClass>();\
}
