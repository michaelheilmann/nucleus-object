#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Types/Size.h"
#include "Nucleus/Types/HashValue.h"
#include "Nucleus/Object/Exports.h"

#define NUCLEUS_CAST(T,E) ((T)(E))

// `Nucleus_Type`
#define NUCLEUS_TYPE(pointer) ((Nucleus_Type *)(pointer))
typedef struct Nucleus_Type Nucleus_Type;

typedef enum Nucleus_TypeKind
{
    Nucleus_TypeKind_Class,
} Nucleus_TypeKind; // Nucleus_TypeKind

struct Nucleus_Type
{
    char *name;
    Nucleus_TypeKind kind;
    Nucleus_HashValue hashValue;
    Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)();
}; // struct Nucleus_Type

Nucleus_NonNull() Nucleus_Status
Nucleus_Type_construct
	(
		Nucleus_Type *type,
		const char *name,
		Nucleus_TypeKind kind,
		Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)()
	);

Nucleus_NonNull() Nucleus_Status
Nucleus_Type_destruct
	(
		Nucleus_Type *type
	);

typedef enum Nucleus_ClassTypeState Nucleus_ClassTypeState;

enum Nucleus_ClassTypeState
{
	Nucleus_ClassTypeState_Uninitialized,
	Nucleus_ClassTypeState_DispatchInitialized,
	Nucleus_ClassTypeState_SignalsInstalled,
}; // enum Nucleus_ClassTypeState


#define NUCLEUS_CLASSTYPE(pointer) ((Nucleus_ClassType *)(pointer))
typedef struct Nucleus_ClassType Nucleus_ClassType;

struct Nucleus_ClassType
{
	Nucleus_Type parent;

    Nucleus_Size objectSize;
    Nucleus_Status (*objectDestructor)(void *object);
    
    Nucleus_Size dispatchSize;
    Nucleus_Status (*dispatchConstructor)(void *dispatch);
    void *dispatch;

	Nucleus_Status (*signalsConstructor)(void *dispatch);
    
    Nucleus_Type *parentType;

	Nucleus_ClassTypeState state;
	
}; // struct Nucleus_ClassType

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ClassType_initialize
	(
		Nucleus_ClassType *type
	);
	
Nucleus_NonNull() Nucleus_Status
Nucleus_ClassType_uninitialize
	(
		Nucleus_ClassType *type
	);

Nucleus_NonNull() Nucleus_Status
Nucleus_ClassType_construct
	(
        Nucleus_ClassType *type,
        const char *name,
        Nucleus_Size objectSize,
        Nucleus_Status (*objectDestructor)(void *object),
        Nucleus_Size dispatchSize,
        Nucleus_Status (*dispatchConstructor)(void *dispatch),
		Nucleus_Status (*signalsConstructor)(void *dispatch),
        Nucleus_Type *parentType,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)()
	);

Nucleus_NonNull() Nucleus_Status
Nucleus_ClassType_destruct
	(
		Nucleus_ClassType *type
	);

Nucleus_NonNull() Nucleus_Status
Nucleus_ClassType_create
    (
        Nucleus_ClassType **type,
        const char *name,
        Nucleus_Size objectSize,
        Nucleus_Status (*objectDestructor)(void *object),
        Nucleus_Size dispatchSize,
        Nucleus_Status (*dispatchConstructor)(void *dispatch),
		Nucleus_Status (*signalsConstructor)(void *dispatch),
        Nucleus_Type *parentType,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)()
    );
 
Nucleus_NonNull() Nucleus_Status
Nucleus_Type_destroy
	(
		Nucleus_Type *type
	);
