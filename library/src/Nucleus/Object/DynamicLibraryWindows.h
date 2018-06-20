// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Configuration.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS)

#include "Nucleus/Object/DynamicLibrary.h"
#include "Nucleus/IncludesWindows.h"

typedef struct Nucleus_DynamicLibraryWindows Nucleus_DynamicLibraryWindows;
#define NUCLEUS_DYNAMICLIBRARYWINDOWS(p) ((Nucleus_DynamicLibraryWindows *)(p))

struct Nucleus_DynamicLibraryWindows
{
    Nucleus_DynamicLibrary parent;
    HINSTANCE handle;
}; // struct Nucleus_DynamicLibraryWindows

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryWindows_construct
    (
        Nucleus_DynamicLibraryWindows *self,
        const char *pathname
    );

Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryWindows_destruct
    (
        Nucleus_DynamicLibraryWindows *self
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryWindows_create
    (
        Nucleus_DynamicLibraryWindows **self,
        const char *pathname
    );

#endif
