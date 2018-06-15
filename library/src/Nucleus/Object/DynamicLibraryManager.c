// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/DynamicLibraryManager.h"

#include "Nucleus/Object/DynamicLibrary.h"
#include "Nucleus/Collections/PointerArray.h"
#include "Nucleus/Memory.h"
#include <string.h>

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_LINUX)  || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_CYGWIN) || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_MACOS)
    #include "Nucleus/Object/DynamicLibraryLinux.h"
#elif (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS)
    #include "Nucleus/Object/DynamicLibraryWindows.h"
#else
    #error("operating system not supported")
#endif

struct Nucleus_DynamicLibraryManager_Class
{
    Nucleus_Object_Class parent;
}; // struct Nucleus_DynamicLibraryManager_Class

struct Nucleus_DynamicLibraryManager
{
    Nucleus_Object parent;
    Nucleus_Collections_PointerArray dynamicLibraries;
}; // struct Nucleus_DynamicLibraryManager

Nucleus_ClassTypeDefinition(Nucleus_Object_Library_Export,
                            "Nucleus.DynamicLibraryManager",
                            Nucleus_DynamicLibraryManager,
                            Nucleus_Object)

static Nucleus_DynamicLibraryManager *g_singleton = NULL;

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_DynamicLibraryManager_Class *dispatch
    )
{ return Nucleus_Status_Success; }

Nucleus_NonNull() static Nucleus_Status
create
    (
        Nucleus_DynamicLibraryManager **dynamicLibraryManager
    );

Nucleus_NonNull() static Nucleus_Status
construct
    (
        Nucleus_DynamicLibraryManager *self
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    Nucleus_Type *type;
    Nucleus_Status status;
    status = Nucleus_DynamicLibraryManager_getType(&type);
    if (Nucleus_Unlikely(status)) return status;
    Nucleus_Object_construct(NUCLEUS_OBJECT(self));
    status = Nucleus_Collections_PointerArray_initialize(&self->dynamicLibraries,
                                                         8,
                                                         NUCLEUS_LOCKFUNCTION(&Nucleus_Object_incrementReferenceCount),
                                                         NUCLEUS_UNLOCKFUNCTION(&Nucleus_Object_decrementReferenceCount));
    if (Nucleus_Unlikely(status)) return status;
    NUCLEUS_OBJECT(self)->type = type;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
destruct
    (
        Nucleus_DynamicLibraryManager *self
    )
{
    Nucleus_Collections_PointerArray_uninitialize(&self->dynamicLibraries);
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
create
    (
        Nucleus_DynamicLibraryManager **dynamicLibraryManager
    )
{ 
    if (Nucleus_Unlikely(!dynamicLibraryManager)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    Nucleus_DynamicLibraryManager *temporary;
    status = Nucleus_Object_allocate((Nucleus_Object **)&temporary,
                                     sizeof(Nucleus_DynamicLibraryManager));
    if (Nucleus_Unlikely(status)) return status;
    status = construct(temporary);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(temporary));
        return status;
    }
    *dynamicLibraryManager = temporary;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryManager_create
    (
        Nucleus_DynamicLibraryManager **dynamicLibraryManager
    )
{
    if (Nucleus_Unlikely(!g_singleton))
    {
        Nucleus_Status status = create(&g_singleton);
        if (Nucleus_Unlikely(status)) return status;
        *dynamicLibraryManager = g_singleton;
    }
    else
    {
        Nucleus_Object_incrementReferenceCount(NUCLEUS_OBJECT(g_singleton));
        *dynamicLibraryManager = g_singleton;
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryManager_load
    (
        Nucleus_DynamicLibraryManager *dynamicLibraryManager,
        const char *pathname,
        Nucleus_DynamicLibrary **dynamicLibrary
    )
{
    if (Nucleus_Unlikely(!dynamicLibraryManager || !pathname || !dynamicLibrary)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    Nucleus_Size i = 0, n;
    status = Nucleus_Collections_PointerArray_getSize(&dynamicLibraryManager->dynamicLibraries, &n);
    if (Nucleus_Unlikely(status)) return status;
    for (; i < n; ++i)
    {
        Nucleus_DynamicLibrary *temporary;
        status = Nucleus_Collections_PointerArray_at(&dynamicLibraryManager->dynamicLibraries, i, (void **)&temporary);
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
    status = Nucleus_Collections_PointerArray_append(&dynamicLibraryManager->dynamicLibraries, temporary);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(temporary));
        return status;
    }
    *dynamicLibrary = temporary;
    return Nucleus_Status_Success;
}

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryManager_unloadAll
    (
        Nucleus_DynamicLibraryManager *dynamicLibraryManager
    )
{ return Nucleus_Collections_PointerArray_clear(&dynamicLibraryManager->dynamicLibraries); }

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryManager_getSize
    (
        Nucleus_DynamicLibraryManager *dynamicLibraryManager,
        Nucleus_Size *size
    )
{
    if (Nucleus_Unlikely(!dynamicLibraryManager)) return Nucleus_Status_InvalidArgument;
    return Nucleus_Collections_PointerArray_getSize(&dynamicLibraryManager->dynamicLibraries, size);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryManager_at
    (
        Nucleus_DynamicLibraryManager *dynamicLibraryManager,
        Nucleus_Size index,
        Nucleus_DynamicLibrary **dynamicLibrary
    )
{
    if (Nucleus_Unlikely(!dynamicLibraryManager)) return Nucleus_Status_InvalidArgument;
    return Nucleus_Collections_PointerArray_at(&dynamicLibraryManager->dynamicLibraries, index, (void **)dynamicLibrary);
}
