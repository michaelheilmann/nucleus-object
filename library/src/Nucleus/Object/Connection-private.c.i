#include "Nucleus/Object/Connection-private.h.i"

Nucleus_NonNull() static Nucleus_Status
Connection_create
    (
        Connection **connection,
        Nucleus_Signal *signal,
        Nucleus_Callback *callback
    )
{
    Connection *temporary;
    Nucleus_Status status;
    status = Nucleus_allocateMemory((void **)&temporary, sizeof(Connection));
    if (Nucleus_Unlikely(status))
    {
        return status;
    }
    //
    temporary->signal = signal;
    temporary->callback = callback;
    *connection = temporary;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
Connection_destroy
    (
        Connection *connection
    )
{
    Nucleus_deallocateMemory(connection);
    return Nucleus_Status_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NonNull() static Nucleus_Status
Connections_create
    (
        Connections **connections
    )
{
    Nucleus_Status status;
    Connections *temporary;
    status = Nucleus_allocateMemory((void **)&temporary, sizeof(Connections));
    if (Nucleus_Unlikely(status)) return status;
    temporary->connections = NULL;
    *connections = temporary;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
Connections_destroy
    (
        Connections *connections
    )
{
    while (connections->connections)
    {
        Connection *connection = connections->connections;
        Connection_destroy(connection);
    }
    return Nucleus_Status_Success;
}
