// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/TypeSystem.h"
#include "Nucleus/Memory.h"
#include "Nucleus/Collections/PointerHashMap.h"
#include "Nucleus/Hash/Memory.h"
#include <string.h>

struct Nucleus_TypeSystem
{
    int referenceCount;
    Nucleus_Collections_PointerHashMap types;
}; // struct Nucleus_TypeSystem

static Nucleus_TypeSystem *g_singleton = NULL;

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
