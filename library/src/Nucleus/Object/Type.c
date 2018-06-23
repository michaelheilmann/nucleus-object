#include "Nucleus/Object/Type.h"

#include "Nucleus/Hash.h"
#include "Nucleus/Memory.h"
#include <string.h>

Nucleus_NonNull() Nucleus_Status
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
    // Store the kind of the type.
    temporary->kind = Nucleus_TypeKind_Class;
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
 
Nucleus_NonNull() Nucleus_Status
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
