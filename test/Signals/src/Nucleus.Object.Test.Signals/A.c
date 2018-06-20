// Copyright (c) 2018 Michael Heilmann
#include "Nucleus.Object.Test.Signals/A.h"

Nucleus_ClassTypeDefinition(Export,
                            "A",
                            A,
                            Nucleus_Object)

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        A_Class *dispatch
    )
{ return Nucleus_Status_Success; }

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
