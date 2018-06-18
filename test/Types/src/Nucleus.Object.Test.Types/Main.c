// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Types.h"
#include <stdlib.h>

int
main
    (
        int argc,
        char **argv
    )
{
    Nucleus_Status status;
    status = Nucleus_Types_initialize();
    if (Nucleus_Unlikely(status)) { return EXIT_FAILURE; }
    status = Nucleus_Types_initialize();
    if (Nucleus_Unlikely(status)) { return EXIT_FAILURE; }
    Nucleus_Types_uninitialize();
    Nucleus_Types_uninitialize();
    return EXIT_SUCCESS;
}
