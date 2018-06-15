// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/TypeSystem.h"
#include <stdlib.h>

int
main
    (
        int argc,
        char **argv
    )
{
    Nucleus_Status status;
    status = Nucleus_TypeSystem_startup();
    if (Nucleus_Unlikely(status)) { return EXIT_FAILURE; }
    status = Nucleus_TypeSystem_startup();
    if (Nucleus_Unlikely(status)) { return EXIT_FAILURE; }
    Nucleus_TypeSystem_shutdown();
    Nucleus_TypeSystem_shutdown();
    return EXIT_SUCCESS;
}
