// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Object.h"

#include <string.h>
#include <stdarg.h>
#include "Nucleus/Object/TypeSystem.h"
#include "Nucleus/Memory.h"

Nucleus_NonNull() static Nucleus_Status
equalTo
    (
        Nucleus_Object *self,
        Nucleus_Object *other,
        Nucleus_Boolean *equalTo
    );

Nucleus_AlwaysSucceed() static Nucleus_Status
notifyShutdownContext
    (
    );

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
destroy
    (
        Nucleus_Object *self
    );

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_Object_Class *dispatch
    );
    
static Nucleus_Type *g_type = NULL;

Nucleus_NonNull() static Nucleus_Status
equalTo
    (
        Nucleus_Object *self,
        Nucleus_Object *other,
        Nucleus_Boolean *equalTo
    )
{
    if (Nucleus_Unlikely(!self || !other || !equalTo)) return Nucleus_Status_InvalidArgument;
    *equalTo = (self == other);
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() static Nucleus_Status
notifyShutdownContext
    (
    )
{
    g_type = NULL;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
destroy
    (
        Nucleus_Object *self
    )
{
    while (self->type)
    {
        Nucleus_Type *type = self->type;
        if (type->objectDestructor)
        {
            type->objectDestructor(self);
        }
        self->type = type->parentType;
    }
    Nucleus_deallocateMemory(self);
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_Object_Class *dispatch
    )
{
    NUCLEUS_OBJECT_CLASS(dispatch)->equalTo = (Nucleus_NonNull() Nucleus_Status (*)(Nucleus_Object *, Nucleus_Object *, Nucleus_Boolean *))&equalTo;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Object_allocate
    (
        Nucleus_Object **object,
        Nucleus_Size size
    )
{
    Nucleus_Status status;
    Nucleus_Object *temporary;

    if (Nucleus_Unlikely(size < sizeof(Nucleus_Object)))
    { return Nucleus_Status_InvalidArgument; }

    status = Nucleus_allocateMemory((void **)&temporary, size);
    if (Nucleus_Unlikely(status)) return status;
    
    temporary->type = NULL;
    temporary->referenceCount = 1;

    *object = temporary;

    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Object_getType
    (
        Nucleus_Type **type
    )
{
    if (Nucleus_Unlikely(!type)) return Nucleus_Status_InvalidArgument;
    if (Nucleus_Unlikely(!g_type))
    {
        Nucleus_TypeSystem *typeSystem;
        Nucleus_Status status;
        status = Nucleus_TypeSystem_get(&typeSystem);
        if (Nucleus_Unlikely(status)) return status;
        status = Nucleus_TypeSystem_registerClass(typeSystem,
                                                  &g_type,
                                                  "Nucleus.Object",
                                                  sizeof(Nucleus_Object),
                                                  (Nucleus_Status (*)(void *))NULL,
                                                  sizeof(Nucleus_Object_Class),
                                                  (Nucleus_Status (*)(void *))&constructDispatch,
                                                  NULL,
                                                  &notifyShutdownContext);
        if (Nucleus_Unlikely(status)) return status;
    }
    *type = g_type;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Object_construct
    (
        Nucleus_Object *self
    )
{
    Nucleus_Status status;
    Nucleus_Type *type;
    status = Nucleus_Object_getType(&type);
    if (Nucleus_Unlikely(status)) return status;
    self->type = type;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_Object_incrementReferenceCount
    (
        Nucleus_Object *self
    )
{ 
    self->referenceCount++;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_Object_decrementReferenceCount
    (
        Nucleus_Object *self
    )
{ 
    if (0 == --self->referenceCount)
    {
        destroy(self);
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Object_equalTo
    (
        Nucleus_Object *self,
        Nucleus_Object *other,
        Nucleus_Boolean *equalTo
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    return NUCLEUS_OBJECT_CLASS(self->type->dispatch)->equalTo(self, other, equalTo);
}
