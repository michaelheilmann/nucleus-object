// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/TypeSystem-private.h.i"

Nucleus_NonNull() static Nucleus_Status
lockTypeName
    (
        char *typeName
    )
{ return Nucleus_Status_Success; }

Nucleus_NonNull() static Nucleus_Status
unlockTypeName
    (
        char *typeName
    )
{ return Nucleus_Status_Success; }

Nucleus_NonNull() static Nucleus_Status
lockType
    (
        Nucleus_Type *p
    )
{ return Nucleus_Status_Success; }

Nucleus_NonNull() static Nucleus_Status
unlockType
    (
        Nucleus_Type *p
    )
{ 
    Nucleus_deallocateMemory(p);
    return Nucleus_Status_Success; 
}

Nucleus_NonNull() static Nucleus_Status
hashTypeName
    (
        const char *p,
        Nucleus_HashValue *hashValue
    )
{ return Nucleus_hashMemory(p, strlen(p), hashValue); }


Nucleus_NonNull() static Nucleus_Status
equalToTypeName
    (
        const char *p,
        const char *q,
        Nucleus_Boolean *equalTo
    )
{
    if (Nucleus_Unlikely(!p || !q || !equalTo)) return Nucleus_Status_InvalidArgument;
    *equalTo = !strcmp(p, q); 
    return Nucleus_Status_Success; 
}

Nucleus_NonNull() static Nucleus_Status
initialize
    (
        Nucleus_TypeSystem *typeSystem
    )
{
    if (Nucleus_Unlikely(!typeSystem)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    status = Nucleus_Collections_PointerHashMap_initialize(&typeSystem->types, 8,
                                                           NUCLEUS_LOCKFUNCTION(&lockTypeName),
                                                           NUCLEUS_UNLOCKFUNCTION(&unlockTypeName),
                                                           NUCLEUS_HASHFUNCTION(&hashTypeName), // hash
                                                           NUCLEUS_EQUALTOFUNCTION(&equalToTypeName), // equal
                                                           NUCLEUS_LOCKFUNCTION(&lockType),
                                                           NUCLEUS_UNLOCKFUNCTION(&unlockType));
    if (Nucleus_Unlikely(status))
    {
        return status; 
    }
    typeSystem->referenceCount = 1;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
uninitialize
    (
        Nucleus_TypeSystem *typeSystem
    )
{
    Nucleus_Size i, n;
    Nucleus_Collections_PointerHashMap_getSize(&typeSystem->types, &n); // Must not fail.
    for (i = 0; i < n; ++i)
    {
    }
    Nucleus_Collections_PointerHashMap_uninitialize(&typeSystem->types); // Must not fail.
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
create
    (
        Nucleus_TypeSystem **typeSystem
    )
{
    if (Nucleus_Unlikely(!typeSystem)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    Nucleus_TypeSystem *temporary;
    status = Nucleus_allocateMemory((void **)&temporary, sizeof(Nucleus_TypeSystem));
    if (Nucleus_Unlikely(status)) return status;
    status = initialize(temporary);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(temporary);
        return status;
    }
    *typeSystem = temporary;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
destroy
    (
        Nucleus_TypeSystem *typeSystem
    )
{
    if (Nucleus_Unlikely(!typeSystem)) return Nucleus_Status_InvalidArgument;
    uninitialize(typeSystem);
    Nucleus_deallocateMemory(typeSystem);
    return Nucleus_Status_Success;
}
