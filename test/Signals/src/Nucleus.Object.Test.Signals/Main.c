// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Signals.h"
#include "Nucleus.Object.Test.Signals/A.h"
#include <stdlib.h>
#include <stdio.h>

static Nucleus_Status
test1();

static Nucleus_Status
test2();

static Nucleus_Status
test1()
{
	Nucleus_Status status;
	Nucleus_Type *type;
	status = A_getType(&type);
	if (Nucleus_Unlikely(status)) return status;
	Nucleus_ClassType_initialize(NUCLEUS_CLASSTYPE(type));
    // Return success.
    return Nucleus_Status_Success;
}

static Nucleus_Status
test2()
{
	Nucleus_Status status;
    A *a;
    // Create instance of "A".
    status = A_create(&a);
    if (Nucleus_Unlikely(status))
    { return status; }
    // Destroy instance of "A".
    Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(a));
    // Return success.
    return Nucleus_Status_Success;
}

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
    status = test1();
    if (Nucleus_Unlikely(status))
    {
        goto End;
    }
	status = test2();
	if (Nucleus_Unlikely(status))
	{
		goto End;
	}
End:
    // Uninitialize signals twice.
    Nucleus_Signals_uninitialize();
    Nucleus_Signals_uninitialize();
    // Return success.
    if (status) return EXIT_FAILURE;
    else return EXIT_SUCCESS;
}
