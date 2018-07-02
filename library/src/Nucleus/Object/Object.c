// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Object.c.i"
#include <stdio.h>

Nucleus_NonNull() static Nucleus_Status
equalTo
    (
        Nucleus_Object *self,
        Nucleus_Object *other,
        Nucleus_Boolean *equalTo
    );

Nucleus_NonNull() static Nucleus_Status
hash
    (
        Nucleus_Object *self,
        Nucleus_HashValue *hashValue
    );

Nucleus_AlwaysSucceed() static Nucleus_Status
notifyShutdownContext
    (
    );

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
destroy
    (
        Nucleus_Object *self
    );

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_Object_Class *dispatch
    );
    
static Nucleus_Type *g_type = NULL;

Nucleus_NonNull() static Nucleus_Status
equalTo
    (
        Nucleus_Object *self,
        Nucleus_Object *other,
        Nucleus_Boolean *equalTo
    )
{
    if (Nucleus_Unlikely(!self || !other || !equalTo)) return Nucleus_Status_InvalidArgument;
    *equalTo = (self == other);
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
hash
    (
        Nucleus_Object *self,
        Nucleus_HashValue *hashValue
    )
{
    if (Nucleus_Unlikely(!self || !hashValue)) return Nucleus_Status_InvalidArgument;
    return Nucleus_hashPointer(self, hashValue);
}

Nucleus_AlwaysSucceed() static Nucleus_Status
notifyShutdownContext
    (
    )
{
    g_type = NULL;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
destroy
    (
        Nucleus_Object *self
    )
{
    while (self->type)
    {
        Nucleus_Type *type = self->type;
        if (NUCLEUS_CLASSTYPE(type)->objectDestructor)
        {
            NUCLEUS_CLASSTYPE(type)->objectDestructor(self);
        }
        self->type = NUCLEUS_CLASSTYPE(type)->parentType;
    }
    Nucleus_deallocateMemory(self);
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_Object_Class *dispatch
    )
{
    NUCLEUS_OBJECT_CLASS(dispatch)->equalTo = (Nucleus_NonNull() Nucleus_Status (*)(Nucleus_Object *, Nucleus_Object *, Nucleus_Boolean *))&equalTo;
    NUCLEUS_OBJECT_CLASS(dispatch)->hash = (Nucleus_NonNull() Nucleus_Status (*)(Nucleus_Object *, Nucleus_HashValue *))&hash;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Object_allocate
    (
        Nucleus_Object **object,
		Nucleus_Type *type
    )
{
	if (Nucleus_Unlikely(!object))
	{
		fprintf(stderr, "%s: invalid argument (%s)\n", "Nucleus_Object_allocate", "NULL == object");
		return Nucleus_Status_InvalidArgument;
	}
	if (Nucleus_Unlikely(!type))
	{
		fprintf(stderr, "%s: invalid argument (%s)\n", "Nucleus_Object_allocate", "NULL == type");
		return Nucleus_Status_InvalidArgument;
	}
	if (type->kind != Nucleus_TypeKind_Class)
	{
		fprintf(stderr, "%s: invalid argument (%s)\n", "Nucleus_Object_allocate", "not a class type");
		return Nucleus_Status_InvalidArgument;
	}

	Nucleus_ClassType *classType = NUCLEUS_CLASSTYPE(type);
	if (classType->parentType)
	{
		Nucleus_ClassType *parentClassType = NUCLEUS_CLASSTYPE(classType->parentType);
		if (Nucleus_Unlikely(classType->objectSize < parentClassType->objectSize))
		{
			fprintf(stderr, "%s: invalid argument (%s)\n", "Nucleus_Object_allocate", "invalid object size");
			return Nucleus_Status_InvalidArgument;
		}
		if (Nucleus_Unlikely(classType->dispatchSize < parentClassType->dispatchSize))
		{
			fprintf(stderr, "%s: invalid argument (%s)\n", "Nucleus_Object_allocate", "invalid dispatch size");
			return Nucleus_Status_InvalidArgument;
		}
	}
	else
	{
		if (Nucleus_Unlikely(classType->objectSize < sizeof(Nucleus_Object)))
		{
			fprintf(stderr, "%s: invalid argument (%s)\n", "Nucleus_Object_allocate", "invalid object size");
			return Nucleus_Status_InvalidArgument;
		}
		if (Nucleus_Unlikely(classType->dispatchSize < sizeof(Nucleus_Object_Class)))
		{
			fprintf(stderr, "%s: invalid argument (%s)\n", "Nucleus_Object_allocate", "invalid dispatch size");
			return Nucleus_Status_InvalidArgument;
		}
	}

    Nucleus_Status status;
	
	status = Nucleus_ClassType_initialize(classType);
	if (Nucleus_Unlikely(status))
	{ return status; }
	
    Nucleus_Object *temporary;
    status = Nucleus_allocateMemory((void **)&temporary, classType->objectSize);
    if (Nucleus_Unlikely(status)) return status;
    
    temporary->type = NULL;
    temporary->referenceCount = 1;

    *object = temporary;

    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Object_getType
    (
        Nucleus_Type **type
    )
{
    if (Nucleus_Unlikely(!type)) return Nucleus_Status_InvalidArgument;
    if (Nucleus_Unlikely(!g_type))
    {
        Nucleus_Status status;
        status = Nucleus_Types_addClassType(&g_type,
                                            u8"Nucleus.Object",
                                            sizeof(Nucleus_Object),
                                            (Nucleus_Status (*)(void *))NULL,
                                            sizeof(Nucleus_Object_Class),
                                            (Nucleus_Status (*)(void *))&constructDispatch,
                                            NULL,
											NULL,
                                            &notifyShutdownContext);
        if (Nucleus_Unlikely(status)) return status;
    }
    *type = g_type;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Object_construct
    (
        Nucleus_Object *self
    )
{
    Nucleus_Status status;
    Nucleus_Type *type;
    status = Nucleus_Object_getType(&type);
    if (Nucleus_Unlikely(status)) return status;
    self->type = type;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_Object_incrementReferenceCount
    (
        Nucleus_Object *self
    )
{ 
    self->referenceCount++;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_Object_decrementReferenceCount
    (
        Nucleus_Object *self
    )
{ 
    if (0 == --self->referenceCount)
    {
        destroy(self);
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Object_equalTo
    (
        Nucleus_Object *self,
        Nucleus_Object *other,
        Nucleus_Boolean *equalTo
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    return NUCLEUS_OBJECT_CLASS(NUCLEUS_CLASSTYPE(self->type)->dispatch)->equalTo(self, other, equalTo);
}

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Object_hash
    (
        Nucleus_Object *self,
        Nucleus_HashValue *hashValue
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    return NUCLEUS_OBJECT_CLASS(NUCLEUS_CLASSTYPE(self->type)->dispatch)->hash(self, hashValue);
}

DEFINE_MODULE(Nucleus_Objects)
