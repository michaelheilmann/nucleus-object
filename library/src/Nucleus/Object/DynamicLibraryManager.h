// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/DynamicLibrary.h"

Nucleus_ClassTypeDeclaration(Nucleus_Object_Library_Export,
                             "Nucleus.DynamicLibraryManager",
                             Nucleus_DynamicLibraryManager,
                             Nucleus_Object)
#define NUCLEUS_DYNAMICLIBRARYMANAGER(p) ((Nucleus_DynamicLibraryManager *)(p))
#define NUCLEUS_DYNAMICLIBRARYMANAGER_CLASS(p) ((Nucleus_DynamicLibraryManager_Class *)(p))

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryManager_create
    (
        Nucleus_DynamicLibraryManager **dynamicLibraryManager
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryManager_load
    (
        Nucleus_DynamicLibraryManager *dynamicLibraryManager,
        const char *pathname,
        Nucleus_DynamicLibrary **dynamicLibrary
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryManager_unloadAll
    (
        Nucleus_DynamicLibraryManager *dynamicLibraryManager
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryManager_getSize
    (
        Nucleus_DynamicLibraryManager *dynamicLibraryManager,
        Nucleus_Size *size
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryManager_at
    (
        Nucleus_DynamicLibraryManager *dynamicLibraryManager,
        Nucleus_Size index,
        Nucleus_DynamicLibrary **dynamicLibrary
    );
