// This file is part of the iff library
//
// Copyright (c) 2018 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "stdmain.h"
#include <signal.h>

/// Called when a signal is received.
static void OnSignal (int sig) noexcept
{
    printf ("Fatal error: %s received.\n", strsignal(sig));
    enum { qc_ShellSignalQuitOffset = 128 };
    exit (qc_ShellSignalQuitOffset+sig);
}

/// Called by the framework on unrecoverable exception handling errors.
static void Terminate (void) noexcept
{
    assert (!"Unrecoverable exception handling error detected.");
    raise (SIGABRT);
    exit (EXIT_FAILURE);
}

/// Installs OnSignal as handler for signals.
extern "C" void InstallCleanupHandlers (void) noexcept
{
    static const uint8_t c_Signals[] = {
	SIGINT, SIGQUIT, SIGILL,  SIGTRAP, SIGABRT,
	SIGIOT, SIGBUS,  SIGFPE,  SIGSEGV, SIGTERM,
	SIGIO,  SIGCHLD
    };
    for (auto i = 0u; i < VectorSize(c_Signals); ++ i)
	signal (c_Signals[i], OnSignal);
    std::set_terminate (Terminate);
}
