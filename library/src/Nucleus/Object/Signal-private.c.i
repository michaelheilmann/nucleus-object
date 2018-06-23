#include "Nucleus/Object/Signal-private.h.i"

Nucleus_NonNull() static Nucleus_Status
Signal_initialize
    (
        Nucleus_Signal *signal,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    )
{
    //
    signal->name = name;
    Nucleus_ImmutableString_lock(name);
    //
    signal->type = type;
    //
    Nucleus_HashValue nameHashValue;
    Nucleus_ImmutableString_hash(name, &nameHashValue);
    Nucleus_HashValue typeHashValue;
    Nucleus_Type_hash(type, &typeHashValue);
    Nucleus_combineHashValues(nameHashValue,
                              typeHashValue,
                              &signal->hashValue);
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
Signal_uninitialize
    (
        Nucleus_Signal *signal
    )
{
    //
    signal->type = NULL;   
    //
    Nucleus_Object_incrementReferenceCount(NUCLEUS_OBJECT(signal->name));
    signal->name = NULL;
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
Signal_create
    (
        Nucleus_Signal **signal,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    )
{
    Nucleus_Status status;

    Nucleus_Signal *temporary;

    status = Nucleus_allocateMemory((void **)&temporary, sizeof(Nucleus_Signal));
    if (Nucleus_Unlikely(status))
    { return status; }

    status = Signal_initialize(temporary, name, type);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(temporary);
        return status;
    }

    *signal = temporary;

    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
Signal_destroy
    (
        Nucleus_Signal *signal
    )
{
    Signal_uninitialize(signal);
    Nucleus_deallocateMemory(signal);
    return Nucleus_Status_Success;
}
