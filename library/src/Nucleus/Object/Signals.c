// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Signals-private.c.i"
#include "Nucleus/Object/ImmutableString.h"

#if defined (Nucleus_WithSignals) && 1 == Nucleus_WithSignals

DEFINE_MODULE(Nucleus_Signals)

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

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Signals_connectToSignal
    (
        Nucleus_Object *object,
        const char *name,
        Nucleus_Callback *callback
    )
{
    // Get the type of the object.
    Nucleus_Type *type = object->type;
    // Get the signal name as an immutable string.
    Nucleus_Status status;
    Nucleus_ImmutableString *name1;
    status = Nucleus_ImmutableString_create(&name1, name, strlen(name));
    if (Nucleus_Unlikely(status)) return status;
    // Get the signal by its name.
    Nucleus_Signal *signal;
    status = lookupInClasses(&signal, name1, type);
    Nucleus_ImmutableString_unlock(name1);
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    // Get or create the connections for the object.
    Connections *connections = NULL;
    status = Nucleus_Collections_PointerHashMap_get(&g_singleton->connections, object,
                                                    (void **)&connections);
    if (Nucleus_Unlikely(status))
    {
        if (Nucleus_Status_NotExists == status)
        {
            status = Connections_create(&connections);
            if (Nucleus_Unlikely(status)) return status;
            status = Nucleus_Collections_PointerHashMap_set(&g_singleton->connections, object,
                                                            connections, Nucleus_Boolean_False);
            if (Nucleus_Unlikely(status))
            {
                Connections_destroy(connections);
                return status;
            }
        }
        else
        {
            return status;
        }
    }
    // Add the connection to the connections of the object.
    Connection *connection = NULL;
    status = Connection_create(&connection, signal, callback);
    if (Nucleus_Unlikely(status)) return status;
    connection->next = connections->connections; connections->connections = connection;
    // Return success.
    return Nucleus_Status_Success;
}

#endif
