#include "Nucleus/Object/FinalizationHooks-private.h.i"

#include "Nucleus/Hash/Pointer.h"
#include "Nucleus/EqualTo/Pointer.h"

DEFINE_MODULE_PRIVATE(Nucleus_FinalizationHooks)

Nucleus_NonNull() static Nucleus_Status
__Nucleus_FinalizationHooks_initialize
    (
        Nucleus_FinalizationHooks *module
    )
{
    if (Nucleus_Unlikely(!module)) return Nucleus_Status_InvalidArgument;
    //
	Nucleus_Status status;
	//
    status = Nucleus_Collections_PointerHashMap_initialize(&module->finalizationHookLists,
                                                           8,
                                                           NULL,
														   NULL,
                                                           NUCLEUS_HASHFUNCTION(&Nucleus_hashPointer), // hash
                                                           NUCLEUS_EQUALTOFUNCTION(&Nucleus_equalToPointer), // equal
                                                           NULL,
                                                           NUCLEUS_UNLOCKFUNCTION(&destroyFinalizationHookList));
    if (Nucleus_Unlikely(status))
    {
        return status; 
    }
    //
    module->referenceCount = 1;
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
__Nucleus_FinalizationHooks_uninitialize
    (
        Nucleus_FinalizationHooks *module
    )
{
	Nucleus_Collections_PointerHashMap_uninitialize(&module->finalizationHookLists);
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
createFinalizationHook
	(
		FinalizationHook **finalizationHook,
		Nucleus_Callback *callback
	)
{
	Nucleus_Status status;
	FinalizationHook *temporary;
	status = Nucleus_allocateMemory((void **)&temporary, sizeof(FinalizationHook));
	if (Nucleus_Unlikely(status)) return status;
	temporary->next = NULL;
	temporary->callback = callback;
	*finalizationHook = temporary;
	return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
destroyFinalizationHook
	(
		FinalizationHook *finalizationHook
	)
{
	Nucleus_deallocateMemory(finalizationHook);
	return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
createFinalizationHookList
	(
		FinalizationHookList **finalizationHookList
	)
{
	Nucleus_Status status;
	FinalizationHookList *temporary;
	status = Nucleus_allocateMemory((void **)&temporary, sizeof(FinalizationHookList));
	if (Nucleus_Unlikely(status)) return status;
	temporary->head = NULL;
	*finalizationHookList = temporary;
	return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
destroyFinalizationHookList
	(
		FinalizationHookList *finalizationHookList
	)
{
	while (finalizationHookList->head)
	{
		FinalizationHook *finalizationHook = finalizationHookList->head;
		finalizationHookList->head = finalizationHook->next;
		Nucleus_deallocateMemory(finalizationHook);
	}
	Nucleus_deallocateMemory(finalizationHookList);
	return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
getOrCreateFinalizationHookList
	(
		void *address,
		FinalizationHookList **finalizationHookList
	)
{
	Nucleus_Status status;
	FinalizationHookList *temporary;
	status = Nucleus_Collections_PointerHashMap_get(&g_singleton->finalizationHookLists, address, (void **)&temporary);
	if (status == Nucleus_Status_NotExists)
	{
		status = createFinalizationHookList(&temporary);
		if (Nucleus_Unlikely(status))
		{
			return status;
		}
		status = Nucleus_Collections_PointerHashMap_set(&g_singleton->finalizationHookLists, address, temporary,
		                                                Nucleus_Boolean_False);
		if (Nucleus_Unlikely(status))
		{
			destroyFinalizationHookList(temporary);
			return status;
		}
		
	}
	*finalizationHookList = temporary;
	return Nucleus_Status_Success;
}
