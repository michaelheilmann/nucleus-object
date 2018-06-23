#pragma once

#include "Nucleus/Object/ImmutableString.h"

typedef struct Nucleus_Signal
{
    Nucleus_ImmutableString *name; // TODO: Add and use Nucleus_Atom.
    Nucleus_Type *type;
    Nucleus_HashValue hashValue;
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
