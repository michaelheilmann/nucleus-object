#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Types/Size.h"
#include "Nucleus/Types/HashValue.h"

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
    ClassType classType;
}; // struct Nucleus_Type

Nucleus_NonNull() Nucleus_Status
createClassType
    (
        Nucleus_Type **type,
        const char *name,
        Nucleus_Size objectSize,
        Nucleus_Status (*objectDestructor)(void *object),
        Nucleus_Size dispatchSize,
        Nucleus_Status (*dispatchConstructor)(void *dispatch),
        Nucleus_Type *parentType,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)()
    );
 
Nucleus_NonNull() Nucleus_Status
destroyClassType
    (
        Nucleus_Type *type
    );
