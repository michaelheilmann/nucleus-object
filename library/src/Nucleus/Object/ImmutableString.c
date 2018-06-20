#include "Nucleus/Object/ImmutableString.h"

#include "Nucleus/Hash/Memory.h"
#include "Nucleus/Memory.h"

struct Nucleus_ImmutableString
{
    int referenceCount;
    Nucleus_HashValue hashValue;
    Nucleus_Size numberOfBytes;
    char bytes[];
}; // struct Nucleus_ImmutableString

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_lock
    (
        Nucleus_ImmutableString *immutableString
    )
{
    if (Nucleus_Unlikely(!immutableString))
    { return Nucleus_Status_InvalidArgument; }
    immutableString->referenceCount++;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_unlock
    (
        Nucleus_ImmutableString *immutableString
    )
{
    if (Nucleus_Unlikely(!immutableString))
    { return Nucleus_Status_InvalidArgument; }
    if (0 == --immutableString->referenceCount)
    { Nucleus_deallocateMemory(immutableString); }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_create
    (
        Nucleus_ImmutableString **immutableString,
        const char *bytes,
        Nucleus_Size numberOfBytes
    )
{
    Nucleus_ImmutableString *temporary;
    // Validate arguments.
    if (Nucleus_Unlikely(!immutableString || !bytes))
    { return Nucleus_Status_InvalidArgument; }
    if (Nucleus_Unlikely(Nucleus_Size_Greatest - sizeof(Nucleus_ImmutableString) < numberOfBytes))
    { return Nucleus_Status_InvalidArgument; }        
    // Allocate.
    Nucleus_Status status;
    status = Nucleus_allocateMemory((void **)&temporary,
                                    sizeof(Nucleus_ImmutableString) + numberOfBytes);
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    // Reference count.
    temporary->referenceCount = 1;
    // Number of Bytes.
    temporary->numberOfBytes = numberOfBytes;
    // Hash value.
    status = Nucleus_hashMemory(bytes, numberOfBytes, &temporary->hashValue);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(temporary);
        return status;
    }
    // Bytes.
    status = Nucleus_copyMemory(temporary->bytes, bytes, numberOfBytes);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(temporary);
        return status;
    }
    // Return string.
    *immutableString = temporary;
    // Return success.
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_equalTo
    (
        const Nucleus_ImmutableString *v,
        const Nucleus_ImmutableString *w,
        Nucleus_Boolean *equalTo 
    )
{
    if (Nucleus_Unlikely(!v || !w || !equalTo)) return Nucleus_Status_InvalidArgument;
    if (v == w)
    {
        *equalTo = Nucleus_Boolean_True;
        return Nucleus_Status_Success;
    }
    if (v->hashValue == w->hashValue &&
        v->numberOfBytes == w->numberOfBytes)
    {
        return Nucleus_compareMemory(v->bytes, w->bytes, v->numberOfBytes, equalTo);
    }
    *equalTo = Nucleus_Boolean_False;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_hash
    (
        const Nucleus_ImmutableString *w,
        Nucleus_HashValue *hashValue 
    )
{
    if (Nucleus_Unlikely(!w || !hashValue)) return Nucleus_Status_InvalidArgument;
    *hashValue = w->hashValue;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_ImmutableString_bytes
    (
        const Nucleus_ImmutableString *w,
        const char **bytes,
        Nucleus_Size *numberOfBytes
    )
{
    if (Nucleus_Unlikely(!w || !bytes || !numberOfBytes)) return Nucleus_Status_InvalidArgument;
    *bytes = w->bytes;
    *numberOfBytes = w->numberOfBytes;
    return Nucleus_Status_Success;
}
