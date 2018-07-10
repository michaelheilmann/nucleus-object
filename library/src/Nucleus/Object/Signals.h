// Copyright (c) 2018 Michael Heilmann
#pragma once

#include "Nucleus/Object/Configuration.h"

#if defined(Nucleus_WithSignals) && (1 == Nucleus_WithSignals)

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Types/Size.h"
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
// The signal may not exist in this type or in any ancestor type of this type.
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Signals_addSignal
    (
        const char *name,
        Nucleus_Type *type
    );

// Remove all signals from a type.
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Signals_removeAllSignals
    (
        Nucleus_Type *type
    );

// Add a connection to an object.
// `name` the name of the signal
// `sink` a pointer to the sink or a null pointer
// `callback` a pointer to the callback function
// If the signal is invoked, then sink is passed as the first argument to the callback, source as
// the second argument, the last argument is a pointer to a Nucleus_Object or derived type object
// or a null pointer depending on the signal. The sink may be a null pointer.
Nucleus_Object_Library_Export Nucleus_NonNull(1, 2, 4) Nucleus_Status
Nucleus_Signals_connect
    (
        Nucleus_Object *source,
        const char *name,
        Nucleus_Object *sink,
        Nucleus_Callback *callback
    );

Nucleus_Object_Library_Export Nucleus_NonNull(1, 2) Nucleus_Status
Nucleus_Signals_getNumberOfConnections
    (
        Nucleus_Object *source,
        Nucleus_Size *count
    );

// Remove the first connection of an object
// with the specified name, the specified sink, and the specified callback.
Nucleus_Object_Library_Export Nucleus_NonNull(1, 2, 4) Nucleus_Status
Nucleus_Signals_disconnect
    (
        Nucleus_Object *source,
        const char *name,
        Nucleus_Object *sink,
        Nucleus_Callback *callback
    );

// Remove all connection of an object.
Nucleus_Object_Library_Export Nucleus_NonNull(1) Nucleus_Status
Nucleus_Signals_disconnectAll
    (
        Nucleus_Object *source
    );
   
Nucleus_Object_Library_Export Nucleus_NonNull(1, 2) Nucleus_Status
Nucleus_Signal_invoke
    (
        Nucleus_Object *source,
        const char *name,
        Nucleus_Object *arguments
    );

#endif
