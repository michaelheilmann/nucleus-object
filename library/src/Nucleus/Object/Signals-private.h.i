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
#include <string.h>
#include <stdio.h>

// `Nucleus_SignalSystem`
typedef struct Signals Signals;

#define MUTEX int
#define MUTEX_INITIALIZER (0)
#define MUTEX_LOCK(mutex) /* - */
#define MUTEX_UNLOCK(mutex) /* - */

static MUTEX g_mutexSingleton = MUTEX_INITIALIZER;
static Signals *g_singleton = NULL;

struct Signals
{
    int referenceCount;
    Nucleus_Collections_PointerHashMap signals;
}; // struct Signals

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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Nucleus_Signal
{
    Nucleus_ImmutableString *name; // TODO: Add and use Nucleus_Atom.
    Nucleus_Type *type;
} Nucleus_Signal;

Nucleus_NonNull() static Nucleus_Status
Signal_initialize
    (
        Nucleus_Signal *signal,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    );

Nucleus_NonNull() static Nucleus_Status
Signal_uninitialize
    (
        Nucleus_Signal *signal
    );

Nucleus_NonNull() static Nucleus_Status
Signal_create
    (
        Nucleus_Signal **signal,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type
    );

Nucleus_NonNull() static Nucleus_Status
Signal_destroy
    (
        Nucleus_Signal *signal
    );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NonNull() static Nucleus_Status
initialize
    (
        Signals *signals
    );

Nucleus_NonNull() static Nucleus_Status
uninitialize
    (
        Signals *signals
    );

Nucleus_NonNull() static Nucleus_Status
create
    (
        Signals **signals
    );

Nucleus_NonNull() static Nucleus_Status
destroy
    (
        Signals *signals
    );

#endif
