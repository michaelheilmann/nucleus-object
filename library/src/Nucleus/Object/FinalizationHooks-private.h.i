#pragma once

#include "Nucleus/Object/FinalizationHooks.h"
#include "Nucleus/Memory.h"
#include "Nucleus/Collections/PointerHashMap.h"

#define Nucleus_FinalizationHooks_Mutex int
#define Nucleus_FinalizationHooks_Mutex_Initializer (0)
#define Nucleus_FinalizationHooks_Mutex_lock(mutex) /* - */
#define Nucleus_FinalizationHooks_Mutex_unlock(mutex) /* - */
DECLARE_MODULE_PRIVATE(Nucleus_FinalizationHooks)

typedef struct FinalizationHook FinalizationHook;
struct FinalizationHook
{
	FinalizationHook *next;
	Nucleus_Callback *callback;
}; // struct FinalizationHook

Nucleus_NonNull() Nucleus_Status
createFinalizationHook
	(
		FinalizationHook **finalizationHook,
		Nucleus_Callback *callback
	);

Nucleus_NonNull() Nucleus_Status
destroyFinalizationHook
	(
		FinalizationHook *finalizationHook
	);

typedef struct FinalizationHookList FinalizationHookList;
struct FinalizationHookList
{
	FinalizationHook *head;
}; // struct FinalizationHookList

Nucleus_NonNull() Nucleus_Status
createFinalizationHookList
	(
		FinalizationHookList **finalizationHookList
	);

Nucleus_NonNull() Nucleus_Status
destroyFinalizationHookList
	(
		FinalizationHookList *finalizationHookList
	);

Nucleus_NonNull() Nucleus_Status
getOrCreateFinalizationHookList
	(
		void *address,
		FinalizationHookList **finalizationHookList
	);

struct Nucleus_FinalizationHooks
{
    int referenceCount;
	// Map from object addresses to finalization hook lists.
	Nucleus_Collections_PointerHashMap finalizationHookLists;
}; // struct Nucleus_FinalizationHooks
