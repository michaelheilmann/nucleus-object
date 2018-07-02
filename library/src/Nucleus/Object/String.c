// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/String.h"

#include "Nucleus/Memory.h"
#include "Nucleus/Hash/Memory.h"
#include <string.h>

Nucleus_ClassTypeDefinition(Nucleus_Object_Library_Export,
                            u8"Nucleus.String",
                            Nucleus_String,
                            Nucleus_Object)
                            
Nucleus_NonNull() static Nucleus_Status
equalTo
    (
        Nucleus_String *self,
        Nucleus_Object *other,
        Nucleus_Boolean *equalTo
    );

Nucleus_NonNull() static Nucleus_Status
hash
    (
        Nucleus_String *self,
        Nucleus_HashValue *hashValue
    );

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_String_Class *dispatch
    )
{
    NUCLEUS_OBJECT_CLASS(dispatch)->equalTo = (Nucleus_Status (*)(Nucleus_Object *, Nucleus_Object *, Nucleus_Boolean *))&equalTo;
    NUCLEUS_OBJECT_CLASS(dispatch)->hash = (Nucleus_Status (*)(Nucleus_Object *, Nucleus_HashValue *))&hash;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructSignals
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

Nucleus_NonNull() static Nucleus_Status
equalTo
    (
        Nucleus_String *self,
        Nucleus_Object *other,
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
        Nucleus_Type *stringType;
        Nucleus_String_getType(&stringType);
        if (stringType != other->type)
        {
            *equalTo = Nucleus_Boolean_False;
        }
        else
        {
            *equalTo = !strcmp(self->bytes, NUCLEUS_STRING(other)->bytes);
        }
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
hash
    (
        Nucleus_String *self,
        Nucleus_HashValue *hashValue
    )
{
    if (Nucleus_Unlikely(!self || !hashValue)) return Nucleus_Status_InvalidArgument;
    return Nucleus_hashMemory(self->bytes, strlen(self->bytes), hashValue);
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

Nucleus_DefineCreate(Nucleus_String,
                     Nucleus_Parameters(const char *bytes),
                     Nucleus_Arguments(bytes))
