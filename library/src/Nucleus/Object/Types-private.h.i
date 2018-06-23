// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/Types.h"
#include "Nucleus/Object/FinalizationHooks.h"
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

#define Nucleus_Types_Mutex int
#define Nucleus_Types_Mutex_Initializer (0)
#define Nucleus_Types_Mutex_lock(mutex) /* - */
#define Nucleus_Types_Mutex_unlock(mutex) /* - */
DECLARE_MODULE_PRIVATE(Nucleus_Types)

struct Nucleus_Types
{
    int referenceCount;
    Nucleus_Collections_PointerHashMap types;
    Nucleus_Collections_PointerArray dynamicLibraries;
}; // struct Nucleus_Types

Nucleus_NonNull() static Nucleus_Status
lockTypeName
    (
        char *typeName
    );

Nucleus_NonNull() static Nucleus_Status
unlockTypeName
    (
        char *typeName
    );

Nucleus_NonNull() static Nucleus_Status
lockType
    (
        Nucleus_Type *p
    );

Nucleus_NonNull() static Nucleus_Status
unlockType
    (
        Nucleus_Type *p
    );

Nucleus_NonNull() static Nucleus_Status
hashTypeName
    (
        const char *p,
        Nucleus_HashValue *hashValue
    );

Nucleus_NonNull() static Nucleus_Status
equalToTypeName
    (
        const char *p,
        const char *q,
        Nucleus_Boolean *equalTo
    );
