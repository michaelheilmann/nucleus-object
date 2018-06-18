#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Types/Boolean.h"
#include "Nucleus/Types/HashValue.h"
#include "Nucleus/Types/Size.h"

typedef struct Nucleus_ImmutableString Nucleus_ImmutableString;

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_lock
    (
        Nucleus_ImmutableString *w
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_unlock
    (
        Nucleus_ImmutableString *w
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_create
    (
        Nucleus_ImmutableString **w,
        const char *bytes,
        Nucleus_Size numberOfBytes
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_equalTo
    (
        const Nucleus_ImmutableString *v,
        const Nucleus_ImmutableString *w,
        Nucleus_Boolean *equalTo 
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_hash
    (
        const Nucleus_ImmutableString *w,
        Nucleus_HashValue *hashValue 
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_bytes
    (
        const Nucleus_ImmutableString *w,
        const char **bytes,
        Nucleus_Size *numberOfBytes
    );
