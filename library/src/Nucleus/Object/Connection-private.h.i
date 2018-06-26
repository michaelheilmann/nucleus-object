#pragma once


// We maintain a (hash) map from object addreses to object connections.     When connecting to some
// signal of some object o, then the entry (o,C(o)) is added where C(o) are the object connections.
// In C(o) all connections between somes signal for o are stored.  With one direct hash it is hence
// possible to find the object connections C(o) for some object o.


#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Object/Callback.h"

typedef struct Connection Connection;
struct Connection
{
    Connection *next;
    Nucleus_Signal *signal;
    Nucleus_Callback *callback;
}; // struct Connection

Nucleus_NonNull() static Nucleus_Status
Connection_create
    (
        Connection **connection,
        Nucleus_Signal *signal,
        Nucleus_Callback *callback
    );

Nucleus_NonNull() static Nucleus_Status
Connection_destroy
    (
        Connection *connection
    );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Connections
{
    Connection *connections;
} Connections;

Nucleus_NonNull() static Nucleus_Status
Connections_create
    (
        Connections **connections
    );

Nucleus_NonNull() static Nucleus_Status
Connections_destroy
    (
        Connections *connections
    );
