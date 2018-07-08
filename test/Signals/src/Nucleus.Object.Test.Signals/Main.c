// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Signals.h"
#include "Nucleus.Object.Test.Signals/A.h"
#include "Nucleus.Object.Test.Signals/testTypeConstruction.h"
#include "Nucleus.Object.Test.Signals/testObjectCreation.h"
#include "Nucleus.Object.Test.Signals/testExplicitDisconnect.h"
#include "Nucleus.Object.Test.Signals/testObjectDestruction.h"
#include <stdlib.h>
#include <stdio.h>

int
main
    (
        int argc,
        char **argv
    )
{
    Nucleus_Status status;
    // Initialize signals twice to test reference counted singletons.
    status = Nucleus_Signals_initialize();
    if (Nucleus_Unlikely(status))
    {
        fprintf(stderr, u8"%s failed\n", u8"Nucleus_Signals_initialize");
        return EXIT_FAILURE;
    }
    //
    status = Nucleus_Signals_initialize();
    if (Nucleus_Unlikely(status))
    {
        fprintf(stderr, u8"%s failed\n", u8"Nucleus_Signals_initialize");
        return EXIT_FAILURE;
    }
    //
    status = testTypeConstruction();
    if (Nucleus_Unlikely(status)) goto End;
    status = testObjectCreation();
    if (Nucleus_Unlikely(status)) goto End;
    status = testExplicitDisconnect();
    if (Nucleus_Unlikely(status)) goto End;
    status = testObjectDestruction();
    if (Nucleus_Unlikely(status)) goto End;
End:
    // Uninitialize signals twice.
    Nucleus_Signals_uninitialize();
    Nucleus_Signals_uninitialize();
    // Return success.
    if (status) return EXIT_FAILURE;
    else return EXIT_SUCCESS;
}
