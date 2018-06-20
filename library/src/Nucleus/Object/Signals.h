// Copyright (c) 2018 Michael Heilmann
#pragma once

// Signals are supported if this is defined and 1.
#define Nucleus_WithSignals (1)

#if defined(Nucleus_WithSignals) && 1 == Nucleus_WithSignals

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Object/Exports.h"

// Forward declaration.
typedef struct Nucleus_Type Nucleus_Type;

// A signal.
typedef struct Nucleus_Signal Nucleus_Signal;

// # `Nucleus_Signals_initialize`
// *Initialize the `Nucleus_Signals` singleton.
Nucleus_Object_Library_Export Nucleus_Status
Nucleus_Signals_initialize
    (
    );
    
// # `Nucleus_Signals_uninitialize`
// *Uninitialize the `Nucleus_Signals` singleton.*
Nucleus_Object_Library_Export Nucleus_Status
Nucleus_Signals_uninitialize
    (
    );

// Add a signal.
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Signals_addSignal
    (
        Nucleus_Signal **signal,
        const char *name,
        Nucleus_Type *type,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)()
    );

#endif
