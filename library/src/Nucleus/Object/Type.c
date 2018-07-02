#include "Nucleus/Object/Type.h"

#include "Nucleus/Hash.h"
#include "Nucleus/Memory.h"
#include "Nucleus/Object/Object.h"
#include "Nucleus/Object/Signals.h"
#include <string.h>
#include <stdio.h>

Nucleus_NonNull() static Nucleus_Status
Nucleus_ClassType_destroy
    (
        Nucleus_ClassType *type
    );

Nucleus_NonNull() static Nucleus_Status
Nucleus_ClassType_destroy
    (
        Nucleus_ClassType *type
    )
{
	Nucleus_ClassType_destruct(type);
    Nucleus_deallocateMemory(type);
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Type_construct
	(
		Nucleus_Type *type,
		const char *name,
		Nucleus_TypeKind kind,
		Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)()
	)
{ 
    // Store the kind of the type.
    type->kind = kind;
    // Store the type name.
    type->name = strdup(name);
    if (Nucleus_Unlikely(!type->name))
    {
        return Nucleus_Status_AllocationFailed;
    }
    // Store the type name hash.
    Nucleus_hashMemory(name, strlen(name), &type->hashValue);
    // Store shutdown callback.
    type->notifyShutdown = notifyShutdown;
	// Return success.
	return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Type_destruct
	(
		Nucleus_Type *type
	)
{
    Nucleus_deallocateMemory(type->name);
    type->name = NULL;
	return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ClassType_initialize
	(
		Nucleus_ClassType *type
	)
{
	if (type->state < Nucleus_ClassTypeState_SignalsInstalled)
	{
		do
		{
			switch (type->state)
			{
				case Nucleus_ClassTypeState_Uninitialized:
					{
						Nucleus_Status status;
						//
						fprintf(stdout, "type %s: initializing dispatch\n",
						                NUCLEUS_TYPE(type)->name);
						// Ensure parent class type is initialized.
						Nucleus_ClassType *parentType = NULL;
						if (type->parentType)
						{
							parentType = NUCLEUS_CLASSTYPE(type->parentType);
							status = Nucleus_ClassType_initialize(parentType);
							if (Nucleus_Unlikely(status)) return status;
						}
						//
						status = Nucleus_allocateMemory((void **)&(type->dispatch),
														type->dispatchSize);
						if (Nucleus_Unlikely(status))
						{
							fprintf(stderr, "type %s: unable to allocate dispatch\n",
							                NUCLEUS_TYPE(type)->name);
							return status;
						}
						//
						if (parentType)
						{
							Nucleus_copyMemory((void *)type->dispatch,
											   (void *)parentType->dispatch,
											   parentType->dispatchSize);
						}
						//
						NUCLEUS_OBJECT_CLASS(type->dispatch)->type = NUCLEUS_TYPE(type);
						//
						if (type->dispatchConstructor)
						{
							status = type->dispatchConstructor(type->dispatch);
							if (Nucleus_Unlikely(status))
							{
								Nucleus_deallocateMemory(type->dispatch);
								type->dispatch = NULL;
								fprintf(stderr, "type %s: unable to allocate dispatch\n",
								                NUCLEUS_TYPE(type)->name);
								return status;
							}
						}
						//
						fprintf(stdout, "type %s: dispatch initialized\n",
						                NUCLEUS_TYPE(type)->name);
						//
						type->state = Nucleus_ClassTypeState_DispatchInitialized;
					} break;
				case Nucleus_ClassTypeState_DispatchInitialized:
					{
						fprintf(stdout, "type %s: installing signals\n",
						                NUCLEUS_TYPE(type)->name);
						if (type->signalsConstructor)
						{
							Nucleus_Status status;
							status = type->signalsConstructor(type->dispatch);
							if (Nucleus_Unlikely(status))
							{
								// It is RECOMMENDED that signal installer implementations
								// revert everything on failure on their own. To ensure
								// fidelity, we make sure here that all signals are
								// certainly removed.
								Nucleus_Signals_removeAllSignals(NUCLEUS_TYPE(type));
								return status;
							}
						}
						fprintf(stdout, "type %s: signals installed\n",
						                NUCLEUS_TYPE(type)->name);
						type->state = Nucleus_ClassTypeState_SignalsInstalled;
					} break;
				case Nucleus_ClassTypeState_SignalsInstalled:
					{
					} break;
				default:
					return Nucleus_Status_InternalError;
			};
		} while (type->state < Nucleus_ClassTypeState_SignalsInstalled);
	} //
	return Nucleus_Status_Success;
}
	
Nucleus_NonNull() Nucleus_Status
Nucleus_ClassType_uninitialize
	(
		Nucleus_ClassType *type
	)
{
	if (type->state > Nucleus_ClassTypeState_Uninitialized)
	{
		do
		{
			switch (type->state)
			{
				case Nucleus_ClassTypeState_SignalsInstalled:
				{
					type->state = Nucleus_ClassTypeState_DispatchInitialized;
				} break;
				case Nucleus_ClassTypeState_DispatchInitialized:
				{
					if (type->dispatch)
					{
						Nucleus_deallocateMemory(type->dispatch);
						type->dispatch = NULL;
					}
					type->state = Nucleus_ClassTypeState_Uninitialized;
				} break;
				default:
					return Nucleus_Status_InternalError;
			};
		} while (type->state > Nucleus_ClassTypeState_Uninitialized);
	}
	return Nucleus_Status_Success;
}

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
	)
{
	Nucleus_Status status;
	//
	status = Nucleus_Type_construct(NUCLEUS_TYPE(type),
	                                name,
									Nucleus_TypeKind_Class, 
	                                notifyShutdown);
	if (Nucleus_Unlikely(status))
	{ return status; }
    //
    type->objectSize = objectSize;
    type->objectDestructor = objectDestructor;
    type->dispatchSize = dispatchSize;
    type->dispatchConstructor = dispatchConstructor;
    type->parentType = parentType;
    type->dispatch = NULL;
	type->signalsConstructor = signalsConstructor;
	type->state = Nucleus_ClassTypeState_Uninitialized;
	//
	return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ClassType_destruct
	(
		Nucleus_ClassType *type
	)
{
	if (type->dispatch)
	{
		Nucleus_deallocateMemory(type->dispatch);
		type->dispatch = NULL;
	}
	Nucleus_Type_destruct(NUCLEUS_TYPE(type));
	return Nucleus_Status_Success;
}

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
    )
{
    Nucleus_Status status;
    Nucleus_ClassType *temporary;
    // Allocate the type.
    status = Nucleus_allocateMemory((void **)&temporary, sizeof(Nucleus_ClassType));
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
	// Construct the type.
	status = Nucleus_ClassType_construct(temporary,
	                                     name,
										 objectSize,
										 objectDestructor,
										 dispatchSize,
										 dispatchConstructor,
										 signalsConstructor,
										 parentType,
										 notifyShutdown);
	if (Nucleus_Unlikely(status))
	{
		Nucleus_deallocateMemory(temporary);
		return status;
	}
    // Return result.
    *type = temporary;
    // Return success.
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_Type_destroy
	(
		Nucleus_Type *type
	)
{
	switch(type->kind)
	{
		case Nucleus_TypeKind_Class:
			return Nucleus_ClassType_destroy(NUCLEUS_CLASSTYPE(type));
		default:
			return Nucleus_Status_InvalidArgument;
	};
}
 

