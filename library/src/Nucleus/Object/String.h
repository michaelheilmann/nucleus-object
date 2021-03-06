// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/Object.h"
#include "Nucleus/Object/Types.h"
#include "Nucleus/Types/Boolean.h"
#include "Nucleus/Types/HashValue.h"

Nucleus_ClassTypeDeclaration(Nucleus_Object_Library_Export,
                             "Nucleus.String",
                             Nucleus_String,
                             Nucleus_Object)
#define NUCLEUS_STRING(p) ((Nucleus_String *)(p))
#define NUCLEUS_STRING_CLASS(p) ((Nucleus_String_Class *)(p))

struct Nucleus_String
{
    Nucleus_Object parent;
    char *bytes;
}; // struct Nucleus_String

struct Nucleus_String_Class
{
    Nucleus_Object_Class parent;
}; // struct Nucleus_String_Class

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_String_construct
    (
        Nucleus_String *self,
        const char *bytes
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_String_create
    (
        Nucleus_String **string,
        const char *bytes
    );
