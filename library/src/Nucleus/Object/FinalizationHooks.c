#include "Nucleus/Object/FinalizationHooks-private.c.i"

DEFINE_MODULE(Nucleus_FinalizationHooks)

Nucleus_Status
Nucleus_FinalizationHooks_add
    (
        void *pointer,
        Nucleus_Callback *callback
    )
{
    if (Nucleus_Unlikely(!callback)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    // Get or create the finalization hook list.
    FinalizationHookList *finalizationHookList;
    status = getOrCreateFinalizationHookList(pointer, &finalizationHookList);
    // Create the finalization hook. Add the finalization hook to the finalization hook list.
    if (Nucleus_Unlikely(status)) return status;
    FinalizationHook *finalizationHook;
    status = createFinalizationHook(&finalizationHook, callback);
    if (Nucleus_Unlikely(status)) return status;
    finalizationHook->next = finalizationHookList->head;
    finalizationHookList->head = finalizationHook;
    // Return success.
    return Nucleus_Status_Success;
}

Nucleus_Status
Nucleus_FinalizationHooks_invoke
    (
        void *pointer
    )   
{
    void *p;
    FinalizationHookList *k;
    Nucleus_Status status;
    status = Nucleus_Collections_PointerHashMap_removeExtended(&g_singleton->finalizationHookLists, pointer,
                                                               (void **)&p, (void **)&k,
                                                               Nucleus_Boolean_False, Nucleus_Boolean_False);
    if (Nucleus_Status_NotFound == status)
    {
        return Nucleus_Status_Success;
    }
    else if (status)
    {
        return status;
    }
    while (k->head)
    {
        FinalizationHook *finalizationHook = k->head;
        k->head = finalizationHook->next;
        ((Nucleus_Status (*)(void *))finalizationHook->callback)(pointer);
        destroyFinalizationHook(finalizationHook);
    }
    destroyFinalizationHookList(k);
    return Nucleus_Status_Success; 
}

Nucleus_Status
Nucleus_FinalizationHooks_removeAll
    (
        void *pointer
    )
{
    void *p;
    FinalizationHookList *k;
    Nucleus_Status status;
    status = Nucleus_Collections_PointerHashMap_removeExtended(&g_singleton->finalizationHookLists, pointer,
                                                               (void **)&p, (void **)&k,
                                                               Nucleus_Boolean_False, Nucleus_Boolean_False);
    if (Nucleus_Status_NotFound == status)
    {
        return Nucleus_Status_Success;
    }
    else if (status)
    {
        return status;
    }
    while (k->head)
    {
        FinalizationHook *finalizationHook = k->head;
        k->head = finalizationHook->next;
        destroyFinalizationHook(finalizationHook);
    }
    destroyFinalizationHookList(k);
    return Nucleus_Status_Success; 
}
