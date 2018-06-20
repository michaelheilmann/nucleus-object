// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Types/HashValue.h"
#include "Nucleus/Types/Size.h"
#include "Nucleus/Object/Exports.h"

// Class types are supported if this is defined and 1.
#define Nucleus_WithClassTypes (1)


// `Nucleus_DynamicLibrary`
typedef struct Nucleus_DynamicLibrary Nucleus_DynamicLibrary;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// `Nucleus_Type`
typedef struct Nucleus_Type Nucleus_Type;

typedef struct ClassType ClassType;

struct ClassType
{
    Nucleus_Size objectSize;
    Nucleus_Status (*objectDestructor)(void *object);
    
    Nucleus_Size dispatchSize;
    Nucleus_Status (*dispatchConstructor)(void *dispatch);
    
    Nucleus_Type *parentType;
    
    void *dispatch;
    
}; // struct ClassType

struct Nucleus_Type
{
    char *name;
    Nucleus_HashValue hashValue;
    Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)();
    ClassType classType;
}; // struct Nucleus_Type

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NonNull() Nucleus_Status
Nucleus_Type_hash
    (
        Nucleus_Type *type,
        Nucleus_HashValue *hashValue
    );

// # `Nucleus_Types_initialize`
// *Startup the `Nucleus_Types` singleton.*
// ## C Signature
// ```
// Nucleus_Status
// Nucleus_Types_initialize
//     (
//     );
// ```
// ## Description
// If the `Nucleus_Types` singleton does exist, `Nucleus_Status_Exists` is returned.
// If it does not exist, then it is created.
// If the creation fails, a non-zero status code is returned.
Nucleus_Object_Library_Export Nucleus_Status
Nucleus_Types_initialize
    (
    );

// # `Nucleus_Types_uninitialize`
// *Shutdown the `Nucleus_Types` singleton.*
// ## C Signature
// ```
// Nucleus_Status
// Nucleus_Types_uninitialize
//     (
//     );
// ```
// ## Description
// If the `Nucleus_Types` singleton does not exist, `Nucleus_Status_NotExists` is returned.
// If it does exist, it is destroyed.
Nucleus_Object_Library_Export Nucleus_Status
Nucleus_Types_uninitialize
    (
    );

#if defined(Nucleus_WithClassTypes) && 1 == Nucleus_WithClassTypes

// Add a class type.
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Types_addClassType
    (
        Nucleus_Type **type,
        const char *name,
        Nucleus_Size objectSize,
        Nucleus_Status(*objectDestructor)(void *object),
        Nucleus_Size classSize,
        Nucleus_Status(*dispatchConstructor)(void *dispatch),
        Nucleus_Type *parentType,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)()
    );

#endif

// name the type name
// cxxName the C type name
// cxxParentName The C parent type anem
#define Nucleus_ClassTypeDeclaration(Visibility, Name, NameCxx, ParentNameCxx) \
    typedef struct NameCxx NameCxx; \
    typedef struct NameCxx##_Class NameCxx##_Class; \
    Visibility Nucleus_NonNull() Nucleus_Status \
    NameCxx##_getType \
        ( \
            Nucleus_Type **type \
        );

#define Nucleus_ClassTypeDefinition(Visibility, Name, NameCxx, ParentNameCxx) \
    static Nucleus_Type *g_type = NULL; \
\
    Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status \
    constructDispatch \
        ( \
            NameCxx##_Class *dispatch \
        ); \
\
    Nucleus_AlwaysSucceed() static Nucleus_Status \
    notifyShutdown \
        ( \
        ); \
\
    Nucleus_AlwaysSucceed() static Nucleus_Status \
    destruct \
        ( \
            NameCxx *self \
        ); \
\
    Nucleus_AlwaysSucceed() static Nucleus_Status \
    notifyShutdown \
        ( \
        ) \
    { \
        g_type = NULL; \
        return Nucleus_Status_Success; \
    } \
\
    Visibility Nucleus_NonNull() Nucleus_Status \
    NameCxx##_getType \
        ( \
            Nucleus_Type **type \
        ) \
    { \
        if (Nucleus_Unlikely(!type)) return Nucleus_Status_InvalidArgument; \
        if (Nucleus_Unlikely(!g_type)) \
        { \
            Nucleus_Status status; \
            Nucleus_Type *parentType; \
            status = ParentNameCxx##_getType(&parentType); \
            if (Nucleus_Unlikely(status)) return status; \
            status = Nucleus_Types_addClassType(&g_type, \
                                                Name, \
                                                sizeof(NameCxx), \
                                                (Nucleus_Status (*)(void *))&destruct, \
                                                sizeof(NameCxx##_Class), \
                                                (Nucleus_Status (*)(void *))&constructDispatch, \
                                                parentType, \
                                                &notifyShutdown); \
            if (Nucleus_Unlikely(status)) return status; \
        } \
        *type = g_type; \
        return Nucleus_Status_Success; \
    }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Types_loadDynamicLibrary
    (
        const char *pathname,
        Nucleus_DynamicLibrary **dynamicLibrary
    );

Nucleus_Object_Library_Export Nucleus_Status
Nucleus_Types_unloadAllDynamicLibraries
    (
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Types_getNumberOfDynamicLibraries
    (
        Nucleus_Size *size
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Types_getDynamicLibrary
    (
        Nucleus_Size index,
        Nucleus_DynamicLibrary **dynamicLibrary
    );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
