// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Signals-private.h.i"

#if defined(Nucleus_WithSignals) && 1 == Nucleus_WithSignals

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NonNull() static Nucleus_Status
SignalKey_equalTo
    (
        SignalKey *left,
        SignalKey *right,
        Nucleus_Boolean *equalTo
    )
{
    if (left->type != right->type || left->hashValue != right->hashValue)
    { *equalTo = Nucleus_Boolean_False; return Nucleus_Status_Success; }
    return Nucleus_Object_equalTo(NUCLEUS_OBJECT(left->name), NUCLEUS_OBJECT(right->name), equalTo);
}

Nucleus_NonNull() static Nucleus_Status
SignalKey_hash
    (
        SignalKey *signalKey,
        Nucleus_HashValue *hashValue
    )
{
    *hashValue = signalKey->hashValue;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
SignalKey_initialize
    (
        SignalKey *signalKey,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    )
{
    //
    signalKey->name = name;
    Nucleus_Object_incrementReferenceCount(NUCLEUS_OBJECT(name));
    Nucleus_Object_hash(NUCLEUS_OBJECT(name), &signalKey->typeHashValue);
    //
    signalKey->type = type;
    Nucleus_Type_hash(type, &signalKey->hashValue);
    //
    Nucleus_combineHashValues(signalKey->nameHashValue,
                              signalKey->typeHashValue,
                              &signalKey->hashValue);
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
SignalKey_uninitialize
    (
        SignalKey *signalKey
    )
{
    //
    signalKey->typeHashValue = 0;
    signalKey->type = NULL;   
    //
    signalKey->nameHashValue = 0;
    Nucleus_Object_incrementReferenceCount(NUCLEUS_OBJECT(signalKey->name));
    signalKey->name = NULL;
    //
    signalKey->hashValue = 0;
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
SignalKey_create
    (
        SignalKey **signalKey,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    )
{
    Nucleus_Status status;

    SignalKey *temporary;

    status = Nucleus_allocateMemory((void **)&temporary, sizeof(SignalKey));
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    
    status = SignalKey_initialize(temporary, name, type);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(temporary);
        return status;
    }
    
    *signalKey = temporary;

    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
SignalKey_destroy
    (
        SignalKey *signalKey
    )
{
    SignalKey_uninitialize(signalKey);
    Nucleus_deallocateMemory(signalKey);
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
lookupInClasses
    (
        Nucleus_Signal **signal,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    )
{
    SignalKey *key;
    Nucleus_Status status;

    //
    status = SignalKey_create(&key, name, type);
    if (Nucleus_Unlikely(status))
    { return status; }
    
    //
    do
    {
        // Lookup the signal by key.
        Nucleus_Signal *temporary;
        status = Nucleus_Collections_PointerHashMap_get(&g_singleton->signals,
                                                        key,
                                                        (void **)&temporary);
        if (status == Nucleus_Status_NotExists)
        { 
            type = type->classType.parentType;
            SignalKey_destroy(key);
            if (!type)
            {
                break;
            }
            status = SignalKey_create(&key, name, type);
            if (Nucleus_Unlikely(status))
            {
                return status;
            }         
        }
        else if (status == Nucleus_Status_Success)
        { 
            *signal = temporary;
            SignalKey_destroy(key);
            return Nucleus_Status_Success;
        }
        else
        {
            SignalKey_destroy(key);
            return status;
        }
    } while (Nucleus_Boolean_True);
    //
    *signal = NULL;
    //
    return Nucleus_Status_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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
    Nucleus_Object_incrementReferenceCount(NUCLEUS_OBJECT(name));
    //
    signal->type = type;
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NonNull() static Nucleus_Status
initialize
    (
        Signals *signals
    )
{
    if (Nucleus_Unlikely(!signals)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    // A signal key is a lightweight structure consisting of the signal name and the signal type.
    // The hash value computed from the signal name and the signal type is cached in the signal key.
    status = Nucleus_Collections_PointerHashMap_initialize(&signals->signals,
                                                           8,
                                                           NULL,
                                                           NUCLEUS_UNLOCKFUNCTION(&SignalKey_destroy),
                                                           NUCLEUS_HASHFUNCTION(&SignalKey_hash), // hash
                                                           NUCLEUS_EQUALTOFUNCTION(&SignalKey_equalTo), // equal
                                                           NULL,
                                                           NUCLEUS_UNLOCKFUNCTION(&Signal_destroy));
    if (Nucleus_Unlikely(status))
    {
        return status; 
    }
    //
    signals->referenceCount = 1;
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
uninitialize
    (
        Signals *signals
    )
{
    Nucleus_Collections_PointerHashMap_uninitialize(&signals->signals); // Must not fail.
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
create
    (
        Signals **signals
    )
{
    if (Nucleus_Unlikely(!signals)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    Signals *temporary;
    status = Nucleus_allocateMemory((void **)&temporary, sizeof(Signals));
    if (Nucleus_Unlikely(status)) return status;
    status = initialize(temporary);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(temporary);
        return status;
    }
    *signals = temporary;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
destroy
    (
        Signals *signals
    )
{
    if (Nucleus_Unlikely(!signals)) return Nucleus_Status_InvalidArgument;
    uninitialize(signals);
    Nucleus_deallocateMemory(signals);
    return Nucleus_Status_Success;
}

#endif
