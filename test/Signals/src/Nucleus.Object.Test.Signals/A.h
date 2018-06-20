// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/Object.h"
#include "Nucleus/Object/Types.h"
#include "Nucleus/Object/Signals.h"

#define Export
Nucleus_ClassTypeDeclaration(Export,
                             "A",
                             A,
                             Nucleus_Object)
#define A(p) ((A *)(p))
#define A_CLASS(p) ((A_Class *)(p))

struct A
{
    Nucleus_Object parent;
}; // struct A

struct A_Class
{
    Nucleus_Object_Class parent;
}; // struct A_Class

Nucleus_NonNull() Nucleus_Status
A_construct
    (
        A *self
    );
