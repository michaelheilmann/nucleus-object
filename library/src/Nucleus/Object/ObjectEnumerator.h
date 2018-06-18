// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/Exports.h"
#include "Nucleus/Object/Object.h"
#include "Nucleus/Object/Types.h"

Nucleus_ClassTypeDeclaration(Nucleus_Object_Library_Export,
                             "Nucleus.ObjectEnumerator",
                             Nucleus_ObjectEnumerator,
                             Nucleus_Object)
                             
#define NUCLEUS_OBJECTENUMERATOR(p) ((Nucleus_ObjectEnumerator *)(p))
#define NUCLEUS_OBJECTENUMERATOR_CLASS(p) ((Nucleus_ObjectEnumerator_Class *)(p))

struct Nucleus_ObjectEnumerator
{
    Nucleus_Object parent;
}; // struct Nucleus_ObjectEnumerator

struct Nucleus_ObjectEnumerator_Class
{
    Nucleus_Object_Class parent;
    Nucleus_Status (*hasObject)(Nucleus_ObjectEnumerator *self, Nucleus_Boolean *);
    Nucleus_Status (*nextObject)(Nucleus_ObjectEnumerator *self);
    Nucleus_Status (*getObject)(Nucleus_ObjectEnumerator *self, Nucleus_Object **);
}; // struct Nucleus_ObjectEnumerator_Class

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectEnumerator_construct
    (
        Nucleus_ObjectEnumerator *self
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectEnumerator_hasObject
    (
        Nucleus_ObjectEnumerator *self,
        Nucleus_Boolean *hasObject
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectEnumerator_nextObject
    (
        Nucleus_ObjectEnumerator *self
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectEnumerator_getObject
    (
        Nucleus_ObjectEnumerator *self,
        Nucleus_Object **object
    );
