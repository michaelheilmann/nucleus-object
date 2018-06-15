// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Types/Size.h"
#include "Nucleus/Object/Exports.h"

// `Nucleus_Type`
typedef struct Nucleus_Type Nucleus_Type;

struct Nucleus_Type
{
    char *name;
    
    Nucleus_Size objectSize;
    Nucleus_Status (*objectDestructor)(void *object);
    
    Nucleus_Size dispatchSize;
    Nucleus_Status (*dispatchConstructor)(void *dispatch);
    
    Nucleus_Type *parentType;
    
    Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdownContext)();
 
    void *dispatch;
}; // struct Nucleus_Type

// `Nucleus_TypeSystem`
typedef struct Nucleus_TypeSystem Nucleus_TypeSystem;

// # `Nucleus_TypeSystem_get`
// *Get the `Nucleus_TypeSystem` singleton.*
// ## C Signature
// ```
// Nucleus_Status
// Nucleus_TypeSystem_get
//   (
//       Nucleus_TypeSystem **typeSystem
//   )
// ```
// ## Parameters
// - `typeSystem` a pointer to a `(Nucleus_TypeSystem *)` variable
// ## Description
// If this function succeeds `*typeSystem` is assigned the address of a `Nucleus_TypeSystem` object.
// This function fails iff `typeSystem` is a null pointer or the `Nucleus_TypeSystem` singleon does not exist.
// `Nucleus_Status_InvalidArgument` is returned in the former case and `Nucleus_Status_NotExists` is returned in the latter case.
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_TypeSystem_get
    (
        Nucleus_TypeSystem **typeSystem
    );

// # `Nucleus_TypeSystem_startup`
// *Startup the `Nucleus_TypeSystem` singleton.*
// ## C Signature
// ```
// Nucleus_Status
// Nucleus_TypeSystem_startup
//     (
//     );
// ```
// ## Description
// If the `Nucleus_TypeSystem` singleton does exist, `Nucleus_Status_Exists` is returned.
// If it does not exist, then it is created.
// If the creation fails, a non-zero status code is returned.
Nucleus_Object_Library_Export Nucleus_Status
Nucleus_TypeSystem_startup
    (
    );

// # `Nucleus_TypeSystem_shutdown`
// *Shutdown the `Nucleus_TypeSystem` singleton.*
// ## C Signature
// ```
// Nucleus_Status
// Nucleus_TypeSystem_shutdown
//     (
//     );
// ```
// ## Description
// If the `Nucleus_TypeSystem` singleton does not exist, `Nucleus_Status_NotExists` is returned.
// If it does exist, it is destroyed.
Nucleus_Object_Library_Export Nucleus_Status
Nucleus_TypeSystem_shutdown
    (
    );

// Register a class.
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_TypeSystem_registerClass
    (
        Nucleus_TypeSystem *typeSystem,
        Nucleus_Type **type,
        const char *name,
        Nucleus_Size objectSize,
        Nucleus_Status(*objectDestructor)(void *object),
        Nucleus_Size classSize,
        Nucleus_Status(*dispatchConstructor)(void *dispatch),
        Nucleus_Type *parentType,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdownContext)()
    );

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
    notifyShutdownContext \
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
    notifyShutdownContext \
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
            Nucleus_TypeSystem *typeSystem; \
            Nucleus_Status status; \
            status = Nucleus_TypeSystem_get(&typeSystem); \
            if (Nucleus_Unlikely(status)) return status; \
            Nucleus_Type *parentType; \
            status = ParentNameCxx##_getType(&parentType); \
            if (Nucleus_Unlikely(status)) return status; \
            status = Nucleus_TypeSystem_registerClass(typeSystem, \
                                                      &g_type, \
                                                      Name, \
                                                      sizeof(NameCxx), \
                                                      (Nucleus_Status (*)(void *))&destruct, \
                                                      sizeof(NameCxx##_Class), \
                                                      (Nucleus_Status (*)(void *))&constructDispatch, \
                                                      parentType, \
                                                      &notifyShutdownContext); \
            if (Nucleus_Unlikely(status)) return status; \
        } \
        *type = g_type; \
        return Nucleus_Status_Success; \
    }
