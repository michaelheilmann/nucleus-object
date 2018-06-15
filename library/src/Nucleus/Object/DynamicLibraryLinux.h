// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Configuration.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_LINUX)  || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_CYGWIN) || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_MACOS)

#include "Nucleus/Object/DynamicLibrary.h"

Nucleus_ClassTypeDeclaration(Nucleus_Object_Library_Export,
                             "Nucleus.DynamicLibraryLinux",
                             Nucleus_DynamicLibraryLinux,
                             Nucleus_DynamicLibrary)
#define NUCLEUS_DYNAMICLIBRARYLINUX(p) ((Nucleus_DynamicLibraryLinux *)(p))
#define NUCLEUS_DYNAMICLIBRARYLINUX_CLASS(p) ((Nucleus_DynamicLibraryLinux_Class *)(p))

struct Nucleus_DynamicLibraryLinux_Class
{
    Nucleus_DynamicLibrary_Class parent;
}; // struct Nucleus_DynamicLibraryLinux_Class

struct Nucleus_DynamicLibraryLinux
{
    Nucleus_DynamicLibrary parent;
    void *handle;
}; // struct Nucleus_DynamicLibraryLinux

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryLinux_create
    (
        Nucleus_DynamicLibraryLinux **self,
        const char *pathname
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryLinux_construct
    (
        Nucleus_DynamicLibraryLinux *self,
        const char *pathname
    );

Nucleus_NonNull() void
Nucleus_DynamicLibraryLinux_destruct
    (
        Nucleus_DynamicLibraryLinux *self
    );

#endif
