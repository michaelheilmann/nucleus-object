// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/Types.h"
#include "Nucleus/Memory.h"
#include "Nucleus/Collections/PointerArray.h"
#include "Nucleus/Collections/PointerHashMap.h"
#include "Nucleus/Hash/Memory.h"
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


// `Nucleus_TypeSystem`
typedef struct Nucleus_TypeSystem Nucleus_TypeSystem;

// The mutex implementation.
#define MUTEX int
#define MUTEX_INITIALIZER (0)
#define MUTEX_LOCK(mutex) /* - */
#define MUTEX_UNLOCK(mutex) /* - */

static MUTEX g_mutexSingleton = MUTEX_INITIALIZER;
static Nucleus_TypeSystem *g_singleton = NULL;

struct Nucleus_TypeSystem
{
    int referenceCount;
    Nucleus_Collections_PointerHashMap types;
    Nucleus_Collections_PointerArray dynamicLibraries;
}; // struct Nucleus_TypeSystem

Nucleus_NonNull() static Nucleus_Status
initialize
    (
        Nucleus_TypeSystem *typeSystem
    );

Nucleus_NonNull() static Nucleus_Status
uninitialize
    (
        Nucleus_TypeSystem *typeSystem
    );

Nucleus_NonNull() static Nucleus_Status
create
    (
        Nucleus_TypeSystem **typeSystem
    );

Nucleus_NonNull() static Nucleus_Status
destroy
    (
        Nucleus_TypeSystem *typeSystem
    );

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
    );
 
Nucleus_NonNull() static Nucleus_Status
destroyClassType
    (
        Nucleus_Type *type
    );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/