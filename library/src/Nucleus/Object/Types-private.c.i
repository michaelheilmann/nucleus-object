// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Types-private.h.i"

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
initialize
    (
        Nucleus_TypeSystem *types
    )
{
    if (Nucleus_Unlikely(!types)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    status = Nucleus_Collections_PointerHashMap_initialize(&types->types, 8,
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
    status = Nucleus_Collections_PointerArray_initialize(&types->dynamicLibraries,
                                                         8,
                                                         NUCLEUS_LOCKFUNCTION(&Nucleus_DynamicLibrary_lock),
                                                         NUCLEUS_UNLOCKFUNCTION(&Nucleus_DynamicLibrary_unlock));
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Collections_PointerHashMap_uninitialize(&types->types);
        return status; 
    }
    types->referenceCount = 1;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
uninitialize
    (
        Nucleus_TypeSystem *types
    )
{
    // (1) Uninitialize the types.
    Nucleus_Collections_PointerHashMap_Enumerator e;
    Nucleus_Collections_PointerHashMap_Enumerator_initialize(&e, &types->types);
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
        if (type->notifyShutdown)
        {
            type->notifyShutdown();
        }
        Nucleus_Collections_PointerHashMap_Enumerator_next(&e);
    }
    Nucleus_Collections_PointerHashMap_Enumerator_uninitialize(&e);

    // (2) Uninitialize the dynamic libraries.
    Nucleus_Collections_PointerArray_uninitialize(&types->dynamicLibraries);
    
    // (3) Return success.
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NonNull() static Nucleus_Status
createClassType
    (
        Nucleus_Type **type,
        const char *name,
        Nucleus_Size objectSize,
        Nucleus_Status (*objectDestructor)(void *object),
        Nucleus_Size dispatchSize,
        Nucleus_Status (*dispatchConstructor)(void *dispatch),
        Nucleus_Type *parentType,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)()
    )
{
    Nucleus_Status status;
    Nucleus_Type *temporary;
    // Allocate the type.
    status = Nucleus_allocateMemory((void **)&temporary, sizeof(Nucleus_Type));
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    // Store the type name.
    temporary->name = strdup(name);
    if (Nucleus_Unlikely(!temporary->name))
    {
        Nucleus_deallocateMemory(temporary);
        return Nucleus_Status_AllocationFailed;
    }
    // Store the type name hash.
    Nucleus_hashMemory(name, strlen(name), &temporary->hashValue);
    // Store shutdown callback.
    temporary->notifyShutdown = notifyShutdown;
    // Store class type specific values.
    temporary->classType.objectSize = objectSize;
    temporary->classType.objectDestructor = objectDestructor;
    temporary->classType.dispatchSize = dispatchSize;
    temporary->classType.dispatchConstructor = dispatchConstructor;
    temporary->classType.parentType = parentType;
    temporary->classType.dispatch = NULL;  
    // Initialize the dispatch.
    status = Nucleus_allocateMemory((void **)&(temporary->classType.dispatch),
                                    temporary->classType.dispatchSize);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(temporary->name);
        temporary->name = NULL;
        Nucleus_deallocateMemory(temporary);
        return Nucleus_Status_AllocationFailed;
    }
    if (temporary->classType.parentType)
    {
        Nucleus_copyMemory((void *)temporary->classType.dispatch,
                           (void *)temporary->classType.parentType->classType.dispatch,
                           temporary->classType.parentType->classType.dispatchSize);
    }
    if (temporary->classType.dispatchConstructor)
    {
        temporary->classType.dispatchConstructor(temporary->classType.dispatch);
    }
    // Return result.
    *type = temporary;
    // Return success.
    return Nucleus_Status_Success;
}
 
Nucleus_NonNull() static Nucleus_Status
destroyClassType
    (
        Nucleus_Type *type
    )
{
    Nucleus_deallocateMemory(type->classType.dispatch);
    type->classType.dispatch = NULL;
    Nucleus_deallocateMemory(type->name);
    type->name = NULL;
    Nucleus_deallocateMemory(type);
    return Nucleus_Status_Success;
}
