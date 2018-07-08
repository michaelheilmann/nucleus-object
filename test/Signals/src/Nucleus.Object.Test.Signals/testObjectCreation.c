// Copyright (c) 2018 Michael Heilmann
#include "Nucleus.Object.Test.Signals/testObjectCreation.h"

Nucleus_Status
testObjectCreation
    (
    )
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
