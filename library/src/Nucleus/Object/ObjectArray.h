#pragma once

#include "Nucleus/Object/Exports.h"
#include "Nucleus/Object/Object.h"
#include "Nucleus/Object/Types.h"
#include "Nucleus/Object/ObjectEnumerator.h"

#include "Nucleus/Collections/PointerArray.h"

// Forward declaration.
typedef struct Nucleus_ObjectEnumerator Nucleus_ObjectEnumerator;

Nucleus_ClassTypeDeclaration(Nucleus_Object_Library_Export,
                             "Nucleus.ObjectArray",
                             Nucleus_ObjectArray,
                             Nucleus_Object)
                             
#define NUCLEUS_OBJECTARRAY(p) ((Nucleus_ObjectArray *)(p))
#define NUCLEUS_OBJECTARRAY_CLASS(p) ((Nucleus_ObjectArray_Class *)(p))

struct Nucleus_ObjectArray
{
    Nucleus_Object parent;
    Nucleus_Collections_PointerArray implementation;
}; // struct Nucleus_ObjectArray

struct Nucleus_ObjectArray_Class
{
    Nucleus_Object_Class parent;
}; // struct Nucleus_ObjectArray_Class

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_construct
    (
        Nucleus_ObjectArray *self
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_create
    (
        Nucleus_ObjectArray **objectArray
    );

// https://github.com/primordialmachine/nucleus-object/blob/master/documentation/Nucleus_ObjectArray_increaseCapacity.md
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_increaseCapacity
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size requiredAdditionalCapacity
    );

// https://github.com/primordialmachine/nucleus-object/blob/master/documentation/Nucleus_ObjectArray_ensureFreeCapacity.md
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_ensureFreeCapacity
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size requiredFreeCapacity
    );

// https://github.com/primordialmachine/nucleus-object/blob/master/documentation/Nucleus_ObjectArray_prepend.md
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_prepend
    (
        Nucleus_ObjectArray *self,
        Nucleus_Object *object
    );

// https://github.com/primordialmachine/nucleus-object/blob/master/documentation/Nucleus_ObjectArray_append.md
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_append
    (
        Nucleus_ObjectArray *self,
        Nucleus_Object *object
    );

// https://github.com/primordialmachine/nucleus-object/blob/master/documentation/Nucleus_ObjectArray_insert.md
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_insert
    (
        Nucleus_ObjectArray *self,
        Nucleus_Object *object,
        Nucleus_Size index
    );

// https://github.com/primordialmachine/nucleus-object/blob/master/documentation/Nucleus_ObjectArray_at.md
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_at
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size index,
        Nucleus_Object **object
    );

// https://github.com/primordialmachine/nucleus-object/blob/master/documentation/Nucleus_ObjectArray_getSize.md
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_getSize
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size *size
    );

// https://github.com/primordialmachine/nucleus-object/blob/master/documentation/Nucleus_ObjectArray_getCapacity.md
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_getCapacity
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size *capacity
    );

// https://github.com/primordialmachine/nucleus/blob/master/documentation/Nucleus_ObjectArray_getFreeCapacity.md
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_getFreeCapacity
    (
        Nucleus_ObjectArray *self,
        Nucleus_Size *freeCapacity
    );

// https://github.com/primordialmachine/nucleus-object/blob/master/documentation/Nucleus_ObjectArray_clear.md
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_clear
    (
        Nucleus_ObjectArray *self
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArray_getEnumerator
    (
        Nucleus_ObjectArray *self,
        Nucleus_ObjectEnumerator **enumerator
    );
