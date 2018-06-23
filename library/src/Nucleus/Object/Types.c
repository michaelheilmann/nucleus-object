// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Types-private.c.i"

DEFINE_MODULE(Nucleus_Types)

Nucleus_NonNull() Nucleus_Status
Nucleus_Type_hash
    (
        Nucleus_Type *type,
        Nucleus_HashValue *hashValue
    )
{
    if (Nucleus_Unlikely(!type || !hashValue)) return Nucleus_Status_InvalidArgument;
    *hashValue = type->hashValue;
    return Nucleus_Status_Success;
}

#if defined(Nucleus_WithClassTypes) && 1 == Nucleus_WithClassTypes

Nucleus_NonNull() Nucleus_Status
Nucleus_Types_addClassType
    (
        Nucleus_Type **type,
        const char *name,
        Nucleus_Size objectSize,
        Nucleus_Status (*objectDestructor)(void *object),
        Nucleus_Size dispatchSize,
        Nucleus_Status (*dispatchConstructor)(void *dispatch),
        Nucleus_Type *parentType,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)(Nucleus_Type *)
    )
{
    Nucleus_Types_Mutex_lock(&g_singletonMutex);
    if (!g_singleton)
    {
        Nucleus_Types_Mutex_unlock(&g_singletonMutex);
        return Nucleus_Status_NotInitialized;
    }

    Nucleus_Status status;
    Nucleus_Type *type1;
    
    // Assert no type of the same name exists.
    status = Nucleus_Collections_PointerHashMap_get(&g_singleton->types,
                                                    (void *)name,
                                                    (void **)&type1);
    if (Nucleus_Unlikely(status && status != Nucleus_Status_NotExists))
    {
        Nucleus_Types_Mutex_unlock(&g_singletonMutex);
        return status;
    }
    // Create the type.
    status = createClassType(&type1, name, objectSize, objectDestructor,
                                           dispatchSize, dispatchConstructor,
                                           parentType, notifyShutdown);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Types_Mutex_unlock(&g_singletonMutex);
        return status;
    }
    // Registers the finalization hook.
    status = Nucleus_FinalizationHooks_add(type1, NUCLEUS_CALLBACK(notifyShutdown));
    if (Nucleus_Unlikely(status))
    {
        destroyClassType(type1);
        return status;
    }
    // Add the type.
    status = Nucleus_Collections_PointerHashMap_set(&g_singleton->types, type1->name, type1,
                                                    Nucleus_Boolean_False); 
    if (Nucleus_Unlikely(status))
    {
        Nucleus_FinalizationHooks_removeAll(type);
        destroyClassType(type1);
        Nucleus_Types_Mutex_unlock(&g_singletonMutex);
        return status;       
    }

    // Return the type.
    *type = type1;

    Nucleus_Types_Mutex_unlock(&g_singletonMutex);

    // Return success.
    return Nucleus_Status_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NonNull() Nucleus_Status
Nucleus_Types_loadDynamicLibrary
    (
        const char *pathname,
        Nucleus_DynamicLibrary **dynamicLibrary
    )
{
    if (Nucleus_Unlikely(!pathname || !dynamicLibrary)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    Nucleus_Size i = 0, n;
    status = Nucleus_Collections_PointerArray_getSize(&g_singleton->dynamicLibraries, &n);
    if (Nucleus_Unlikely(status)) return status;
    for (; i < n; ++i)
    {
        Nucleus_DynamicLibrary *temporary;
        status = Nucleus_Collections_PointerArray_at(&g_singleton->dynamicLibraries, i, (void **)&temporary);
        if (Nucleus_Unlikely(status)) return status;
        const char *temporaryPathname;
        status = Nucleus_DynamicLibrary_getName(temporary, &temporaryPathname);
        if (Nucleus_Unlikely(status)) return status;
        if (!strcmp(temporaryPathname, pathname))
        {
            *dynamicLibrary = temporary;
            return Nucleus_Status_Success;
        }
    }
    Nucleus_DynamicLibrary *temporary;
#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_LINUX)  || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_CYGWIN) || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_MACOS)
    status = Nucleus_DynamicLibraryLinux_create((Nucleus_DynamicLibraryLinux **)&temporary, pathname);
#elif (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS)
    status = Nucleus_DynamicLibraryWindows_create((Nucleus_DynamicLibraryWindows **)&temporary, pathname);
#else
    #error("operating system not supported")
#endif
    if (Nucleus_Unlikely(status)) return status;
    status = Nucleus_DynamicLibrary_load(temporary);
    if (Nucleus_Unlikely(status)) return status;
    status = Nucleus_Collections_PointerArray_append(&g_singleton->dynamicLibraries, temporary);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_DynamicLibrary_unlock(temporary);
        return status;
    }
    *dynamicLibrary = temporary;
    return Nucleus_Status_Success;
}

Nucleus_Status
Nucleus_Types_unloadAllDynamicLibraries
    (
    )
{ return Nucleus_Collections_PointerArray_clear(&g_singleton->dynamicLibraries); }

Nucleus_NonNull() Nucleus_Status
Nucleus_Types_getNumberOfDynamicLibraries
    (
        Nucleus_Size *size
    )
{ return Nucleus_Collections_PointerArray_getSize(&g_singleton->dynamicLibraries, size); }

Nucleus_NonNull() Nucleus_Status
Nucleus_Types_getDynamicLibrary
    (
        Nucleus_Size index,
        Nucleus_DynamicLibrary **dynamicLibrary
    )
{
    return Nucleus_Collections_PointerArray_at(&g_singleton->dynamicLibraries, index,
                                               (void **)dynamicLibrary);
}

#endif
