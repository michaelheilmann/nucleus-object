// Copyright (c) 2018 Michael Heilmann
#include "Nucleus.Object.Test.Signals/testExplicitDisconnect.h"

static Nucleus_Status
keyboardKeyPressed
    (
        Nucleus_Object *self,
        Nucleus_Object *source,
        Nucleus_Object *arguments
    )
{
    return Nucleus_Status_Success;
}

static Nucleus_Status
keyboardKeyReleased
    (
        Nucleus_Object *self,
        Nucleus_Object *source,
        Nucleus_Object *arguments
    )
{
    return Nucleus_Status_Success;
}

Nucleus_Status
testExplicitDisconnect
    (
    )
{
    Nucleus_Size count;
	Nucleus_Status status;
    A *a;
    // Create instance of "A".
    status = A_create(&a);
    if (Nucleus_Unlikely(status))
    { return status; }
    status = Nucleus_Signals_getNumberOfConnections(NUCLEUS_OBJECT(a), &count);
    if (Nucleus_Unlikely(status))
    { goto End; }
    if (count != 0) { status = Nucleus_Status_InternalError; goto End; }
    // Add signal.
    status = Nucleus_Signals_connect((void *)a, u8"keyboard-key-pressed", NULL, NUCLEUS_CALLBACK(&keyboardKeyPressed));
    if (Nucleus_Unlikely(status))
    { goto End; }
    status = Nucleus_Signals_getNumberOfConnections(NUCLEUS_OBJECT(a), &count);
    if (Nucleus_Unlikely(status))
    { goto End; }
    if (count != 1) { status = Nucleus_Status_InternalError; goto End; }
    // Add signal.
    status = Nucleus_Signals_connect((void *)a, u8"keyboard-key-released", NULL, NUCLEUS_CALLBACK(&keyboardKeyReleased));
    if (Nucleus_Unlikely(status))
    { goto End; }
    status = Nucleus_Signals_getNumberOfConnections(NUCLEUS_OBJECT(a), &count);
    if (Nucleus_Unlikely(status))
    { goto End; }
    if (count != 2) { status = Nucleus_Status_InternalError; goto End; }
    // Remove signal.
    Nucleus_Signals_disconnect((void *)a, u8"keyboard-key-released", NULL, NUCLEUS_CALLBACK(&keyboardKeyReleased));
    status = Nucleus_Signals_getNumberOfConnections(NUCLEUS_OBJECT(a), &count);
    if (Nucleus_Unlikely(status))
    { goto End; }
    if (count != 1) { status = Nucleus_Status_InternalError; goto End; }
    // Remove signal.
    Nucleus_Signals_disconnect((void *)a, u8"keyboard-key-pressed", NULL, NUCLEUS_CALLBACK(&keyboardKeyPressed));
    status = Nucleus_Signals_getNumberOfConnections(NUCLEUS_OBJECT(a), &count);
    if (Nucleus_Unlikely(status))
    { goto End; }
    if (count != 0) { status = Nucleus_Status_InternalError; goto End; }
End:
    // Destroy instance of "A".
    Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(a));
    // Return success.
    return status;
}
