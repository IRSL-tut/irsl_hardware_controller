#pragma once

#include <signal.h>
#include <stdlib.h> // exit

namespace {

bool setSignalHandler(const int sig_type, sighandler_t handler)
{
    sighandler_t res = signal(sig_type, handler);
    if (res == SIG_ERR) {
        // read errno
        return false;
    }
    return true;
}

} // unnamed namespace
