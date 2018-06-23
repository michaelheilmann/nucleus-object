// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Types/HashValue.h"
#include "Nucleus/Types/Size.h"
#include "Nucleus/Object/Exports.h"
#include "Nucleus/Object/Type.h"
#include "Nucleus/Object/Module.h"
#include <assert.h>

// Class types are supported if this is defined and 1.
#define Nucleus_WithClassTypes (1)


// `Nucleus_DynamicLibrary`
typedef struct Nucleus_DynamicLibrary Nucleus_DynamicLibrary;

DECLARE_MODULE(Nucleus_Types)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NonNull() Nucleus_Status
Nucleus_Type_hash
    (
        Nucleus_Type *type,
        Nucleus_HashValue *hashValue
    );

#if defined(Nucleus_WithClassTypes) && 1 == Nucleus_WithClassTypes

// Add a class type.
Nucleus_Object_Library_Export Nucleus_NonNull(1, 2) Nucleus_Status
Nucleus_Types_addClassType
    (
        Nucleus_Type **type,
        const char *name,
        Nucleus_Size objectSize,
        Nucleus_Status(*objectDestructor)(void *object),
        Nucleus_Size classSize,
        Nucleus_Status (*dispatchConstructor)(void *dispatch),
		Nucleus_Status (*signalsConstructor)(void *dispatch),
        Nucleus_Type *parentType,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)(Nucleus_Type *)
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
	Nucleus_NoWarnUnused() Nucleus_NonNull() static NameCxx##_Class * \
	getDispatch \
		( \
			NameCxx *self \
		); \
\
    Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status \
    constructDispatch \
        ( \
            NameCxx##_Class *dispatch \
        ); \
\
	Nucleus_NonNull() static Nucleus_Status \
	constructSignals \
		( \
			NameCxx##_Class *dispatch \
		); \
\
    Nucleus_AlwaysSucceed() static Nucleus_Status \
    notifyShutdown \
        ( \
            Nucleus_Type *type \
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
            Nucleus_Type *type \
        ) \
    { \
        assert(type == g_type); \
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
												(Nucleus_Status (*)(void *))&constructSignals, \
                                                parentType, \
                                                &notifyShutdown); \
            if (Nucleus_Unlikely(status)) return status; \
        } \
        *type = g_type; \
        return Nucleus_Status_Success; \
    } \
\
	Nucleus_NoWarnUnused() Nucleus_NonNull() static NameCxx##_Class * \
	getDispatch \
		( \
			NameCxx *self \
		) \
	{ \
		return NUCLEUS_CAST(NameCxx##_Class *,NUCLEUS_CAST(Nucleus_ClassType *, NUCLEUS_CAST(Nucleus_Object *, self)->type)->dispatch); \
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
