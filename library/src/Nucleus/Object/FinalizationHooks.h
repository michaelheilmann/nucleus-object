#pragma once

#include "Nucleus/Object/Configuration.h"

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Object/Exports.h"
#include "Nucleus/Object/Module.h"
#include "Nucleus/Object/Callback.h"

#if defined(Nucleus_WithFinalizationHooks) && 1 == Nucleus_WithFinalizationHooks

DECLARE_MODULE(Nucleus_FinalizationHooks)

// Add a finalization hook for the specified pointer (which might be a null pointer). The callback
// is passed the pointer - which may not be dereferenced - as its only argument when the
// finalization is invoked.
Nucleus_Object_Library_Export Nucleus_Status
Nucleus_FinalizationHooks_add
    (
        void *pointer,
		Nucleus_Callback *callback
    );

Nucleus_Object_Library_Export Nucleus_Status
Nucleus_FinalizationHooks_invoke
    (
        void *pointer
    );

Nucleus_Object_Library_Export Nucleus_Status
Nucleus_FinalizationHooks_removeAll
	(
		void *pointer
	);

#endif
