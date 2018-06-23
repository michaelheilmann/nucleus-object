// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Types-private.h.i"

DEFINE_MODULE_PRIVATE(Nucleus_Types)

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
{ return destroyClassType(p); }

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
__Nucleus_Types_initialize
    (
        Nucleus_Types *module
    )
{
    if (Nucleus_Unlikely(!module)) return Nucleus_Status_InvalidArgument;
    //
    Nucleus_Status status;
    //
    status = Nucleus_FinalizationHooks_initialize();
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    //
    status = Nucleus_Collections_PointerHashMap_initialize(&module->types, 8,
                                                           NUCLEUS_LOCKFUNCTION(&lockTypeName),
                                                           NUCLEUS_UNLOCKFUNCTION(&unlockTypeName),
                                                           NUCLEUS_HASHFUNCTION(&hashTypeName), // hash
                                                           NUCLEUS_EQUALTOFUNCTION(&equalToTypeName), // equal
                                                           NUCLEUS_LOCKFUNCTION(&lockType),
                                                           NUCLEUS_UNLOCKFUNCTION(&unlockType));
    if (Nucleus_Unlikely(status))
    {
        Nucleus_FinalizationHooks_uninitialize();
        return status; 
    }
    //
    status = Nucleus_Collections_PointerArray_initialize(&module->dynamicLibraries,
                                                         8,
                                                         NUCLEUS_LOCKFUNCTION(&Nucleus_DynamicLibrary_lock),
                                                         NUCLEUS_UNLOCKFUNCTION(&Nucleus_DynamicLibrary_unlock));
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Collections_PointerHashMap_uninitialize(&module->types);
        Nucleus_FinalizationHooks_uninitialize();
        return status; 
    }
    //
    module->referenceCount = 1;
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
__Nucleus_Types_uninitialize
    (
        Nucleus_Types *module
    )
{
    // (1) Uninitialize the types.
    Nucleus_Collections_PointerHashMap_Enumerator e;
    Nucleus_Collections_PointerHashMap_Enumerator_initialize(&e, &module->types);
    while (1)
    {
        Nucleus_Boolean hasValue;
        Nucleus_Collections_PointerHashMap_Enumerator_hasValue(&e, &hasValue);
        if (!hasValue)
        {
            break;
        }
        char *typeName;
        Nucleus_Type *type;
        Nucleus_Collections_PointerHashMap_Enumerator_getValue(&e, (void **)&typeName,
                                                                   (void **)&type);
        Nucleus_FinalizationHooks_invoke(type);
        Nucleus_Collections_PointerHashMap_Enumerator_next(&e);
    }
    Nucleus_Collections_PointerHashMap_Enumerator_uninitialize(&e);

    // (2) Uninitialize the dynamic libraries.
    Nucleus_Collections_PointerArray_uninitialize(&module->dynamicLibraries);
    
    // (4) Uninitialize the finalization hooks.
    Nucleus_FinalizationHooks_uninitialize();
    
    // (5) Return success.
    return Nucleus_Status_Success;
}
