#include "Nucleus/Object/ObjectArrayEnumerator.h"

Nucleus_ClassTypeDefinition(Nucleus_Object_Library_Export,
                            u8"Nucleus.ObjectArrayEnumerator",
                            Nucleus_ObjectArrayEnumerator,
                            Nucleus_ObjectEnumerator)

Nucleus_NonNull() static Nucleus_Status
getObject
    (
        Nucleus_ObjectEnumerator *self,
        Nucleus_Object **object
    );

Nucleus_NonNull() static Nucleus_Status
hasObject
    (
        Nucleus_ObjectEnumerator *self,
        Nucleus_Boolean *hasObject
    );

Nucleus_NonNull() static Nucleus_Status
nextObject
    (
        Nucleus_ObjectEnumerator *self
    );

Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_ObjectArrayEnumerator_Class *dispatch
    )
{
    NUCLEUS_OBJECTENUMERATOR_CLASS(dispatch)->getObject = &getObject;
    NUCLEUS_OBJECTENUMERATOR_CLASS(dispatch)->hasObject = &hasObject;
    NUCLEUS_OBJECTENUMERATOR_CLASS(dispatch)->nextObject = &nextObject;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
constructSignals
    (
        Nucleus_ObjectArrayEnumerator_Class *dispatch
    )
{ return Nucleus_Status_Success; }

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
destruct
    (
        Nucleus_ObjectArrayEnumerator *self
    )
{ return Nucleus_Status_Success; }

Nucleus_NonNull() static Nucleus_Status
getObject
    (
        Nucleus_ObjectEnumerator *self,
        Nucleus_Object **object
    )
{
    Nucleus_Status status;
    Nucleus_Object *temporary;
    status = Nucleus_Collections_PointerArray_Enumerator_getValue(&NUCLEUS_OBJECTARRAYENUMERATOR(self)->implementation,
                                                                  (void **)&temporary);
    if (Nucleus_Unlikely(status)) return status;
    Nucleus_Object_incrementReferenceCount(temporary);
    *object = temporary;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
hasObject
    (
        Nucleus_ObjectEnumerator *self,
        Nucleus_Boolean *hasObject
    )
{
    return Nucleus_Collections_PointerArray_Enumerator_hasValue(&NUCLEUS_OBJECTARRAYENUMERATOR(self)->implementation,
                                                                hasObject);
}

Nucleus_NonNull() static Nucleus_Status
nextObject
    (
        Nucleus_ObjectEnumerator *self
    )
{
    return Nucleus_Collections_PointerArray_Enumerator_nextValue(&NUCLEUS_OBJECTARRAYENUMERATOR(self)->implementation);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectArrayEnumerator_construct
    (
        Nucleus_ObjectArrayEnumerator *self,
        Nucleus_ObjectArray *objectArray
    )
{
    if (Nucleus_Unlikely(!self || !objectArray)) return Nucleus_Status_InvalidArgument;
    //
    Nucleus_Type *type;
    Nucleus_Status status;
    // (1) Get the type.
    status = Nucleus_ObjectArrayEnumerator_getType(&type);
    if (Nucleus_Unlikely(status)) return status;
    // (2) Invoke constructor of parent class.
    Nucleus_ObjectEnumerator_construct(NUCLEUS_OBJECTENUMERATOR(self));
    // (3) 
    status = Nucleus_Collections_PointerArray_Enumerator_initialize(&self->implementation,
                                                                    &objectArray->implementation);
    if (Nucleus_Unlikely(status)) return status;
    //
    NUCLEUS_OBJECT(self)->type = type;
    return Nucleus_Status_Success;
}

Nucleus_DefineCreate(Nucleus_ObjectArrayEnumerator,
                     Nucleus_Parameters(Nucleus_ObjectArray *objectArray),
					 Nucleus_Arguments(objectArray))
