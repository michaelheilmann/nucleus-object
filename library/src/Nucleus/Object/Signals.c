// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/Signals-private.c.i"
#include "Nucleus/Object/ImmutableString.h"

#if defined (Nucleus_WithSignals) && 1 == Nucleus_WithSignals

DEFINE_MODULE(Nucleus_Signals)

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Signals_addSignal
    (
        const char *name,
        Nucleus_Type *type
    )
{
    Nucleus_Status status;
    Nucleus_ImmutableString *name1;
    //
    status = Nucleus_ImmutableString_create(&name1, name, strlen(name));
    if (Nucleus_Unlikely(status)) return status;
    //
    status = addSignal(name1, type);
    Nucleus_ImmutableString_unlock(name1);
    //
    return status;
}

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_Signals_removeAllSignals
    (
        Nucleus_Type *type
    )
{
	Nucleus_Status status;
	Nucleus_Collections_PointerHashMap_Enumerator e;
	status = Nucleus_Collections_PointerHashMap_Enumerator_initialize(&e, &g_singleton->signals);
	if (Nucleus_Unlikely(status))
	{
		return status;
	}
	while (Nucleus_Boolean_True)
	{
		Nucleus_Boolean hasValue;
		//
		status = Nucleus_Collections_PointerHashMap_Enumerator_hasValue(&e, &hasValue);
		if (Nucleus_Unlikely(status))
		{
			Nucleus_Collections_PointerHashMap_Enumerator_uninitialize(&e);
			return status;
		}
		//
		if (!hasValue)
		{
			break;
		}
		//
		void *k, *v;
		status = Nucleus_Collections_PointerHashMap_Enumerator_getValue(&e, &k, &v);
		if (Nucleus_Unlikely(status))
		{
			Nucleus_Collections_PointerHashMap_Enumerator_uninitialize(&e);
			return status;
		}
		//
		if (((Nucleus_Signal *)v)->type == type)
		{
			status = Nucleus_Collections_PointerHashMap_Enumerator_remove(&e);
			if (Nucleus_Unlikely(status))
			{
				Nucleus_Collections_PointerHashMap_Enumerator_uninitialize(&e);
				return status;
			}
		}
		else
		{
			status = Nucleus_Collections_PointerHashMap_Enumerator_next(&e);
			if (Nucleus_Unlikely(status))
			{
				Nucleus_Collections_PointerHashMap_Enumerator_uninitialize(&e);
				return status;
			}
		}
	}
	Nucleus_Collections_PointerHashMap_Enumerator_uninitialize(&e);
	return Nucleus_Status_Success;
}

// Nucleus_Status_NotExists if the signal does not exist.
Nucleus_NonNull() Nucleus_Status
lookupSignal
    (
        Nucleus_Signal **signal,
        Nucleus_Object *source,
        const char *name
    )
{
    Nucleus_Status status;
    // Get the type of the source.
    Nucleus_Type *type = source->type;
    // Get the signal name as an immutable string.
    Nucleus_ImmutableString *nameImmutableString;
    status = Nucleus_ImmutableString_create(&nameImmutableString, name, strlen(name));
    if (Nucleus_Unlikely(status)) return status;
    // Get the signal by its name.
    Nucleus_Signal *temporary;
    status = lookupInClasses(&temporary, nameImmutableString, type);
    Nucleus_ImmutableString_unlock(nameImmutableString);
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    *signal = temporary;
    return Nucleus_Status_Success;
}

Nucleus_Object_Library_Export Nucleus_NonNull(1, 2, 4) Nucleus_Status
Nucleus_Signals_connect
    (
        Nucleus_Object *object,
        const char *name,
        Nucleus_Object *sink,
        Nucleus_Callback *callback
    )
{
    // Get the signal.
    Nucleus_Signal *signal;
    Nucleus_Status status = lookupSignal(&signal, object, name);
    if (Nucleus_Unlikely(status)) return status;
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
    status = Connection_create(&connection, signal, sink, callback);
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    connection->next = connections->connections; connections->connections = connection;
    // Return success.
    return Nucleus_Status_Success;
}

Nucleus_Object_Library_Export Nucleus_NonNull(1, 2) Nucleus_Status
Nucleus_Signals_getNumberOfConnections
    (
        Nucleus_Object *source,
        Nucleus_Size *count
    )
{
    // Get the connections of the source.
    Connections *connections = NULL;
    Nucleus_Status status;
    status = Nucleus_Collections_PointerHashMap_get(&g_singleton->connections, source,
                                                    (void **)&connections);
    // If there are not connections for the source, we are done.
    if (status == Nucleus_Status_NotExists)
    {
        *count = 0;
        return Nucleus_Status_Success;
    }
    // There are connections for the source. Count the connections.
    Nucleus_Size i = 0;
    Connection *current = connections->connections;
    while (current)
    {
        i++;
        current = current->next;
    }
    *count = i;
    return Nucleus_Status_Success;
}

Nucleus_Object_Library_Export Nucleus_NonNull(1, 2, 4) Nucleus_Status
Nucleus_Signals_disconnect
    (
        Nucleus_Object *source,
        const char *name,
        Nucleus_Object *sink,
        Nucleus_Callback *callback
    )
{
    // Get the connections of the source.
    Connections *connections = NULL;
    Nucleus_Status status;
    status = Nucleus_Collections_PointerHashMap_get(&g_singleton->connections, source,
                                                    (void **)&connections);
    // If there are not connections for the source, we are done.
    if (status == Nucleus_Status_NotExists) return Nucleus_Status_Success;
    // Get the signal.
    Nucleus_Signal *signal;
    status = lookupSignal(&signal, source, name);
    if (Nucleus_Unlikely(status)) return status;
    // There are connections for the source: Search and remove the connection.
    Connection **previous = &(connections->connections),
                *current = connections->connections;
    while (current)
    {
        if (current->sink == sink && current->signal == signal && current->callback == callback)
        {
            *previous = current->next;
            current->next = NULL;
            Connection_destroy(current);
            break;
        }
        else
        {
            previous = &current->next;
            current = current->next;
        }
    }
    // Return success.
    return Nucleus_Status_Success;
}

Nucleus_Object_Library_Export Nucleus_NonNull(1) Nucleus_Status
Nucleus_Signals_disconnectAll
    (
        Nucleus_Object *source
    )
{
    // Get the connections of the source.
    Connections *connections = NULL;
    Nucleus_Status status;
    status = Nucleus_Collections_PointerHashMap_get(&g_singleton->connections, source,
                                                    (void **)&connections);
    // If there are not connections for the source, we are done.
    if (status == Nucleus_Status_NotExists) return Nucleus_Status_Success;
    // If there are connections, remove them.
    // TODO: Add and utilize Connections_clear().
    while (connections->connections)
    {
        Connection *connection = connections->connections;
        connections->connections = connection->next;
        Connection_destroy(connection);
    }
    Nucleus_Collections_PointerHashMap_remove(&g_singleton->connections, source);
    // Return success.
    return Nucleus_Status_Success;   
}

#endif
