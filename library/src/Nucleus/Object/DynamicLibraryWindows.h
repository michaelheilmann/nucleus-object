// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Configuration.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS)

#include "Nucleus/Object/DynamicLibrary.h"
#include "Nucleus/IncludesWindows.h"

Nucleus_ClassTypeDeclaration(Nucleus_Object_Library_Export,
                             "Nucleus.DynamicLibraryWindows",
                             Nucleus_DynamicLibraryWindows,
                             Nucleus_DynamicLibary)
#define NUCLEUS_DYNAMICLIBRARYWINDOWS(p) ((Nucleus_DynamicLibraryWindows *)(p))
#define NUCLEUS_DYNAMICLIBRARYWINDOWS_CLASS(p) ((Nucleus_DynamicLibraryWindows_Class *)(p))

struct Nucleus_DynamicLibraryWindows_Class
{
    Nucleus_DynamicLibrary_Class parent;
}; // struct Nucleus_DynamicLibraryWindows_Class

struct Nucleus_DynamicLibraryWindows
{
    Nucleus_DynamicLibrary parent;
    HINSTANCE handle;
}; // struct Nucleus_DynamicLibraryWindows

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryWindows_create
    (
        Nucleus_DynamicLibraryWindows **self,
        const char *pathname
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryWindows_construct
    (
        Nucleus_DynamicLibraryWindows *self,
        const char *pathname
    );

Nucleus_NonNull() void
Nucleus_DynamicLibraryWindows_destruct
    (
        Nucleus_DynamicLibraryWindows *self
    );

#endif
