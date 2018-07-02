// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Types/Boolean.h"
#include "Nucleus/Types/HashValue.h"
#include "Nucleus/Types/Size.h"
#include "Nucleus/Object/Exports.h"
#include "Nucleus/Object/Module.h"

// Forward declaration.
typedef struct Nucleus_Type Nucleus_Type;

DECLARE_MODULE(Nucleus_Objects)

#define NUCLEUS_OBJECT(p) ((Nucleus_Object *)(p))
typedef struct Nucleus_Object Nucleus_Object;

#define NUCLEUS_OBJECT_CLASS(p) ((Nucleus_Object_Class *)(p))
typedef struct Nucleus_Object_Class Nucleus_Object_Class;

struct Nucleus_Object_Class
{
    Nucleus_Type *type;
    Nucleus_Status (*equalTo)(Nucleus_Object *self, Nucleus_Object *other, Nucleus_Boolean *equalTo);
    Nucleus_Status (*hash)(Nucleus_Object *self, Nucleus_HashValue *hashValue);
}; // struct Nucleus_Object_Class

struct Nucleus_Object
{
    int referenceCount;
    Nucleus_Type *type;
}; // struct Nucleus_Object

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Object_getType
    (
        Nucleus_Type **type
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Object_allocate
    (
        Nucleus_Object **object,
        Nucleus_Type *type
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Object_construct
    (
        Nucleus_Object *self
    );

Nucleus_Object_Library_Export Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_Object_incrementReferenceCount
    (
        Nucleus_Object *self
    );

Nucleus_Object_Library_Export Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_Object_decrementReferenceCount
    (
        Nucleus_Object *self
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Object_equalTo
    (
        Nucleus_Object *self,
        Nucleus_Object *other,
        Nucleus_Boolean *equalTo
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Object_hash
    (
        Nucleus_Object *self,
        Nucleus_HashValue *hashValue
    );

#define Nucleus_Parameters(...) \
	__VA_ARGS__
	
#define Nucleus_Arguments(...) \
	__VA_ARGS__

#define Nucleus_DeclareDefaultCreate(NameCxx) \
	Nucleus_NonNull() Nucleus_Status \
	NameCxx##_create \
		( \
			NameCxx **object \
		);

#define Nucleus_DefineDefaultCreate(NameCxx) \
	Nucleus_NonNull() Nucleus_Status \
	NameCxx##_create \
		( \
			NameCxx **object \
		) \
	{ \
		/* Validate arguments. */ \
		if (Nucleus_Unlikely(!object)) return Nucleus_Status_InvalidArgument; \
		/* Local variables. */ \
		Nucleus_Type *type; \
		NameCxx *temporary; \
		Nucleus_Status status; \
		/* Get the type. */ \
		status = NameCxx##_getType(&type); \
		if (Nucleus_Unlikely(status)) return status; \
		/* Allocate object. */ \
		status = Nucleus_Object_allocate((Nucleus_Object **)&temporary, type); \
		if (Nucleus_Unlikely(status)) return status; \
		/* Construct object. */ \
		status = NameCxx##_construct(temporary); \
		if (Nucleus_Unlikely(status)) \
		{ \
			Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(temporary)); \
			return status; \
		} \
		/* Return object. */ \
		*object = temporary; \
		/* Return success. */ \
		return Nucleus_Status_Success; \
	}
	
#define Nucleus_DeclareCreate(NameCxx, ParametersCxx, ArgumentsCxx) \
	Nucleus_NonNull() Nucleus_Status \
	NameCxx##_create \
		( \
			NameCxx **object, \
			ParametersCxx \
		);

#define Nucleus_DefineCreate(NameCxx, ParametersCxx, ArgumentsCxx) \
	Nucleus_NonNull() Nucleus_Status \
	NameCxx##_create \
		( \
			NameCxx **object, \
			ParametersCxx \
		) \
	{ \
		/* Validate arguments. */ \
		if (Nucleus_Unlikely(!object)) return Nucleus_Status_InvalidArgument; \
		/* Local variables. */ \
		Nucleus_Type *type; \
		NameCxx *temporary; \
		Nucleus_Status status; \
		/* Get the type. */ \
		status = NameCxx##_getType(&type); \
		if (Nucleus_Unlikely(status)) return status; \
		/* Allocate object. */ \
		status = Nucleus_Object_allocate((Nucleus_Object **)&temporary, type); \
		if (Nucleus_Unlikely(status)) return status; \
		/* Construct object. */ \
		status = NameCxx##_construct(temporary, ArgumentsCxx); \
		if (Nucleus_Unlikely(status)) \
		{ \
			Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(temporary)); \
			return status; \
		} \
		/* Return object. */ \
		*object = temporary; \
		/* Return success. */ \
		return Nucleus_Status_Success; \
	}
