#include "Nucleus/Object/ObjectArray.h"
#include "Nucleus/Object/ObjectArrayEnumerator.h"

Nucleus_ClassTypeDefinition(Nucleus_Object_Library_Export,
                            u8"Nucleus.ObjectArray",
                            Nucleus_ObjectArray,
                            Nucleus_Object)

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_ObjectArray_Class *dispatch
    )
{ return Nucleus_Status_Success; }

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructSignals
    (
        Nucleus_ObjectArray_Class *dispatch
    )
{ return Nucleus_Status_Success; }

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
destruct
    (
        Nucleus_ObjectArray *self
    )
{ return Nucleus_Status_Success; }

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_construct
    (
        Nucleus_ObjectArray *self
    )
{
    //
    Nucleus_Type *type;
    Nucleus_Status status;
    // (1) Get the type.
    status = Nucleus_ObjectArray_getType(&type);
    if (Nucleus_Unlikely(status)) return status;
    // (2) Invoke constructor of parent class.
    Nucleus_Object_construct(NUCLEUS_OBJECT(self));
    // (3) 
    status = Nucleus_Collections_PointerArray_initialize(&self->implementation,
                                                         8,
                                                         NUCLEUS_LOCKFUNCTION(&Nucleus_Object_incrementReferenceCount),
                                                         NUCLEUS_UNLOCKFUNCTION(&Nucleus_Object_decrementReferenceCount));
    if (Nucleus_Unlikely(status)) return status;
    //
    NUCLEUS_OBJECT(self)->type = type;
    return Nucleus_Status_Success;
}

Nucleus_DefineDefaultCreate(Nucleus_ObjectArray)

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_increaseCapacity
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size requiredAdditionalCapacity
    )
{
    return Nucleus_Collections_PointerArray_increaseCapacity(&self->implementation,
                                                             requiredAdditionalCapacity);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_ensureFreeCapacity
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size requiredFreeCapacity
    )
{
    return Nucleus_Collections_PointerArray_ensureFreeCapacity(&self->implementation,
                                                               requiredFreeCapacity);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_prepend
    (
        Nucleus_ObjectArray *self,
        Nucleus_Object *object
    )
{
    return Nucleus_Collections_PointerArray_prepend(&self->implementation, object);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_append
    (
        Nucleus_ObjectArray *self,
        Nucleus_Object *object
    )
{
    return Nucleus_Collections_PointerArray_append(&self->implementation, object);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_insert
    (
        Nucleus_ObjectArray *self,
        Nucleus_Object *object,
        Nucleus_Size index
    )
{
    return Nucleus_Collections_PointerArray_insert(&self->implementation, object, index);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_at
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size index,
        Nucleus_Object **object
    )
{
    return Nucleus_Collections_PointerArray_at(&self->implementation, index, (void **)object);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_getSize
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size *size
    )
{
    return Nucleus_Collections_PointerArray_getSize(&self->implementation, size);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_getCapacity
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size *capacity
    )
{
    return Nucleus_Collections_PointerArray_getCapacity(&self->implementation, capacity);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_getFreeCapacity
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size *freeCapacity
    )
{
    return Nucleus_Collections_PointerArray_getFreeCapacity(&self->implementation, freeCapacity);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_clear
    (
        Nucleus_ObjectArray *self
    )
{
    return Nucleus_Collections_PointerArray_clear(&self->implementation);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_getEnumerator
    (
        Nucleus_ObjectArray *self,
        Nucleus_ObjectEnumerator **enumerator
    )
{
    return Nucleus_ObjectArrayEnumerator_create((Nucleus_ObjectArrayEnumerator **)enumerator, self);
}
