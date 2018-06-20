// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Configuration.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_LINUX)  || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_CYGWIN) || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_MACOS)

#include "Nucleus/Object/DynamicLibrary.h"

typedef struct Nucleus_DynamicLibraryLinux Nucleus_DynamicLibraryLinux;
#define NUCLEUS_DYNAMICLIBRARYLINUX(p) ((Nucleus_DynamicLibraryLinux *)(p))

struct Nucleus_DynamicLibraryLinux
{
    Nucleus_DynamicLibrary parent;
    void *handle;
}; // struct Nucleus_DynamicLibraryLinux

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryLinux_construct
    (
        Nucleus_DynamicLibraryLinux *self,
        const char *pathname
    );

Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryLinux_destruct
    (
        Nucleus_DynamicLibraryLinux *self
    );


Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryLinux_create
    (
        Nucleus_DynamicLibraryLinux **self,
        const char *pathname
    );

#endif
