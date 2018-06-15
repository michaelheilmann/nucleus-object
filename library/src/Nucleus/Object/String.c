// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/String.h"

#include "Nucleus/Memory.h"
#include "Nucleus/Hash/Memory.h"
#include <string.h>

Nucleus_ClassTypeDefinition(Nucleus_Object_Library_Export,
                            "Nucleus.String",
                            Nucleus_String,
                            Nucleus_Object)

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_String_Class *dispatch
    )
{ return Nucleus_Status_Success; }

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
destruct
    (
        Nucleus_String *self
    )
{
    Nucleus_deallocateMemory(self->bytes);
    self->bytes = NULL;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_String_construct
    (
        Nucleus_String *self,
        const char *bytes
    )
{
    //
    Nucleus_Type *type;
    Nucleus_Status status;
    status = Nucleus_String_getType(&type);
    if (Nucleus_Unlikely(status)) return status;
    //
    Nucleus_Object_construct(NUCLEUS_OBJECT(self));
    //
    Nucleus_Size numberOfBytes = strlen(bytes) + 1;
    status = Nucleus_allocateArrayMemory((void **)&self->bytes,
                                         numberOfBytes,
                                         sizeof(char));
    if (Nucleus_Unlikely(status)) return status;
    Nucleus_copyArrayMemory((void *)self->bytes, (void *)bytes, numberOfBytes, sizeof(char));
    //
    NUCLEUS_OBJECT(self)->type = type;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_String_create
    (
        Nucleus_String **string,
        const char *bytes
    )
{
    if (Nucleus_Unlikely(!string || !bytes)) return Nucleus_Status_InvalidArgument;
    Nucleus_String *temporary;
    Nucleus_Status status;
    //
    status = Nucleus_Object_allocate((Nucleus_Object **)&temporary,
                                     sizeof(Nucleus_String));
    if (Nucleus_Unlikely(status)) return status;
    //
    status = Nucleus_String_construct(temporary, bytes);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(temporary));
        return status;
    }
    //
    *string = temporary;
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_String_equalTo
    (
        Nucleus_String *self,
        Nucleus_String *other,
        Nucleus_Boolean *equalTo
    )
{
    if (Nucleus_Unlikely(!self || !other || !equalTo)) return Nucleus_Status_InvalidArgument;
    if (self == other)
    {
        *equalTo = Nucleus_Boolean_True;
        return Nucleus_Status_Success;
    }
    else
    {
        *equalTo = !strcmp(self->bytes, other->bytes);
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_String_hash
    (
        Nucleus_String *self,
        Nucleus_HashValue *hashValue
    )
{
    if (Nucleus_Unlikely(!self || !hashValue)) return Nucleus_Status_InvalidArgument;
    return Nucleus_hashMemory(self->bytes, strlen(self->bytes), hashValue);
}
