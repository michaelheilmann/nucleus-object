// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/TypeSystem-private.c.i"

Nucleus_NonNull() Nucleus_Status
Nucleus_TypeSystem_get
   (
      Nucleus_TypeSystem **typeSystem
   )
{
   if (Nucleus_Unlikely(!typeSystem))
   { return Nucleus_Status_InvalidArgument; }
   // lock
   if (Nucleus_Unlikely(!g_singleton))
   {
       // unlock
       return Nucleus_Status_NotExists;
   }
   *typeSystem = g_singleton;
   // unlock
   return Nucleus_Status_Success;
}

Nucleus_Status
Nucleus_TypeSystem_startup
    (
    )
{
    if (NULL != g_singleton)
    {
        g_singleton->referenceCount++;
        return Nucleus_Status_Success;
    }
    else
    {
        return create(&g_singleton);
    }
}

Nucleus_Status
Nucleus_TypeSystem_shutdown
    (
    )
{
    if (NULL == g_singleton)
    {
        return Nucleus_Status_NotExists;
    }
    if (0 == --g_singleton->referenceCount)
    {
        destroy(g_singleton);
        g_singleton = NULL;
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_TypeSystem_registerClass
    (
        Nucleus_TypeSystem *typeSystem,
        Nucleus_Type **type,
        const char *name,
        Nucleus_Size objectSize,
        Nucleus_Status (*objectDestructor)(void *object),
        Nucleus_Size dispatchSize,
        Nucleus_Status (*dispatchConstructor)(void *dispatch),
        Nucleus_Type *parentType,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdownContext)()
    )
{
    Nucleus_Status status;
    Nucleus_Type *type1;
    
    // Assert no type of the same name exists.
    status = Nucleus_Collections_PointerHashMap_get(&typeSystem->types,
                                                    (void *)name,
                                                    (void **)&type1);
    if (Nucleus_Unlikely(status && status != Nucleus_Status_NotExists)) return status;

    // Create the type.
    status = Nucleus_allocateMemory((void **)&type1, sizeof(Nucleus_Type));
    if (Nucleus_Unlikely(status)) return status;
    type1->name = strdup(name);
    if (Nucleus_Unlikely(!type1->name))
    {
        Nucleus_deallocateMemory(type1);
        return Nucleus_Status_AllocationFailed;
    }
    type1->objectSize = objectSize;
    type1->objectDestructor = objectDestructor;
    type1->dispatchSize = dispatchSize;
    type1->dispatchConstructor = dispatchConstructor;
    type1->parentType = parentType;
    type1->dispatch = NULL;
    type1->notifyShutdownContext = notifyShutdownContext;
    
    // Initialize the dispatch.
    status = Nucleus_allocateMemory((void **)&(type1->dispatch), type1->dispatchSize);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(type1->name);
        type1->name = NULL;
        Nucleus_deallocateMemory(type1);
        return Nucleus_Status_AllocationFailed;
    }
    if (type1->parentType)
    {
        Nucleus_copyMemory((void *)type1->dispatch,
                           (void *)type1->parentType->dispatch,
                           type1->parentType->dispatchSize);
    }
    if (type1->dispatchConstructor)
    {
        type1->dispatchConstructor(type1->dispatch);
    }

    // Add the type.
    status = Nucleus_Collections_PointerHashMap_set(&typeSystem->types, type1->name, type1,
                                                    Nucleus_Boolean_False); 
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(type1->dispatch);
        Nucleus_deallocateMemory(type1->name);
        Nucleus_deallocateMemory(type1);
        return status;       
    }

    // Return the type.
    *type = type1;

    // Return success.
    return Nucleus_Status_Success;
}
