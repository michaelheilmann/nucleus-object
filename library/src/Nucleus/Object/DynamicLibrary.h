// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/Object.h"
#include "Nucleus/Object/Types.h"

typedef struct Nucleus_DynamicLibrary Nucleus_DynamicLibrary;
#define NUCLEUS_DYNAMICLIBRARY(p) ((Nucleus_DynamicLibrary *)(p))

struct Nucleus_DynamicLibrary
{
    int referenceCount;//Nucleus_Object parent;
    char *pathname;
    Nucleus_Status (*load)(Nucleus_DynamicLibrary *self);
    Nucleus_Status (*getSymbol)(Nucleus_DynamicLibrary *self,
                                const char *symbolName,
                                void **symbol);
    Nucleus_AlwaysSucceed() Nucleus_Status (*destruct)(Nucleus_DynamicLibrary *self);
}; // struct Nucleus_DynamicLibrary

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_construct
    (
        Nucleus_DynamicLibrary *dynamicLibrary,
        const char *pathname
    );
 
Nucleus_AlwaysSucceed() Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_destruct
    (
        Nucleus_DynamicLibrary *dynamicLibrary
    );

/// *pathname is assigned a pointer the name.
/// The name has the same lifetime as the dynamic library object.
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_getName
    (
        Nucleus_DynamicLibrary *dynamicLibrary,
        const char **pathname
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_load
    (
        Nucleus_DynamicLibrary *dynamicLibrary
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_getSymbol
    (
        Nucleus_DynamicLibrary *self,
        const char *symbolName,
        void **symbol
    );
   
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_lock
    (
        Nucleus_DynamicLibrary *self
    );
    
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_unlock
    (
        Nucleus_DynamicLibrary *self
    );
