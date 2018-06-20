#pragma once

#include "Nucleus/Object/ObjectArray.h"

Nucleus_ClassTypeDeclaration(Nucleus_Object_Library_Export,
                             "Nucleus.ObjectArrayEnumerator",
                             Nucleus_ObjectArrayEnumerator,
                             Nucleus_Object)
                             
#define NUCLEUS_OBJECTARRAYENUMERATOR(p) ((Nucleus_ObjectArrayEnumerator *)(p))
#define NUCLEUS_OBJECTARRAYENUMERATOR_CLASS(p) ((Nucleus_ObjectArrayEnumerator_Class *)(p))

struct Nucleus_ObjectArrayEnumerator
{
    Nucleus_Object parent;
    Nucleus_Collections_PointerArray_Enumerator implementation;
}; // struct Nucleus_ObjectArray

struct Nucleus_ObjectArrayEnumerator_Class
{
    Nucleus_Object_Class parent;
}; // struct Nucleus_ObjectArrayEnumerator_Class

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArrayEnumerator_construct
    (
        Nucleus_ObjectArrayEnumerator *self,
        Nucleus_ObjectArray *objectArray
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArrayEnumerator_create
    (
        Nucleus_ObjectArrayEnumerator **objectArrayEnumerator,
        Nucleus_ObjectArray *objectArray
    );