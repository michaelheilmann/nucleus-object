// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/Object.h"
#include "Nucleus/Object/TypeSystem.h"

Nucleus_ClassTypeDeclaration(Nucleus_Object_Library_Export,
                             "Nucleus.DynamicLibrary",
                             Nucleus_DynamicLibrary,
                             Nucleus_Object)
#define NUCLEUS_DYNAMICLIBRARY(p) ((Nucleus_DynamicLibrary *)(p))
#define NUCLEUS_DYNAMICLIBRARY_CLASS(p) ((Nucleus_DynamicLibrary_Class *)(p))

struct Nucleus_DynamicLibrary_Class
{
    Nucleus_Object_Class parent;
    Nucleus_Status (*load)(Nucleus_DynamicLibrary *self);
    Nucleus_Status (*getSymbol)(Nucleus_DynamicLibrary *self,
                                const char *symbolName,
                                void **symbol);
}; // struct Nucleus_DynamicLibrary_Class

struct Nucleus_DynamicLibrary
{
    Nucleus_Object parent;
    char *pathname;
}; // struct Nucleus_DynamicLibrary

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_construct
    (
        Nucleus_DynamicLibrary *dynamicLibrary,
        const char *pathname
    );

/// *pathname is assigned a pointer the name.
/// The name has the same lifetime as the dynamic library object.
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_getName
    (
        Nucleus_DynamicLibrary *dynamicLibrary,
        const char **pathname
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_load
    (
        Nucleus_DynamicLibrary *dynamicLibrary
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_getSymbol
    (
        Nucleus_DynamicLibrary *self,
        const char *symbolName,
        void **symbol
    );
