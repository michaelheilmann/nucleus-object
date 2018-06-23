// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/Signals.h"

#if defined(Nucleus_WithSignals) && 1 == Nucleus_WithSignals

#include "Nucleus/Memory.h"
#include "Nucleus/Collections/PointerHashMap.h"
#include "Nucleus/Hash/Memory.h"
#include "Nucleus/Hash/combineHashValues.h"
#include "Nucleus/Object/String.h"
#include "Nucleus/Object/ImmutableString.h"
#include "Nucleus/Object/Signal-private.c.i"
#include "Nucleus/Object/Connection-private.c.i"
#include <string.h>
#include <stdio.h>

#define Nucleus_Signals_Mutex int
#define Nucleus_Signals_Mutex_Initializer (0)
#define Nucleus_Signals_Mutex_lock(mutex) /* - */
#define Nucleus_Signals_Mutex_unlock(mutex) /* - */
DECLARE_MODULE_PRIVATE(Nucleus_Signals)

struct Nucleus_Signals
{
    int referenceCount;
    // A map from signal keys to signals.
    Nucleus_Collections_PointerHashMap signals;
    // A map from object connections keys to object connections.
    Nucleus_Collections_PointerHashMap connections;
}; // struct Nucleus_Signals

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct SignalKey
{
    Nucleus_ImmutableString *name; // TODO: Add and use Nucleus_Atom.
    Nucleus_HashValue nameHashValue;
    
    Nucleus_Type *type;
    Nucleus_HashValue typeHashValue;
    
    Nucleus_HashValue hashValue;
} SignalKey;

Nucleus_NonNull() static Nucleus_Status
SignalKey_equalTo
    (
        SignalKey *left,
        SignalKey *right,
        Nucleus_Boolean *equalTo
    );

Nucleus_NonNull() static Nucleus_Status
SignalKey_hash
    (
        SignalKey *signalKey,
        Nucleus_HashValue *hashValue
    );

Nucleus_NonNull() static Nucleus_Status
SignalKey_initialize
    (
        SignalKey *signalKey,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    );

Nucleus_NonNull() static Nucleus_Status
SignalKey_uninitialize
    (
        SignalKey *signalKey
    );

Nucleus_NonNull() static Nucleus_Status
SignalKey_create
    (
        SignalKey **signalKey,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    );

Nucleus_NonNull() static Nucleus_Status
SignalKey_destroy
    (
        SignalKey *signalKey
    );

// Search the class hierarchy for a signal.
// *signal is assigned the signal if it was found and NULL otherwise
Nucleus_NonNull() static Nucleus_Status
lookupInClasses
    (
        Nucleus_Signal **signal,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    );

Nucleus_NonNull() static Nucleus_Status
addSignal
    (
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    );

#endif
