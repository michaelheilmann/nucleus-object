// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Signals.h"
#include <stdlib.h>

int
main
    (
        int argc,
        char **argv
    )
{
    Nucleus_Status status;
    status = Nucleus_Signals_initialize();
    if (Nucleus_Unlikely(status)) { return EXIT_FAILURE; }
    status = Nucleus_Signals_initialize();
    if (Nucleus_Unlikely(status)) { return EXIT_FAILURE; }
    Nucleus_Signals_uninitialize();
    Nucleus_Signals_uninitialize();
    return EXIT_SUCCESS;
}
