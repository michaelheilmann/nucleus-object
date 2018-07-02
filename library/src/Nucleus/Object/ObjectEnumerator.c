// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/ObjectEnumerator.h"

Nucleus_ClassTypeDefinition(Nucleus_Object_Library_Export,
                            "Nucleus.ObjectEnumerator",
                            Nucleus_ObjectEnumerator,
                            Nucleus_Object)
                            
Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_ObjectEnumerator_Class *dispatch
    )
{
    NUCLEUS_OBJECTENUMERATOR_CLASS(dispatch)->hasObject = NULL;
    NUCLEUS_OBJECTENUMERATOR_CLASS(dispatch)->nextObject = NULL;
    NUCLEUS_OBJECTENUMERATOR_CLASS(dispatch)->getObject = NULL;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
constructSignals
    (
        Nucleus_ObjectEnumerator_Class *dispatch
    )
{ return Nucleus_Status_Success; }

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
destruct
    (
        Nucleus_ObjectEnumerator *self
    )
{ return Nucleus_Status_Success; }

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectEnumerator_construct
    (
        Nucleus_ObjectEnumerator *self
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    Nucleus_Type *type;
    Nucleus_Status status;
    status = Nucleus_ObjectEnumerator_getType(&type);
    if (Nucleus_Unlikely(status)) return status;
    status = Nucleus_Object_construct(NUCLEUS_OBJECT(self));
    if (Nucleus_Unlikely(status)) return status;
    NUCLEUS_OBJECT(self)->type = type;
    return Nucleus_Status_Success; 
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectEnumerator_hasObject
    (
        Nucleus_ObjectEnumerator *self,
        Nucleus_Boolean *hasObject
    )
{ return NUCLEUS_OBJECTENUMERATOR_CLASS(NUCLEUS_CLASSTYPE(NUCLEUS_OBJECT(self)->type)->dispatch)->hasObject(self, hasObject); }

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectEnumerator_nextObject
    (
        Nucleus_ObjectEnumerator *self
    )
{ return NUCLEUS_OBJECTENUMERATOR_CLASS(NUCLEUS_CLASSTYPE(NUCLEUS_OBJECT(self)->type)->dispatch)->nextObject(self); }

Nucleus_NonNull() Nucleus_Status
Nucleus_ObjectEnumerator_getObject
    (
        Nucleus_ObjectEnumerator *self,
        Nucleus_Object **object
    )
{ return NUCLEUS_OBJECTENUMERATOR_CLASS(NUCLEUS_CLASSTYPE(NUCLEUS_OBJECT(self)->type)->dispatch)->getObject(self, object); }
