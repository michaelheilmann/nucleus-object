// Copyright (c) 2018 Michael Heilmann
#include "Nucleus.Object.Test.Signals/testTypeConstruction.h"

Nucleus_Status
testTypeConstruction
    (
    )
{
	Nucleus_Status status;
	Nucleus_Type *type;
	status = A_getType(&type);
	if (Nucleus_Unlikely(status)) return status;
	Nucleus_ClassType_initialize(NUCLEUS_CLASSTYPE(type));
    // Return success.
    return Nucleus_Status_Success;
}
