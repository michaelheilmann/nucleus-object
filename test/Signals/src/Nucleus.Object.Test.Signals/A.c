// Copyright (c) 2018 Michael Heilmann
#include "Nucleus.Object.Test.Signals/A.h"
#include <stdio.h>

Nucleus_ClassTypeDefinition(Export,
                            u8"A",
                            A,
                            Nucleus_Object)

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        A_Class *dispatch
    )
{ return Nucleus_Status_Success; }

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructSignals
    (
        A_Class *dispatch
    )
{
	Nucleus_Status status;
    // Add a signal.
	fprintf(stdout, u8"installing signal %s\n", u8"keyboard-key-pressed");
    status = Nucleus_Signals_addSignal(u8"keyboard-key-pressed",
                                       NUCLEUS_OBJECT_CLASS(dispatch)->type);
    if (Nucleus_Unlikely(status))
    {
		fprintf(stdout, u8"error installing signal %s\n", u8"keyboard-key-pressed");
		return status;
	}
	fprintf(stdout, u8"installed signal %s\n", u8"keyboard-key-pressed");
    // Add a signal.
	fprintf(stdout, u8"installing signal %s\n", u8"keyboard-key-released");
    status = Nucleus_Signals_addSignal(u8"keyboard-key-released",
                                       NUCLEUS_OBJECT_CLASS(dispatch)->type);
    if (Nucleus_Unlikely(status))
    {
		fprintf(stdout, u8"error installing signal %s\n", u8"keyboard-key-released");
		return status;
	}
	fprintf(stdout, u8"installed signal %s\n", u8"keyboard-key-released");
	// Return success.
	return Nucleus_Status_Success;
}

Nucleus_NonNull(1) static Nucleus_Status
destruct
    (
        A *self
    )
{ return Nucleus_Status_Success; }


Nucleus_NonNull() Nucleus_Status
A_construct
    (
        A *self
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    Nucleus_Type *type;
    Nucleus_Status status;
    status = A_getType(&type);
    if (Nucleus_Unlikely(status)) return status;
    status = Nucleus_Object_construct(NUCLEUS_OBJECT(self));
    if (Nucleus_Unlikely(status)) return status;
    NUCLEUS_OBJECT(self)->type = type;
    return Nucleus_Status_Success;
}

Nucleus_DefineDefaultCreate(A)
