// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Signals-private.c.i"
#include "Nucleus/Object/ImmutableString.h"

#if defined (Nucleus_WithSignals) && 1 == Nucleus_WithSignals

Nucleus_Status
Nucleus_Signals_initialize
    (
    )
{
    MUTEX_LOCK(&g_singletonMutex);
    if (NULL != g_singleton)
    {
        g_singleton->referenceCount++;
        MUTEX_UNLOCK(&g_singletonMutex);
        return Nucleus_Status_Success;
    }
    else
    {
        Nucleus_Status status = create(&g_singleton);
        MUTEX_UNLOCK(&g_singletonMutex);
        return status;
    }
}

Nucleus_Status
Nucleus_Signals_uninitialize
    (
    )
{
    MUTEX_LOCK(&g_singletonMutex);
    if (NULL == g_singleton)
    {
        MUTEX_UNLOCK(&g_singletonMutex);
        return Nucleus_Status_NotExists;
    }
    if (0 == --g_singleton->referenceCount)
    {
        destroy(g_singleton);
        g_singleton = NULL;
        MUTEX_UNLOCK(&g_singletonMutex);
        return Nucleus_Status_Success;
    }
    return Nucleus_Status_Success;
}

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
addSignal
    (
        Nucleus_Signal **signal,
        Nucleus_ImmutableString *name,
        Nucleus_Type *type,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)()
    )
{
    Nucleus_Status status;
    // Search the class for the signal.
    Nucleus_Signal *s;
    status = lookupInClasses(&s, name, type);
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    if (s)
    {
        fprintf(stderr, u8"%s:%d: signal already exists\n", __FILE__, __LINE__);
        return Nucleus_Status_Exists;
    }
    // The signal does not exist. Create it, add it.
    status = Signal_create(&s, name, type);
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    SignalKey *k;
    status = SignalKey_create(&k, name, type);
    if (Nucleus_Unlikely(status))
    {
        Signal_destroy(s);
        return status;
    }
    status = Nucleus_Collections_PointerHashMap_set(&g_singleton->signals,
                                                    (void *)k, (void *)s,
                                                    Nucleus_Boolean_False);
    if (Nucleus_Unlikely(status))
    {
        SignalKey_destroy(k);
        Signal_destroy(s);
        return status;
    }

    // Return success.
    return Nucleus_Status_Success;
}

// Add a signal.
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Signals_addSignal
    (
        Nucleus_Signal **signal,
        const char *name,
        Nucleus_Type *type,
        Nucleus_AlwaysSucceed() Nucleus_Status (*notifyShutdown)()
    )
{
    Nucleus_Status status;
    Nucleus_ImmutableString *name1;
    //
    status = Nucleus_ImmutableString_create(&name1, name, strlen(name));
    if (Nucleus_Unlikely(status)) return status;
    //
    status = addSignal(signal, name1, type, notifyShutdown);
    Nucleus_ImmutableString_unlock(name1);
    //
    return status;
}

#endif
