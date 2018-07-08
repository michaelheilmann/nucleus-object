// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Signals-private.h.i"

#include "Nucleus/Hash/Pointer.h"
#include "Nucleus/EqualTo/Pointer.h"

#if defined(Nucleus_WithSignals) && 1 == Nucleus_WithSignals

DEFINE_MODULE_PRIVATE(Nucleus_Signals)

Nucleus_NonNull() static Nucleus_Status
addSignal
    (
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    )
{
    Nucleus_Status status;
    // Search the class for the signal.
    Nucleus_Signal *s;
    status = lookupInClasses(&s, name, type);
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    if (s)
    {
        fprintf(stderr, u8"%s:%d: signal already exists\n", __FILE__, __LINE__);
        return Nucleus_Status_Exists;
    }
    // The signal does not exist. Create it, add it.
    status = Signal_create(&s, name, type);
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    SignalKey *k;
    status = SignalKey_create(&k, name, type);
    if (Nucleus_Unlikely(status))
    {
        Signal_destroy(s);
        return status;
    }
    status = Nucleus_Collections_PointerHashMap_set(&g_singleton->signals,
                                                    (void *)k, (void *)s,
                                                    Nucleus_Boolean_False);
    if (Nucleus_Unlikely(status))
    {
        SignalKey_destroy(k);
        Signal_destroy(s);
        return status;
    }

    // Return success.
    return Nucleus_Status_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NonNull() static Nucleus_Status
SignalKey_equalTo
    (
        SignalKey *left,
        SignalKey *right,
        Nucleus_Boolean *equalTo
    )
{
    if (left == right)
    { *equalTo = Nucleus_Boolean_True; return Nucleus_Status_Success; }
    if (left->type != right->type || left->hashValue != right->hashValue)
    { *equalTo = Nucleus_Boolean_False; return Nucleus_Status_Success; }
    return Nucleus_ImmutableString_equalTo(left->name, right->name, equalTo);
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
    Nucleus_ImmutableString_lock(name);
    Nucleus_ImmutableString_hash(name, &signalKey->nameHashValue);
    //
    signalKey->type = type;
    Nucleus_Type_hash(type, &signalKey->typeHashValue);
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
    Nucleus_ImmutableString_unlock(signalKey->name);
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
            type = NUCLEUS_CLASSTYPE(type)->parentType;
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
__Nucleus_Signals_initialize
    (
        Nucleus_Signals *module
    )
{
    if (Nucleus_Unlikely(!module)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    //
    status = Nucleus_Types_initialize();
    if (Nucleus_Unlikely(status)) return status;
    // A signal key is a lightweight structure consisting of the signal name and the signal type.
    // The hash value computed from the signal name and the signal type is cached in the signal key.
	fprintf(stdout, "initializing signal hash map\n");
    status = Nucleus_Collections_PointerHashMap_initialize(&module->signals,
                                                           8,
                                                           NULL,
                                                           NUCLEUS_UNLOCKFUNCTION(&SignalKey_destroy),
                                                           NUCLEUS_HASHFUNCTION(&SignalKey_hash), // hash
                                                           NUCLEUS_EQUALTOFUNCTION(&SignalKey_equalTo), // equal
                                                           NULL,
                                                           NUCLEUS_UNLOCKFUNCTION(&Signal_destroy));
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Types_uninitialize();
        return status; 
    }
    //
	fprintf(stdout, "initializing connections hash map\n");
    status = Nucleus_Collections_PointerHashMap_initialize(&module->connections,
                                                           8,
                                                           NULL,
                                                           NULL,
                                                           NUCLEUS_HASHFUNCTION(&Nucleus_hashPointer), // hash
                                                           NUCLEUS_EQUALTOFUNCTION(&Nucleus_equalToPointer), // equal
                                                           NULL,
                                                           NUCLEUS_UNLOCKFUNCTION(&Connections_destroy));
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Collections_PointerHashMap_uninitialize(&module->signals);
        Nucleus_Types_uninitialize();
        return status; 
    }
    //
    module->referenceCount = 1;
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
__Nucleus_Signals_uninitialize
    (
        Nucleus_Signals *module
    )
{
    Nucleus_Collections_PointerHashMap_uninitialize(&module->connections); // Must not fail.
    Nucleus_Collections_PointerHashMap_uninitialize(&module->signals); // Must not fail.
    Nucleus_Types_uninitialize();
    return Nucleus_Status_Success;
}

#endif
