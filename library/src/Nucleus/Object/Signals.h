// Copyright (c) 2018 Michael Heilmann
#pragma once

// Signals are supported if this is defined and 1.
#define Nucleus_WithSignals (1)

#if defined(Nucleus_WithSignals) && 1 == Nucleus_WithSignals

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Object/Callback.h"
#include "Nucleus/Object/Exports.h"
#include "Nucleus/Object/Module.h"

// Forward declaration.
typedef struct Nucleus_Object Nucleus_Object;
typedef struct Nucleus_Type Nucleus_Type;

// A signal.
typedef struct Nucleus_Signal Nucleus_Signal;

DECLARE_MODULE(Nucleus_Signals)

// Add a signal.
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Signals_addSignal
    (
        const char *name,
        Nucleus_Type *type
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Signals_removeAllSignals
    (
        Nucleus_Type *type
    );

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Signals_connectToSignal
    (
        Nucleus_Object *source,
        const char *name,
        Nucleus_Callback *callback
    );

#endif
