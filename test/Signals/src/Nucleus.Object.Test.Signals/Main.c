// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Signals.h"
#include "Nucleus.Object.Test.Signals/A.h"
#include <stdlib.h>

Nucleus_Status static
test();

static Nucleus_Signal *g_keyboardKeyPressed = NULL;
Nucleus_Status notifyShutdownKeyboardKeyPressed()
{ g_keyboardKeyPressed = NULL;  return Nucleus_Status_Success; }

static Nucleus_Signal *g_keyboardKeyReleased = NULL;
Nucleus_Status notifyShutdownKeyboardKeyReleased()
{ g_keyboardKeyReleased = NULL; return Nucleus_Status_Success; }

Nucleus_Status static
test()
{
    Nucleus_Type *type;
    A *a;
    Nucleus_Status status;
    // Get the type of "A".
    status = A_getType(&type);
    if (Nucleus_Unlikely(status))
    { return status; }
    Nucleus_Signal *signal;
    // Add a signal.
    status = Nucleus_Signals_addSignal(&g_keyboardKeyPressed,
                                       u8"keyboard-key-pressed",
                                       type,
                                       &notifyShutdownKeyboardKeyPressed);
    if (Nucleus_Unlikely(status))
    { return status; }
    // Add a signal twice - must fail.
    status = Nucleus_Signals_addSignal(&g_keyboardKeyPressed,
                                       u8"keyboard-key-pressed",
                                       type,
                                       &notifyShutdownKeyboardKeyPressed);
    if (status == Nucleus_Status_Success)
    { return Nucleus_Status_InternalError; }
    // Add a signal.
    status = Nucleus_Signals_addSignal(&g_keyboardKeyReleased,
                                       u8"keyboard-key-released",
                                       type,
                                       &notifyShutdownKeyboardKeyReleased);
    if (Nucleus_Unlikely(status))
    { return status; }
    // Create instance of "A".
    status = A_create(&a);
    if (Nucleus_Unlikely(status))
    { return status; }
    // Destroy instance of "A".
    Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(a));
    // Return success.
    return Nucleus_Status_Success;
}

int
main
    (
        int argc,
        char **argv
    )
{
    Nucleus_Status status;

    // Initialize signals twice to test reference counted singletons.
    status = Nucleus_Signals_initialize();
    if (Nucleus_Unlikely(status)) { return EXIT_FAILURE; }
    status = Nucleus_Signals_initialize();
    if (Nucleus_Unlikely(status)) { return EXIT_FAILURE; }
    //
    status = test();
    if (Nucleus_Unlikely(status))
    {
        goto End;
    }
End:
    // Uninitialize signals twice.
    Nucleus_Signals_uninitialize();
    Nucleus_Signals_uninitialize();

    // Return success.
    if (status) return EXIT_FAILURE;
    else return EXIT_SUCCESS;
}
